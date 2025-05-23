#include "Renderer.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/trace/Trace.h"
#include "ui/renderable/primitive/Rectangle.h"
#include "ui/renderable/element/DisplayBuffer.h"
#include "ui/renderable/element/Polygon.h"

namespace
{
	const float inlineVerticies[] = {
		-1, -1, 0,
		-1, 1, 0,
		1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		1, 1, 0
	};

	const float inlineUVCoords[] = {
		0, 0,
		0, 1,
		1, 0,
		1, 0,
		0, 1,
		1, 1
	};

	const auto bufferExpandFactor = 50;
	const auto commonVertexStride = sizeof(float) * 3;
	const auto commonUVStride = sizeof(float) * 2;
	const auto commonOffset = 0;

	const auto cullingDistance = 5000.0;
}


namespace AW
{
	Renderer::Renderer(const ScreenConfig& screenConfig, std::shared_ptr<Renderer> oldRenderer)
	{
		renderPositionModeStack.push(RenderPositionMode::Positioned);
		renderProcessingStack.push(RenderPositionProcessingMode::None);
		textureModeStack.push(RenderTextureMode::LinearNoWrap);
		renderDepthStack.push(RenderDepthTestMode::Disabled);
		renderMultiSampleModeStack.push(RenderMultiSampleMode::Disabled);
		renderTargetStack.push(RenderTargetMode::Screen);
		renderColorMode.push(RenderColorMode::Multiplicative);

		for (auto i = 0; i < bufferExpandFactor; ++i)
		{
			renderBuffer.push_back(RenderPackage());
		}

		if (oldRenderer != nullptr)
		{
			oldRenderer->releaseOpenGLObjects();
		}
		else
		{
			setClearColor(0xff, 0xff, 0xff, 0xff);
		}

		currentScreenConfig = screenConfig;
	}


