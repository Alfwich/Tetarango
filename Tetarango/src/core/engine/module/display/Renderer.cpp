#include "Renderer.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/trace/Trace.h"
#include "ui/renderable/primitive/Rectangle.h"
#include "ui/renderable/element/DisplayBuffer.h"

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
}

namespace AW
{
	Renderer::Renderer(const ScreenConfig& screenConfig, std::shared_ptr<Renderer> oldRenderer)
	{
		renderPositionModeStack.push(RenderPositionMode::Positioned);
		renderProcessingStack.push(RenderPositionProcessing::None);
		textureModeStack.push(RenderTextureMode::LinearNoWrap);
		renderDepthStack.push(RenderDepthTest::Disabled);
		renderMultiSampleModeStack.push(RenderMultiSampleMode::Disabled);
		renderTargetStack.push(RenderTarget::Screen);
		renderColorMode.push(RenderColorMode::Multiplicative);

		if (oldRenderer != nullptr)
		{
			harvestFromPreviousRenderer(oldRenderer);
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
			Logger::instance()->logFatal("Renderer::GLEW failed to init: " + std::string((char*)glewGetErrorString(glewError)));
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
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
		}

		if (vertexBuffer == 0)
		{
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(inlineVerticies), inlineVerticies, GL_STATIC_DRAW);
		}

		if (textureUVBuffer == 0)
		{
			glGenBuffers(1, &textureUVBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, textureUVBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(inlineUVCoords), inlineUVCoords, GL_STATIC_DRAW);
		}

		if (currentScreenConfig.openGlWireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		generateBackgroundRenderBuffer();

		layerFactor = (1 << 16) / maxLayers;

		return true;
	}

	void Renderer::setDefaultShaders(std::shared_ptr<ShaderReference> vertexShader, std::shared_ptr<ShaderReference> fragmentShader)
	{
		defaultVertexShader = vertexShader;
		defaultFragmentShader = fragmentShader;
	}