	bool Renderer::initOpenGL(SDL_Window* window)
	{
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr)
		{
			Logger::instance()->logFatal("Renderer::SDL_GL_CreateContext failed to init return an OpenGL context");
			return false;
		}

		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			Logger::instance()->logFatal("Renderer::GLEW failed to init\n" + std::string((char*)glewGetErrorString(glewError)));
			return false;
		}

		int major, minor;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);

		if (major != currentScreenConfig.openGLMajorVersion || minor != currentScreenConfig.openGLMinorVersion)
		{
			Logger::instance()->logCritical(
				"Renderer::OpenGL::Did not get expected context "
				+ std::to_string(currentScreenConfig.openGLMajorVersion) + "." + std::to_string(currentScreenConfig.openGLMinorVersion)
				+ ", got major=" + std::to_string(major) + ", minor=" + std::to_string(minor)
			);
		}

		if (vao == 0)
		{
			GLDbgCall(glGenVertexArrays(1, &vao));
			GLDbgCall(glBindVertexArray(vao));
		}

		if (defaultVertexBuffer == 0)
		{
			GLDbgCall(glGenBuffers(1, &defaultVertexBuffer));
			GLDbgCall(glBindBuffer(GL_ARRAY_BUFFER, defaultVertexBuffer));
			GLDbgCall(glBufferData(GL_ARRAY_BUFFER, sizeof(inlineVerticies), inlineVerticies, GL_STATIC_DRAW));
		}

		if (defaultTextureUVBuffer == 0)
		{
			GLDbgCall(glGenBuffers(1, &defaultTextureUVBuffer));
			GLDbgCall(glBindBuffer(GL_ARRAY_BUFFER, defaultTextureUVBuffer));
			GLDbgCall(glBufferData(GL_ARRAY_BUFFER, sizeof(inlineUVCoords), inlineUVCoords, GL_STATIC_DRAW));
		}

		generateBackgroundRenderBuffer();

		layerFactor = (1 << 16) / maxLayers;

		if (defaultWhiteTexture == 0)
		{
			glGenTextures(1, &defaultWhiteTexture);
			unsigned char data[] = { 255, 255, 255, 255 };
			glBindTexture(GL_TEXTURE_2D, defaultWhiteTexture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		return true;
	}

	void Renderer::setDefaultShaders(std::shared_ptr<ShaderReference> vertexShader, std::shared_ptr<ShaderReference> fragmentShader)
	{
		defaultVertexShader = vertexShader;
		defaultFragmentShader = fragmentShader;
	}

	Renderer::~Renderer()
	{
		releaseOpenGLObjects();
	}

	void Renderer::releaseOpenGLObjects()
	{
		if (glContext != NULL)
		{
			SDL_GL_DeleteContext(glContext);
			glContext = NULL;
		}

		if (vao != 0)
		{
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}

		if (defaultVertexBuffer != 0)
		{
			glDeleteBuffers(1, &defaultVertexBuffer);
			defaultVertexBuffer = 0;
		}

		if (defaultTextureUVBuffer != 0)
		{
			glDeleteBuffers(1, &defaultTextureUVBuffer);
			defaultTextureUVBuffer = 0;
		}

		for (const auto programPairToProgramId : programs)
		{
			glDeleteProgram(programPairToProgramId.second);
		}
		programs.clear();

		if (backgroundRenderBuffer != 0)
		{
			glDeleteRenderbuffers(1, &backgroundRenderBuffer);
			backgroundRenderBuffer = 0;
		}

		if (defaultWhiteTexture != 0)
		{
			glDeleteTextures(1, &defaultWhiteTexture);
			defaultWhiteTexture = 0;
		}
	}

	void Renderer::setClearColor(int r, int g, int b, int a)
	{
		clearColor.r = r;
		clearColor.g = g;
		clearColor.b = b;
		clearColor.a = a;
	}

	void Renderer::setClearEnabled(bool flag)
	{
		clearEnabled = flag;
	}

	void Renderer::render(std::shared_ptr<Renderable> root, Screen* screen, double frameTimestamp)
	{
		preRender(screen, frameTimestamp);
		renderOpenGL(root);
		postRender();
	}

	void Renderer::renderOpenGL(std::shared_ptr<Renderable> root)
	{
		glClearColor(clearColor.r / 255.0, clearColor.g / 255.0, clearColor.b / 255.0, clearColor.a / 255.0);

		if (clearEnabled && clearColor.a != 0)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		const auto width = screenWidth, height = screenHeight;

		double lowerDepthLayer = -(maxLayers / 2) * (double)layerFactor;
		double upperDepthLayer = (maxLayers / 2) * (double)layerFactor;
		LM::mat4x4_ortho(p, 0.0, width, height, 0.0, lowerDepthLayer, upperDepthLayer);
		LM::mat4x4_ortho(pAbs, 0, width, height, 0.0, lowerDepthLayer, upperDepthLayer);

		glEnableVertexAttribArray(0);
		GLDbgCall(setVertexAttributePointer(defaultVertexBuffer, commonVertexStride, commonOffset));

		glEnableVertexAttribArray(1);
		GLDbgCall(setUVAttributePointer(defaultTextureUVBuffer, commonUVStride, commonOffset));

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderRecursive(getNextRenderPackageForObj(root));

		glDisable(GL_BLEND);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glUseProgram(0);
		currentProgramId = 0;
	}

	void Renderer::openGLDrawTriangles(RenderPackage* renderPackage)
	{
		if (renderPackage->stencilDepth > 0)
		{
			glStencilFunc(GL_EQUAL, 1, 0xFF);
		}

		if (!depthEnabled && renderDepthStack.top() == RenderDepthTestMode::Enabled)
		{
			glEnable(GL_DEPTH_TEST);
			depthEnabled = true;
		}
		else if (depthEnabled)
		{
			glDisable(GL_DEPTH_TEST);
			depthEnabled = false;
		}

		if (!msaaEnabled && renderMultiSampleModeStack.top() == RenderMultiSampleMode::Enabled)
		{
			glEnable(GL_MULTISAMPLE);
			msaaEnabled = true;
		}
		else if (msaaEnabled && renderMultiSampleModeStack.top() == RenderMultiSampleMode::Disabled)
		{
			glDisable(GL_MULTISAMPLE);
			msaaEnabled = false;
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (currentScreenConfig.openGlWireframeMode)
		{
			glUniform4f(inColorModLocation, 1.0, 0, 0, 1.0);
			GLDbgCall(glDrawArrays(GL_LINE_LOOP, 0, 6));
		}
	}

	void Renderer::openGLDrawArraysStencil(RenderPackage* renderPackage)
	{
		if (renderPackage->stencilDepth > 0)
		{
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
		}

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		if (currentScreenConfig.visualizeClipRects)
		{
			glUniform4f(inColorModLocation, 1.0, 0, 0, 1.0);
			GLDbgCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		}

		glDepthMask(GL_TRUE);
	}


	unsigned int Renderer::getUniformLocationForCurrentProgram(const std::string& paramName, unsigned int programId)
	{
		if (programIdToProgramUniformMapId.count(programId) == 0)
		{
			programIdToProgramUniformMapId[programId] = std::unordered_map<std::string, unsigned int>();
		}

		if (programIdToProgramUniformMapId.at(programId).count(paramName) == 0)
		{
			programIdToProgramUniformMapId[programId][paramName] = glGetUniformLocation(programId, paramName.c_str());
		}

		return programIdToProgramUniformMapId.at(programId).at(paramName);
	}

	void Renderer::generateBackgroundRenderBuffer()
	{
		if (backgroundRenderBuffer != 0)
		{
			glDeleteRenderbuffers(1, &backgroundRenderBuffer);
			backgroundRenderBuffer = 0;
		}

		glGenRenderbuffers(1, &backgroundRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, backgroundRenderBuffer);
		GLDbgCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, currentScreenConfig.width, currentScreenConfig.height));
		// TODO: If MSAA is needed
		//glRenderbufferStorageMultisample(GL_RENDERBUFFER, currentScreenConfig.msaaSamples, GL_DEPTH24_STENCIL8, currentScreenConfig.width, currentScreenConfig.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Renderer::openGLDrawPoints(RenderPackage* renderPackage, unsigned int vBuffer, unsigned int numPoints, bool isFilled)
	{
		if (renderPackage->stencilDepth > 0)
		{
			glStencilFunc(GL_EQUAL, 1, 0xFF);
		}

		if (!depthEnabled && renderDepthStack.top() == RenderDepthTestMode::Enabled)
		{
			glEnable(GL_DEPTH_TEST);
			depthEnabled = true;
		}
		else if (depthEnabled)
		{
			glDisable(GL_DEPTH_TEST);
			depthEnabled = false;
		}

		if (!msaaEnabled && renderMultiSampleModeStack.top() == RenderMultiSampleMode::Enabled)
		{
			glEnable(GL_MULTISAMPLE);
			msaaEnabled = true;
		}
		else if (msaaEnabled && renderMultiSampleModeStack.top() == RenderMultiSampleMode::Disabled)
		{
			glDisable(GL_MULTISAMPLE);
			msaaEnabled = false;
		}

		GLDbgCall(setVertexAttributePointer(vBuffer, commonVertexStride, commonOffset));
		setUVAttributePointer(vBuffer, commonVertexStride, commonOffset);
		GLDbgCall(glDrawArrays(isFilled ? GL_TRIANGLE_FAN : GL_LINE_STRIP, 0, numPoints));

		if (currentScreenConfig.openGlWireframeMode)
		{
			glUniform4f(inColorModLocation, 1.0, 0, 0, 1.0);
			GLDbgCall(glDrawArrays(GL_LINE_LOOP, 0, numPoints));
		}

		setVertexAttributePointer(defaultVertexBuffer, commonVertexStride, commonOffset);
		setUVAttributePointer(defaultTextureUVBuffer, commonUVStride, commonOffset);
	}

	unsigned int Renderer::generateVertexBuffer(const std::vector<AWVec2<float>>& points)
	{
		std::vector<float> pts;
		for (const auto p : points)
		{
			pts.push_back(p.x);
			pts.push_back(p.y);
			pts.push_back(0.f);
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, pts.size() * 4, &pts[0], GL_STATIC_DRAW);

		return buffer;
	}

	void Renderer::setVertexAttributePointer(const unsigned int bufferId, const unsigned int stride, const unsigned int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)0);
	}

	void Renderer::setUVAttributePointer(const unsigned int bufferId, const unsigned int stride, const unsigned int offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void*)0);
	}

	bool Renderer::shouldCull(const RenderPackage* const renderPackage)
	{
		return
			renderPackage->computed.x < -cullingDistance ||
			renderPackage->computed.x > cullingDistance ||
			renderPackage->computed.y < -cullingDistance ||
			renderPackage->computed.y > cullingDistance;
	}

	void Renderer::changeProgram(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		auto vertexShader = rend->getVertexShader();
		if (vertexShader == nullptr)
		{
			vertexShader = defaultVertexShader;
			vertexShader->setCachedProgramId(rend->cachedProgramId);
		}

		auto fragmentShader = rend->getFragmentShader();
		if (fragmentShader == nullptr)
		{
			fragmentShader = defaultFragmentShader;
		}

		changeProgram(vertexShader, fragmentShader);

		rend->cachedProgramId = vertexShader->getCachedProgramId();

		applyShaderUniforms(vertexShader, renderPackage);
		applyShaderUniforms(fragmentShader, renderPackage);
	}

	void Renderer::changeProgram(const std::shared_ptr<ShaderReference>& vertexShader, const std::shared_ptr<ShaderReference>& fragmentShader)
	{
		if (vertexShader->getCachedProgramId() != 0)
		{
			changeProgram(vertexShader->getCachedProgramId());
		}
		else
		{
			const auto programId = createAndLinkProgramIfNeeded(vertexShader->getShaderIds(), fragmentShader->getShaderIds(), fragmentShader->getLoaderId());
			vertexShader->setCachedProgramId(programId);
			changeProgram(programId);
		}
	}

	void Renderer::changeProgram(unsigned int programId)
	{
		if (currentProgramId != programId)
		{
			glUseProgram(programId);
			currentProgramId = programId;

			inMatrixLocation = getUniformLocationForCurrentProgram("mvp", programId);
			inColorModLocation = getUniformLocationForCurrentProgram("cMod", programId);
		}
	}

	std::string Renderer::getKeyForShaders(const std::vector<unsigned int> vertexShaderIds, const std::vector<unsigned int> fragmentShaderIds)
	{
		auto result = std::string();

		for (const auto vertexShaderId : vertexShaderIds)
		{
			result += 'v' + std::to_string(vertexShaderId);
		}

		result += '_';

		for (const auto fragmentShaderId : fragmentShaderIds)
		{
			result += 'f' + std::to_string(fragmentShaderId);
		}

		return result;
	}

	unsigned int Renderer::createAndLinkProgramIfNeeded(const std::vector<unsigned int> vertexShaderIds, const std::vector<unsigned int> fragmentShaderIds, unsigned int loaderShaderId)
	{
		const auto key = getKeyForShaders(vertexShaderIds, fragmentShaderIds);
		if (programs.count(key) == 0)
		{
			const auto programId = glCreateProgram();
			GLDbgCall(glAttachShader(programId, vertexShaderIds[0]));
			for (const auto fragmentShaderId : fragmentShaderIds)
			{
				GLDbgCall(glAttachShader(programId, fragmentShaderId));
			}
			GLDbgCall(glAttachShader(programId, loaderShaderId));
			GLDbgCall(glLinkProgram(programId));

			GLint programLinkSuccess = GL_TRUE;
			glGetProgramiv(programId, GL_LINK_STATUS, &programLinkSuccess);

			if (programLinkSuccess != GL_TRUE)
			{
				char logBuf[1024];
				int len;
				glGetProgramInfoLog(programId, sizeof(logBuf), &len, logBuf);
				reportOpenGLErrors();
				Logger::instance()->logFatal("Renderer::OpenGL::Failed to link program\n" + std::string(logBuf, len));
				programs[key] = 0;
			}
			else
			{
				programs[key] = programId;
			}
		}

		return programs.at(key);
	}

	void Renderer::applyShaderUniforms(const std::shared_ptr<ShaderReference>& shader, const RenderPackage* renderPackage)
	{
		if (shader == nullptr)
		{
			return;
		}

		const auto zoom = renderPackage == nullptr ? 1.0 : renderPackage->zoom;
		ShaderUniformInfoBundle info{ currentFrameTimestamp, screenWidth / zoom, screenHeight / zoom };

		shader->setUniforms(currentProgramId, info);
	}

	void Renderer::setViewport(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
		screenWidth = width;
		screenHeight = height;
	}

	void Renderer::reportOpenGLErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			Logger::instance()->logCritical("OpenGL::Error reported\n" + std::to_string(err));
		}
	}

	void Renderer::updateScreenConfig(const ScreenConfig& config)
	{
		currentScreenConfig = config;

		generateBackgroundRenderBuffer();
	}

	void Renderer::preRender(Screen* screen, double frameTimestamp)
	{
		currentFrameTimestamp = frameTimestamp;
		screenWidth = screen->getWidth();
		screenHeight = screen->getHeight();
		setViewport(screenWidth, screenHeight);
		nextPackage = renderBuffer.begin();
		hasFallenBackToRenderList = false;
	}

	void Renderer::postRender()
	{
		if (!renderBufferFallbackList.empty())
		{
			for (auto& pkg : renderBufferFallbackList)
			{
				renderBuffer.push_back(pkg);
			}

			renderBufferFallbackList.clear();
		}

		for (auto& pkg : renderBuffer)
		{
			pkg.obj = std::shared_ptr<Renderable>();
		}
	}

	RenderPackage* Renderer::getNextRenderPackageForObj(std::shared_ptr<Renderable> obj, RenderPackage* previous)
	{
		auto pkg = !hasFallenBackToRenderList ? &(*nextPackage) : &(*nextPackageFallbackList);

		if (previous != nullptr)
		{
			(*pkg) = *previous;
		}
		else
		{
			*pkg = RenderPackage();
		}

		pkg->obj = obj;

		updateNextBufferPackageIterator();

		return pkg;
	}

	void Renderer::updateNextBufferPackageIterator()
	{
		if (hasFallenBackToRenderList)
		{
			renderBufferFallbackList.push_back(RenderPackage());
			++nextPackageFallbackList;
		}
		else
		{
			if (++nextPackage == renderBuffer.end())
			{
				hasFallenBackToRenderList = true;

				renderBufferFallbackList.push_back(RenderPackage());
				nextPackageFallbackList = --renderBufferFallbackList.end();

				// Increase fallback buffer by a fixed amount to prevent thrashing
				for (auto i = 0; i < bufferExpandFactor - 1; ++i)
				{
					renderBufferFallbackList.push_back(RenderPackage());
				}
			}
		}

	}

	void Renderer::renderRecursive(RenderPackage* renderPackage)
	{
		if (!renderPackage->obj->visible)
		{
			return;
		}

		renderRecursivePushStacks(renderPackage);
		renderRecursivePushStencilBuffer(renderPackage);
		renderRecursiveDoRender(renderPackage);
		renderRecursivePopStencilBuffer(renderPackage);
		renderRecursivePopStacks(renderPackage);
	}

	void Renderer::renderRecursiveDoRender(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		switch (rend->renderMode)
		{
		case RenderMode::Element:
		case RenderMode::Polygon:
			renderElement(renderPackage);
			renderRecursiveRenderChildren(renderPackage);
			break;

		case RenderMode::CachedElement:
			if (!rend->isClean())
			{
				renderRenderableChildrenIntoElementTexture(renderPackage);
			}

			renderElement(renderPackage);
			break;

		case RenderMode::Primitive:
		case RenderMode::ParticleSystem:
			renderPrimitive(renderPackage);
			renderRecursiveRenderChildren(renderPackage);
			break;

		case RenderMode::Container:
			renderContainer(renderPackage);
			renderRecursiveRenderChildren(renderPackage);
			break;

		case RenderMode::ChildrenOnly:
			renderUpdateRenderableRects(renderPackage);
			renderRecursiveRenderChildren(renderPackage);
			break;
		}
	}

	void Renderer::renderRecursivePushStacks(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		switch (rend->renderPositionMode)
		{
		case RenderPositionMode::Absolute:
		case RenderPositionMode::Positioned:
			renderPositionModeStack.push(rend->renderPositionMode);
		}

		if (rend->renderPositionProcessing != RenderPositionProcessingMode::None)
		{
			renderProcessingStack.push(rend->renderPositionProcessing);
		}

		if (rend->renderTextureMode != RenderTextureMode::LinearNoWrap)
		{
			textureModeStack.push(rend->renderTextureMode);
		}

		if (rend->renderDepthTest != RenderDepthTestMode::Unspecified)
		{
			renderDepthStack.push(rend->renderDepthTest);
		}

		if (rend->renderMultiSampleMode != RenderMultiSampleMode::Unspecified)
		{
			renderMultiSampleModeStack.push(rend->renderMultiSampleMode);
		}

		if (rend->renderColorMode != RenderColorMode::Multiplicative)
		{
			renderColorMode.push(rend->renderColorMode);
		}

		pushColorStack(renderPackage);
	}

	void Renderer::renderRecursivePopStacks(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		const auto color = rend->getColor();
		if (color != nullptr)
		{
			colorStack.pop();
		}

		if (rend->renderColorMode != RenderColorMode::Multiplicative)
		{
			renderColorMode.pop();
		}

		if (rend->renderMultiSampleMode != RenderMultiSampleMode::Unspecified)
		{
			renderMultiSampleModeStack.pop();
		}

		if (rend->renderDepthTest != RenderDepthTestMode::Unspecified)
		{
			renderDepthStack.pop();
		}

		if (rend->renderTextureMode != RenderTextureMode::LinearNoWrap)
		{
			textureModeStack.pop();
		}

		if (rend->renderPositionProcessing != RenderPositionProcessingMode::None)
		{
			renderProcessingStack.pop();
		}

		switch (rend->renderPositionMode)
		{
		case RenderPositionMode::Absolute:
		case RenderPositionMode::Positioned:
			renderPositionModeStack.pop();
		}
	}

	void Renderer::renderRecursivePushStencilBuffer(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		if (rend->getHasClipRect())
		{
			if (renderPackage->stencilDepth == 0)
			{
				glClear(GL_STENCIL_BUFFER_BIT);
				glEnable(GL_STENCIL_TEST);
			}

			renderPackage->stencilDepth++;
		}
	}

	void Renderer::renderRecursivePopStencilBuffer(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		if (rend->getHasClipRect())
		{
			renderPackage->stencilDepth--;

			if (renderPackage->stencilDepth == 0)
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}

	void Renderer::renderUpdateRenderableRects(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		const auto scale = renderPackage->zoom * rend->getScale();
		const auto rect = rend->renderPositionMode == RenderPositionMode::NoPositionSelfOnly
			? (Rect(0.0, 0.0, rend->getRect().w * scale, rend->getRect().h * scale))
			: (rend->getRect() * Rect(renderPackage->zoom, renderPackage->zoom, scale, scale));
		const auto rotation = renderPackage->rotation;

		auto rectMiddleX = (rect.x - (rend->getScreenWidth() / 2.0) * scale) + renderPackage->xOffset;
		auto rectMiddleY = (rect.y - (rend->getScreenHeight() / 2.0) * scale) + renderPackage->yOffset;

		if (renderProcessingStack.top() == RenderPositionProcessingMode::Floor)
		{
			rectMiddleX = std::floor(rectMiddleX);
			rectMiddleY = std::floor(rectMiddleY);
		}

		double originW = renderPackage->computed.w;
		double originH = renderPackage->computed.h;
		renderPackage->computed.w = rect.w;
		renderPackage->computed.h = rect.h;

		if (rend->rotateInParentSpace)
		{
			double rotationRad = renderPackage->rotation * AW::NumberHelper::degToRad;
			double newX = renderPackage->computed.x + rectMiddleX;
			double newY = renderPackage->computed.y + rectMiddleY;
			double oX = renderPackage->computed.x + (originW / 2.0) - (renderPackage->computed.w / 2.0);
			double oY = renderPackage->computed.y + (originH / 2.0) - (renderPackage->computed.h / 2.0);
			double cX = newX - oX;
			double cY = newY - oY;
			double xP = cX * std::cos(rotationRad) - cY * std::sin(rotationRad);
			double yP = cY * std::cos(rotationRad) + cX * std::sin(rotationRad);

			renderPackage->computed.x = (xP + oX);
			renderPackage->computed.y = (yP + oY);
		}
		else
		{
			renderPackage->computed.x += rect.x - rect.w / 2.0;
			renderPackage->computed.y += rect.y - rect.h / 2.0;
		}

		renderPackage->zoom *= rend->getScale();
		renderPackage->rotation += rend->getScreenRotation();
		renderPackage->alpha *= rend->getAlpha();

		renderPackage->world.x = renderPackage->world.x - (originW / 2.0) + rend->getScreenX();
		renderPackage->world.y = renderPackage->world.y - (originH / 2.0) + rend->getScreenY();
		renderPackage->world.w = rend->getScreenWidth();
		renderPackage->world.h = rend->getScreenHeight();

		rend->setWorldRectFromScreenRect(&renderPackage->world);
		rend->setScreenRect(&renderPackage->computed);
	}

	void Renderer::renderRecursiveRenderChildren(RenderPackage* currentRenderPackage)
	{
		const auto& rend = currentRenderPackage->obj;
		if (rend->visible)
		{
			const auto aoPtr = std::dynamic_pointer_cast<GameObject>(rend);
			if (aoPtr != nullptr)
			{
				currentRenderPackage->depth += aoPtr->zIndex;
				for (const auto child : aoPtr->getChildrenOrdered())
				{
					const auto renderableChildPtr = std::dynamic_pointer_cast<Renderable>(child);
					if (renderableChildPtr != nullptr)
					{
						renderRecursive(getNextRenderPackageForObj(renderableChildPtr, currentRenderPackage));
					}
				}
			}
		}
	}

	void Renderer::pushColorStack(RenderPackage* renderPackage)
	{
		const auto& color = renderPackage->obj->getColor();
		if (color != nullptr)
		{
			if (renderColorMode.top() == RenderColorMode::Multiplicative)
			{
				const auto currentColor = colorStack.empty() ? Color(255, 255, 255, 255) : colorStack.top();
				const auto newColor = Color(
					(currentColor.r / 255.0) * color->r,
					(currentColor.g / 255.0) * color->g,
					(currentColor.b / 255.0) * color->b,
					(currentColor.a / 255.0) * color->a
				);
				colorStack.push(newColor);
			}
			else
			{
				colorStack.push(*color);
			}
		}
	}

	void Renderer::setColorModParam(RenderPackage* renderPackage)
	{
		if (!colorStack.empty())
		{
			const auto color = colorStack.top();
			glUniform4f(inColorModLocation, color.r / 255.0, color.g / 255.0, color.b / 255.0, (color.a / 255.0) * renderPackage->alpha);
		}
		else
		{
			glUniform4f(inColorModLocation, 1.0, 1.0, 1.0, 1.0 * renderPackage->alpha);
		}
	}

	void Renderer::renderElement(RenderPackage* renderPackage)
	{
		renderUpdateRenderableRects(renderPackage);

		if (shouldCull(renderPackage)) return;

		if (renderPackage->obj->getHasClipRect())
		{
			updateClipRectOpenGL(renderPackage);
		}

		switch (renderPackage->obj->renderMode)
		{
		case RenderMode::Polygon:
			renderPolygonOpenGL(renderPackage);
			break;

		default:
			renderElementOpenGL(renderPackage);
			break;
		}
	}

	void Renderer::renderPrimitive(RenderPackage* renderPackage)
	{
		auto prim = std::dynamic_pointer_cast<Primitive>(renderPackage->obj);
		if (prim == nullptr)
		{
			return;
		}

		renderUpdateRenderableRects(renderPackage);

		if (shouldCull(renderPackage)) return;

		prim->preUpdateRender(&renderPackage->computed, renderPackage);
		prim->preRender(&renderPackage->computed, renderPackage);

		if (prim->getHasClipRect())
		{
			updateClipRectOpenGL(renderPackage);
		}

		switch (prim->renderMode)
		{
		case RenderMode::ParticleSystem:
			renderParticleSystemOpenGL(renderPackage);
			break;

		default:
			renderPrimitiveOpenGL(renderPackage);
			break;
		}
	}

	void Renderer::renderContainer(RenderPackage* renderPackage)
	{
		const auto container = std::dynamic_pointer_cast<Container>(renderPackage->obj);
		if (container == nullptr)
		{
			return;
		}

		container->performAutoLayoutIfNeeded();

		renderUpdateRenderableRects(renderPackage);

		if (container->getHasClipRect())
		{
			updateClipRectOpenGL(renderPackage);
		}
	}

	void Renderer::renderRenderableChildrenIntoElementTexture(RenderPackage* renderPackage)
	{
		const auto cached = std::dynamic_pointer_cast<DisplayBuffer>(renderPackage->obj);
		if (cached == nullptr)
		{
			return;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		unsigned int backRenderBuffer;
		glGenFramebuffers(1, &backRenderBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, backRenderBuffer);

		glBindRenderbuffer(GL_RENDERBUFFER, backgroundRenderBuffer);
		GLDbgCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, backgroundRenderBuffer));

		frameBufferStack.push(std::make_tuple(cached->getScreenWidth(), cached->getScreenHeight(), backRenderBuffer, backgroundRenderBuffer));

		unsigned int texColorBuffer;
		if (!cached->hasTexture())
		{
			glGenTextures(1, &texColorBuffer);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cached->getScreenWidth(), cached->getScreenHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			// TODO: If MSAA is needed
			// glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, currentScreenConfig.msaaSamples, GL_RGB, cached->getHeight(), cached->getWidth(), false);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBindTexture(GL_TEXTURE_2D, 0);

			const auto cachedTexture = std::make_shared<Texture>(texColorBuffer, cached->getScreenWidth(), cached->getScreenHeight());
			cached->setTexture(cachedTexture);
		}
		else
		{
			texColorBuffer = cached->getTexture()->openGlTextureId();
		}

		GLDbgCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0));

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::instance()->logCritical("Renderer::Failed to bind back frame buffer for DisplayBuffer");
			return;
		}

		LM::mat4x4_ortho(pBackground, 0, cached->getScreenWidth(), cached->getScreenHeight(), 0, -(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);
		setViewport(cached->getScreenWidth(), cached->getScreenHeight());

		const auto cColor = cached->getClearColor();
		if (cColor != nullptr && cColor->a > 0)
		{
			glClearColor(cColor->r / 255.0, cColor->g / 255.0, cColor->b / 255.0, cColor->a / 255.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		renderTargetStack.push(RenderTargetMode::Background);

		auto childRenderPackage = getNextRenderPackageForObj(renderPackage->obj);

		childRenderPackage->alpha = renderPackage->alpha * cached->getAlpha();
		childRenderPackage->depth = renderPackage->depth;
		childRenderPackage->stencilDepth = renderPackage->stencilDepth;

		renderRecursiveRenderChildren(childRenderPackage);

		renderTargetStack.pop();

		glDeleteFramebuffers(1, &backRenderBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		frameBufferStack.pop();

		if (frameBufferStack.empty())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			setViewport(currentScreenConfig.width, currentScreenConfig.height);
		}
		else
		{
			const auto previous = frameBufferStack.top();
			glBindFramebuffer(GL_FRAMEBUFFER, std::get<2>(previous));
			glBindRenderbuffer(GL_RENDERBUFFER, std::get<3>(previous));
			setViewport(std::get<0>(previous), std::get<1>(previous));
			LM::mat4x4_ortho(pBackground, 0, std::get<0>(previous), std::get<1>(previous), 0, -(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);
		}
	}

	void Renderer::renderElementOpenGL(RenderPackage* renderPackage)
	{
		const auto ele = std::dynamic_pointer_cast<Element>(renderPackage->obj);

		changeProgram(renderPackage);

		const auto computed = &renderPackage->computed;
		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		LM::mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			LM::mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		LM::mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		LM::mat4x4_translate(t, cX, cY, cY + (ele->zIndex + renderPackage->depth) * layerFactor);

		LM::mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute || ele->renderPositionMode == RenderPositionMode::AbsoluteSelfOnly)
		{
			LM::mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTargetMode::Background)
		{
			LM::mat4x4_dup(tP, pBackground);
		}
		else
		{
			LM::mat4x4_dup(tP, p);
		}

		LM::mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const float*)mvp);

		setColorModParam(renderPackage);

		const auto texture = ele->getTexture();
		bindGLTexture(texture == nullptr ? defaultWhiteTexture : texture->openGlTextureId());

		openGLDrawTriangles(renderPackage);

		ele->markClean();
	}

	void Renderer::updateClipRectOpenGL(RenderPackage* renderPackage)
	{
		const auto& rend = renderPackage->obj;
		const auto clipRect = rend->getClipRect();

		const auto computed = &renderPackage->computed;
		const auto cW = clipRect->w / 2.0;
		const auto cH = clipRect->h / 2.0;
		const auto cX = computed->x + clipRect->x + computed->w / 2.0;
		const auto cY = computed->y + clipRect->y + computed->h / 2.0;

		LM::mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			LM::mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		LM::mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		LM::mat4x4_translate(t, cX, cY, cY + (20 + renderPackage->depth) * layerFactor);

		LM::mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			LM::mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTargetMode::Background)
		{
			LM::mat4x4_dup(tP, pBackground);
		}
		else
		{
			LM::mat4x4_dup(tP, p);
		}

		LM::mat4x4_mul(mvp, tP, m);

		auto vertexShader = rend->getClipRectVertexShader();
		if (vertexShader == nullptr)
		{
			vertexShader = defaultVertexShader;
			vertexShader->setCachedProgramId(rend->cachedClipRectProgram);
		}

		auto fragmentShader = rend->getClipRectFragmentShader();
		if (fragmentShader == nullptr)
		{
			fragmentShader = defaultFragmentShader;
		}

		changeProgram(vertexShader, fragmentShader);

		rend->cachedClipRectProgram = vertexShader->getCachedProgramId();

		applyShaderUniforms(vertexShader, renderPackage);
		applyShaderUniforms(fragmentShader, renderPackage);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const float*)mvp);
		glUniform4f(inColorModLocation, 1.0, 1.0, 1.0, 1.0);

		openGLDrawArraysStencil(renderPackage);
	}

	void Renderer::bindGLTexture(unsigned int textureId)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (textureId == defaultWhiteTexture) return;

		switch (textureModeStack.top())
		{
		case RenderTextureMode::BilinearNoWrap:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;

		case RenderTextureMode::LinearWrapping:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;

		case RenderTextureMode::BilinearWrapping:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;

		case RenderTextureMode::LinearNoWrap:
		default:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		}
	}

	void Renderer::renderPrimitiveOpenGL(RenderPackage* renderPackage)
	{
		const auto prim = std::dynamic_pointer_cast<Primitive>(renderPackage->obj);
		changeProgram(renderPackage);

		const auto computed = &renderPackage->computed;
		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		LM::mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			LM::mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		LM::mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		LM::mat4x4_translate(t, cX, cY, cY + (prim->zIndex + renderPackage->depth) * layerFactor);

		LM::mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			LM::mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTargetMode::Background)
		{
			LM::mat4x4_dup(tP, pBackground);
		}
		else
		{
			LM::mat4x4_dup(tP, p);
		}

		LM::mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const float*)mvp);

		setColorModParam(renderPackage);

		openGLDrawTriangles(renderPackage);

		prim->markClean();
	}

	void Renderer::renderPolygonOpenGL(RenderPackage* renderPackage)
	{
		const auto poly = std::dynamic_pointer_cast<Polygon>(renderPackage->obj);
		if (poly == nullptr) return;

		changeProgram(renderPackage);

		const auto computed = &renderPackage->computed;
		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		LM::mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			LM::mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		LM::mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		LM::mat4x4_translate(t, cX, cY, cY + (poly->zIndex + renderPackage->depth) * layerFactor);

		LM::mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			LM::mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTargetMode::Background)
		{
			LM::mat4x4_dup(tP, pBackground);
		}
		else
		{
			LM::mat4x4_dup(tP, p);
		}

		LM::mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const float*)mvp);

		setColorModParam(renderPackage);

		if (poly->vertexBuffer == nullptr)
		{
			const auto normalizedPoints = poly->getRenderPoints();
			poly->vertexBuffer = std::make_unique<VertexBufferProxy>(generateVertexBuffer(normalizedPoints), (unsigned int)normalizedPoints.size());
		}

		const auto vertexBufferId = poly->vertexBuffer->id;
		const auto vertexBufferSize = poly->vertexBuffer->size;

		const auto texture = poly->getTexture();
		bindGLTexture(texture == nullptr ? defaultWhiteTexture : texture->openGlTextureId());

		openGLDrawPoints(renderPackage, vertexBufferId, vertexBufferSize, poly->getFilled());

		poly->markClean();
	}

	void Renderer::renderParticleSystemOpenGL(RenderPackage* renderPackage)
	{
		const auto prim = std::dynamic_pointer_cast<Primitive>(renderPackage->obj);
		const auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(prim);
		if (particleSystem == nullptr) return;

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			LM::mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTargetMode::Background)
		{
			LM::mat4x4_dup(tP, pBackground);
		}
		else
		{
			LM::mat4x4_dup(tP, p);
		}

		const auto computed = &renderPackage->computed;
		for (const auto& particle : particleSystem->getParticles())
		{
			const auto particleTexture = particle->getTexture();
			const auto glTextureId = particleTexture != nullptr ? particleTexture->openGlTextureId() : defaultWhiteTexture;
			const auto cW = particle->w / 2.0;
			const auto cH = particle->h / 2.0;
			const auto cX = computed->x + particle->x + cW;
			const auto cY = computed->y + particle->y + cH;
			const auto rotation = (renderPackage->rotation + particle->r) * AW::NumberHelper::degToRad;

			LM::mat4x4_identity(m);

			if (rotation != 0.0)
			{
				LM::mat4x4_rotate_Z(m, m, rotation);
			}

			LM::mat4x4_scale_aniso(m, m, cW, cH, 1.0);
			LM::mat4x4_translate(t, cX, cY, cY + (particleSystem->zIndex + particle->zIndex + renderPackage->depth) * layerFactor);
			LM::mat4x4_mul(m, t, m);
			LM::mat4x4_mul(mvp, tP, m);

			const auto clipRect = particle->clip;
			double scaleX, scaleY, xOffset, yOffset;
			if (glTextureId != defaultWhiteTexture)
			{
				scaleX = clipRect.w / particle->getTexture()->getWidth();
				scaleY = clipRect.h / particle->getTexture()->getHeight();
				xOffset = clipRect.x / particle->getTexture()->getWidth();
				yOffset = clipRect.y / particle->getTexture()->getHeight();
			}
			else
			{
				scaleX = clipRect.w / particle->w;
				scaleY = clipRect.h / particle->h;
				xOffset = clipRect.x / particle->w;
				yOffset = clipRect.y / particle->h;
			}

			const auto pVShader = particle->getVertexShader();
			const auto pFShader = particle->getFragmentShader();

			if (pVShader != nullptr && pFShader != nullptr)
			{
				changeProgram(pVShader, pFShader);
				applyShaderUniforms(pVShader, renderPackage);
				applyShaderUniforms(pFShader, renderPackage);
			}
			else
			{
				changeProgram(renderPackage);
			}

			glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const float*)mvp);
			glUniform4f(inColorModLocation, particle->cModR / 255.0, particle->cModG / 255.0, particle->cModB / 255.0, (particle->alphaMod / 255.0) * renderPackage->alpha);

			bindGLTexture(glTextureId);

			openGLDrawTriangles(renderPackage);
		}
	}
}