	void Renderer::harvestFromPreviousRenderer(std::shared_ptr<Renderer> previous)
	{
		clearColor = previous->clearColor;
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

		if (vertexBuffer != 0)
		{
			glDeleteBuffers(1, &vertexBuffer);
			vertexBuffer = 0;
		}

		if (textureUVBuffer != 0)
		{
			glDeleteBuffers(1, &textureUVBuffer);
			textureUVBuffer = 0;
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
		prepareRender(screen, frameTimestamp);

		Rect rootRect;
		RenderPackage renderPackage;

		renderOpenGL(root, rootRect, screen, &renderPackage);
	}

	void Renderer::renderOpenGL(std::shared_ptr<Renderable> root, Rect rootRect, Screen* screen, RenderPackage* renderPackage)
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

		double lowerDepthLayer = -(maxLayers / 2) * layerFactor;
		double upperDepthLayer = (maxLayers / 2) * layerFactor;
		mat4x4_ortho(p, 0.0, width / renderPackage->zoom, height / renderPackage->zoom, 0.0, lowerDepthLayer, upperDepthLayer);
		mat4x4_ortho(pAbs, 0, width, height, 0.0, lowerDepthLayer, upperDepthLayer);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textureUVBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		RenderPackage rootPackage = *renderPackage;
		rootPackage.computed = rootRect;
		renderRecursive(root, *renderPackage);

		SDL_GL_SwapWindow(screen->getWindow());

		glDisable(GL_BLEND);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);
		currentProgramId = 0;
	}

	void Renderer::openGLDrawArrays(RenderPackage* renderPackage)
	{
		if (renderPackage->stencilDepth > 0)
		{
			glStencilFunc(GL_EQUAL, 1, 0xFF);
		}

		if (!depthEnabled && renderDepthStack.top() == RenderDepthTest::Enabled)
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
		else if (msaaEnabled)
		{
			glDisable(GL_MULTISAMPLE);
			msaaEnabled = false;
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
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

		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}


	GLuint Renderer::getUniformLocationForCurrentProgram(const std::string& paramName, GLuint programId)
	{
		if (programIdToProgramUniformMapId.count(programId) == 0)
		{
			programIdToProgramUniformMapId[programId] = std::unordered_map<std::string, GLuint>();
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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, currentScreenConfig.width, currentScreenConfig.height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Renderer::changeProgram(const std::shared_ptr<Renderable>& renderable, const RenderPackage* renderPackage)
	{
		auto vertexShader = renderable->getVertexShader();
		if (vertexShader == nullptr)
		{
			vertexShader = defaultVertexShader;
			vertexShader->setCachedProgramId(renderable->cachedProgramId);
		}

		auto fragmentShader = renderable->getFragmentShader();
		if (fragmentShader == nullptr)
		{
			fragmentShader = defaultFragmentShader;
		}

		changeProgram(vertexShader, fragmentShader);

		renderable->cachedProgramId = vertexShader->getCachedProgramId();

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

	void Renderer::changeProgram(GLuint programId)
	{
		if (currentProgramId != programId)
		{
			glUseProgram(programId);
			currentProgramId = programId;

			inMatrixLocation = getUniformLocationForCurrentProgram("mvp", programId);
			inColorModLocation = getUniformLocationForCurrentProgram("cMod", programId);
		}
	}

	std::string Renderer::getKeyForShaders(const std::vector<GLuint> vertexShaderIds, const std::vector<GLuint> fragmentShaderIds)
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

	GLuint Renderer::createAndLinkProgramIfNeeded(const std::vector<GLuint> vertexShaderIds, const std::vector<GLuint> fragmentShaderIds, GLuint loaderShaderId)
	{
		const auto key = getKeyForShaders(vertexShaderIds, fragmentShaderIds);
		if (programs.count(key) == 0)
		{
			const auto programId = glCreateProgram();
			glAttachShader(programId, vertexShaderIds[0]);
			for (const auto fragmentShaderId : fragmentShaderIds)
			{
				glAttachShader(programId, fragmentShaderId);
			}
			glAttachShader(programId, loaderShaderId);
			glLinkProgram(programId);

			GLint programLinkSuccess = GL_TRUE;
			glGetProgramiv(programId, GL_LINK_STATUS, &programLinkSuccess);

			if (programLinkSuccess != GL_TRUE)
			{
				char logBuf[1024];
				int len;
				glGetProgramInfoLog(programId, sizeof(logBuf), &len, logBuf);
				reportOpenGLErrors();
				Logger::instance()->logFatal("Renderer::OpenGL::Failed to link program: " + std::string(logBuf, len));
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
			Logger::instance()->logCritical("OpenGL::Error reported: " + std::to_string(err));
		}
	}

	void Renderer::updateScreenConfig(const ScreenConfig & config)
	{
		currentScreenConfig = config;

		if (currentScreenConfig.openGlWireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		generateBackgroundRenderBuffer();
	}

	void Renderer::prepareRender(Screen* screen, double frameTimestamp)
	{
		currentFrameTimestamp = frameTimestamp;
		screenWidth = screen->getWidth();
		screenHeight = screen->getHeight();
		setViewport(screenWidth, screenHeight);
	}

	void Renderer::renderRecursive(std::shared_ptr<Renderable> rend, RenderPackage renderPackage)
	{
		renderRecursivePushStacks(rend);
		renderRecursivePushStencilBuffer(rend, &renderPackage);
		renderRecursiveDoRender(rend, &renderPackage);
		renderRecursivePopStencilBuffer(rend, &renderPackage);
		renderRecursivePopStacks(rend);
	}

	void Renderer::renderRecursiveDoRender(const std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		switch (rend->renderMode)
		{
		case RenderMode::Element:
			renderElement(rend, renderPackage);
			renderRecursiveRenderChildren(rend, renderPackage);
			break;

		case RenderMode::CachedElement:
			if (!rend->isClean())
			{
				renderElementChildrenIntoElementTexture(rend, renderPackage);
			}

			renderElement(rend, renderPackage);
			break;

		case RenderMode::Primitive:
		case RenderMode::ParticleSystem:
			renderPrimitive(rend, renderPackage);
			renderRecursiveRenderChildren(rend, renderPackage);
			break;

		case RenderMode::Container:
			renderContainer(rend, renderPackage);
			renderRecursiveRenderChildren(rend, renderPackage);
			break;

		case RenderMode::ChildrenOnly:
			renderUpdateRenderableRects(rend, renderPackage);
			renderRecursiveRenderChildren(rend, renderPackage);
			break;
		}
	}

	void Renderer::renderRecursivePushStacks(const std::shared_ptr<Renderable>& rend)
	{
		switch (rend->renderPositionMode)
		{
		case RenderPositionMode::Absolute:
		case RenderPositionMode::Positioned:
			renderPositionModeStack.push(rend->renderPositionMode);
		}

		if (rend->renderPositionProcessing != RenderPositionProcessing::None)
		{
			renderProcessingStack.push(rend->renderPositionProcessing);
		}

		if (rend->renderTextureMode != RenderTextureMode::LinearNoWrap)
		{
			textureModeStack.push(rend->renderTextureMode);
		}

		if (rend->renderDepthTest != RenderDepthTest::Unspecified)
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

		const auto color = rend->getColor();
		if (color != nullptr)
		{
			pushColorStack(color);
		}
	}

	void Renderer::renderRecursivePopStacks(const std::shared_ptr<Renderable>& rend)
	{
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

		if (rend->renderDepthTest != RenderDepthTest::Unspecified)
		{
			renderDepthStack.pop();
		}

		if (rend->renderTextureMode != RenderTextureMode::LinearNoWrap)
		{
			textureModeStack.pop();
		}

		if (rend->renderPositionProcessing != RenderPositionProcessing::None)
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

	void Renderer::renderRecursivePushStencilBuffer(const std::shared_ptr<Renderable>& rend, RenderPackage* renderPackage)
	{
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

	void Renderer::renderRecursivePopStencilBuffer(const std::shared_ptr<Renderable>& rend, RenderPackage* renderPackage)
	{
		if (rend->getHasClipRect())
		{
			renderPackage->stencilDepth--;

			if (renderPackage->stencilDepth == 0)
			{
				glDisable(GL_STENCIL_TEST);
			}
		}
	}

	void Renderer::renderUpdateRenderableRects(std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		const auto scale = renderPackage->zoom * rend->getScale();
		const auto rect = rend->getRect() * Rect(renderPackage->zoom, renderPackage->zoom, scale, scale);
		const auto rotation = renderPackage->rotation;

		auto rectMiddleX = (rect.x - (rend->getWidth() / 2.0) * scale) + renderPackage->xOffset;
		auto rectMiddleY = (rect.y - (rend->getHeight() / 2.0) * scale) + renderPackage->yOffset;

		if (renderProcessingStack.top() == RenderPositionProcessing::Floor)
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
		renderPackage->rotation += rend->getRotation();
		renderPackage->alpha *= rend->getAlpha();

		rend->setWorldRect(&renderPackage->computed);
		rend->setScreenRect(&renderPackage->computed);
	}

	void Renderer::renderRecursiveRenderChildren(const std::shared_ptr<Renderable>& rend, RenderPackage* renderPackage)
	{
		if (rend->visible)
		{
			const auto aoPtr = std::dynamic_pointer_cast<GameObject>(rend);
			if (aoPtr != nullptr)
			{
				renderPackage->depth += aoPtr->zIndex;
				for (const auto child : aoPtr->getChildrenOrdered())
				{
					const auto renderableChildPtr = std::dynamic_pointer_cast<Renderable>(child);
					if (renderableChildPtr != nullptr)
					{
						renderRecursive(renderableChildPtr, *renderPackage);
					}
				}
			}
		}
	}

	void Renderer::pushColorStack(const Color& color)
	{
		if (renderColorMode.top() == RenderColorMode::Multiplicative)
		{
			const auto currentColor = colorStack.empty() ? Color(255, 255, 255, 255) : colorStack.top();
			const auto newColor = Color(
				(currentColor.r / 255.0) * color.r,
				(currentColor.g / 255.0) * color.g,
				(currentColor.b / 255.0) * color.b,
				(currentColor.a / 255.0) * color.a
			);
			colorStack.push(newColor);
		}
		else
		{
			colorStack.push(color);
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

	void Renderer::renderElement(std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		auto ele = std::dynamic_pointer_cast<Element>(rend);
		if (ele == nullptr)
		{
			return;
		}

		renderUpdateRenderableRects(ele, renderPackage);

		if (ele->getHasClipRect())
		{
			updateClipRectOpenGL(ele, renderPackage);
		}

		renderElementOpenGL(ele, renderPackage);
	}

	void Renderer::renderPrimitive(std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		auto prim = std::dynamic_pointer_cast<Primitive>(rend);
		if (prim == nullptr)
		{
			return;
		}

		renderUpdateRenderableRects(prim, renderPackage);
		prim->preUpdateRender(&renderPackage->computed, renderPackage);
		prim->preRender(&renderPackage->computed, renderPackage);

		if (prim->getHasClipRect())
		{
			updateClipRectOpenGL(prim, renderPackage);
		}

		switch (prim->renderMode)
		{
		case RenderMode::ParticleSystem:
			renderParticleSystemOpenGL(prim, renderPackage);
			break;

		default:
			renderPrimitiveOpenGL(prim, renderPackage);
			break;
		}
	}

	void Renderer::renderContainer(std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		const auto container = std::dynamic_pointer_cast<Container>(rend);
		if (container == nullptr)
		{
			return;
		}

		container->performAutoLayoutIfNeeded();

		renderUpdateRenderableRects(container, renderPackage);

		if (container->getHasClipRect())
		{
			updateClipRectOpenGL(container, renderPackage);
		}
	}

	void Renderer::renderElementChildrenIntoElementTexture(std::shared_ptr<Renderable> rend, const RenderPackage* renderPackage)
	{
		const auto cached = std::dynamic_pointer_cast<DisplayBuffer>(rend);
		if (cached == nullptr)
		{
			return;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		GLuint backRenderBuffer;
		glGenFramebuffers(1, &backRenderBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, backRenderBuffer);

		glBindRenderbuffer(GL_RENDERBUFFER, backgroundRenderBuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, backgroundRenderBuffer);

		frameBufferStack.push(std::make_tuple(cached->getWidth(), cached->getHeight(), backRenderBuffer, backgroundRenderBuffer));

		GLuint texColorBuffer;
		if (!cached->hasTexture())
		{
			glGenTextures(1, &texColorBuffer);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cached->getWidth(), cached->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			// TODO: Consider if needed
			//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, currentScreenConfig.msaaSamples, GL_RGBA, cached->getHeight(), cached->getWidth(), false);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			const auto cachedTexture = std::make_shared<Texture>(texColorBuffer, cached->getWidth(), cached->getHeight());
			cached->setTexture(cachedTexture);
		}
		else
		{
			texColorBuffer = cached->getTexture()->openGlTextureId();
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			Logger::instance()->logCritical("Renderer::Failed to bind back frame buffer for DisplayBuffer");
			return;
		}

		mat4x4_ortho(pBackground, 0, cached->getWidth(), cached->getHeight(), 0, -(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);
		setViewport(cached->getWidth(), cached->getHeight());

		const auto cColor = cached->getClearColor();
		if (cColor != nullptr && cColor->a > 0)
		{
			glClearColor(cColor->r / 255.0, cColor->g / 255.0, cColor->b / 255.0, cColor->a / 255.0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		renderTargetStack.push(RenderTarget::Background);

		RenderPackage childRenderPackage;

		childRenderPackage.alpha = renderPackage->alpha * cached->getAlpha();
		childRenderPackage.depth = renderPackage->depth;
		childRenderPackage.stencilDepth = renderPackage->stencilDepth;

		renderRecursiveRenderChildren(cached, &childRenderPackage);

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
			mat4x4_ortho(pBackground, 0, std::get<0>(previous), std::get<1>(previous), 0, -(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);
		}
	}

	void Renderer::renderElementOpenGL(std::shared_ptr<Element> ele, RenderPackage* renderPackage)
	{
		const auto texture = ele->getTexture();
		if (texture == nullptr)
		{
			return;
		}

		changeProgram(ele, renderPackage);

		const auto computed = &renderPackage->computed;
		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		mat4x4_translate(t, cX, cY, cY + (ele->zIndex + renderPackage->depth) * layerFactor);

		mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute || ele->renderPositionMode == RenderPositionMode::AbsoluteSelfOnly)
		{
			mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTarget::Background)
		{
			mat4x4_dup(tP, pBackground);
		}
		else
		{
			mat4x4_dup(tP, p);
		}

		mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

		setColorModParam(renderPackage);

		bindGLTexture(texture->openGlTextureId());

		openGLDrawArrays(renderPackage);

		ele->markClean();
	}

	void Renderer::updateClipRectOpenGL(std::shared_ptr<Renderable> rend, RenderPackage* renderPackage)
	{
		const auto clipRect = rend->getClipRect();

		const auto computed = &renderPackage->computed;
		const auto cW = clipRect->w / 2.0;
		const auto cH = clipRect->h / 2.0;
		const auto cX = computed->x + clipRect->x + computed->w / 2.0;
		const auto cY = computed->y + clipRect->y + computed->h / 2.0;

		mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		mat4x4_translate(t, cX, cY, cY + (20 + renderPackage->depth) * layerFactor);

		mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTarget::Background)
		{
			mat4x4_dup(tP, pBackground);
		}
		else
		{
			mat4x4_dup(tP, p);
		}

		mat4x4_mul(mvp, tP, m);

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

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);
		glUniform4f(inColorModLocation, 1.0, 1.0, 1.0, 1.0);

		openGLDrawArraysStencil(renderPackage);
	}

	void Renderer::bindGLTexture(GLuint textureId)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);

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

	void Renderer::renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, RenderPackage* renderPackage)
	{
		changeProgram(prim, renderPackage);

		const auto computed = &renderPackage->computed;
		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		mat4x4_identity(m);

		if (renderPackage->rotation != 0.0)
		{
			mat4x4_rotate_Z(m, m, renderPackage->rotation * AW::NumberHelper::degToRad);
		}
		mat4x4_scale_aniso(m, m, cW, cH, 1.0);
		mat4x4_translate(t, cX, cY, cY + (prim->zIndex + renderPackage->depth) * layerFactor);

		mat4x4_mul(m, t, m);

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTarget::Background)
		{
			mat4x4_dup(tP, pBackground);
		}
		else
		{
			mat4x4_dup(tP, p);
		}

		mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

		setColorModParam(renderPackage);

		openGLDrawArrays(renderPackage);

		prim->markClean();
	}

	void Renderer::renderParticleSystemOpenGL(std::shared_ptr<Primitive> prim, RenderPackage* renderPackage)
	{
		const auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(prim);
		if (particleSystem == nullptr)
		{
			return;
		}

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			mat4x4_dup(tP, pAbs);
		}
		else if (renderTargetStack.top() == RenderTarget::Background)
		{
			mat4x4_dup(tP, pBackground);
		}
		else
		{
			mat4x4_dup(tP, p);
		}

		const auto computed = &renderPackage->computed;
		for (const auto& particle : particleSystem->getParticles())
		{
			const auto particleTexture = particle->getTexture();
			const auto glTextureId = particleTexture != nullptr ? particleTexture->openGlTextureId() : 0;
			const auto cW = particle->w / 2.0;
			const auto cH = particle->h / 2.0;
			const auto cX = computed->x + particle->x + cW;
			const auto cY = computed->y + particle->y + cH;
			const auto rotation = (renderPackage->rotation + particle->r) * AW::NumberHelper::degToRad;

			mat4x4_identity(m);

			if (rotation != 0.0)
			{
				mat4x4_rotate_Z(m, m, rotation);
			}
			mat4x4_scale_aniso(m, m, cW, cH, 1.0);
			mat4x4_translate(t, cX, cY, cY + (particleSystem->zIndex + particle->zIndex + renderPackage->depth) * layerFactor);
			mat4x4_mul(m, t, m);
			mat4x4_mul(mvp, tP, m);

			const auto clipRect = particle->clip;
			double scaleX, scaleY, xOffset, yOffset;
			if (glTextureId != 0)
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
				changeProgram(prim, renderPackage);
			}

			glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);
			glUniform4f(inColorModLocation, particle->cModR / 255.0, particle->cModG / 255.0, particle->cModB / 255.0, (particle->alphaMod / 255.0) * renderPackage->alpha);

			bindGLTexture(glTextureId);

			openGLDrawArrays(renderPackage);
		}
	}
}
