#include "Renderer.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/trace/Trace.h"
#include "ui/renderable/primitive/Rectangle.h"

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


	void Renderer::initOpenGL(SDL_Window* window)
	{
		glContext = SDL_GL_CreateContext(window);
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			Logger::instance()->logFatal("Renderer::GLEW::Failed to init: " + std::string((char*)glewGetErrorString(glewError)));
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

		layerFactor = (1 << 16) / maxLayers;
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
	}

	void Renderer::setClearColor(int r, int g, int b, int a)
	{
		clearColor.r = r;
		clearColor.g = g;
		clearColor.b = b;
		clearColor.a = a;
	}

	void Renderer::render(std::shared_ptr<Renderable> root, Screen* screen, double frameTimestamp)
	{
		prepareRender(screen, frameTimestamp);

		Rect rootRect;
		RenderPackage renderPackage;
		renderPackage.zoom = camera == nullptr ? 1.0 : camera->getZoom();
		renderPackage.cameraX = camera == nullptr ? 0.0 : camera->getX() - screenWidth / 2;
		renderPackage.cameraY = camera == nullptr ? 0.0 : camera->getY() - screenHeight / 2;
		renderPackage.xOffset = camera == nullptr ? 0.0 : camera->getXOffset();
		renderPackage.yOffset = camera == nullptr ? 0.0 : camera->getYOffset();

		renderOpenGL(root, rootRect, screen, &renderPackage);
	}

	void Renderer::renderOpenGL(std::shared_ptr<Renderable> root, Rect rootRect, Screen* screen, RenderPackage* renderPackage)
	{
		glClearColor(clearColor.r / 255.0, clearColor.g / 255.0, clearColor.b / 255.0, clearColor.a / 255.0);

		if (clearColor.a != 0)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		const auto width = screenWidth, height = screenHeight;

		renderPackage->xOffset = renderPackage->cameraX - (width / renderPackage->zoom - width) / 2.0;
		renderPackage->yOffset = renderPackage->cameraY - (height / renderPackage->zoom - height) / 2.0;

		mat4x4_ortho(p,
			renderPackage->xOffset,
			width / renderPackage->zoom + renderPackage->xOffset,
			height / renderPackage->zoom + renderPackage->yOffset,
			renderPackage->yOffset,
			-(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);

		mat4x4_ortho(pAbs, 0, width, height, 0, -(maxLayers / 2) * layerFactor, (maxLayers / 2) * layerFactor);


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textureUVBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderRecursive(root, rootRect, *renderPackage);

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

	void Renderer::changeProgram(const std::shared_ptr<Renderable>& renderable)
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

		applyShaderUniforms(vertexShader);
		applyShaderUniforms(fragmentShader);
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

	void Renderer::applyShaderUniforms(const std::shared_ptr<ShaderReference>& shader)
	{
		if (shader == nullptr || !shader->hasCustomParams())
		{
			return;
		}

		if (shader->hasCachedCustomParams())
		{
			for (const auto cachedParam : shader->getCachedFloatIUParams())
			{
				if (cachedParam.first != -1)
				{
					glUniform1f(cachedParam.first, cachedParam.second);
				}
			}

			if (shader->hasFrameTimeParam())
			{
				const auto position = getUniformLocationForCurrentProgram("frameTime", currentProgramId);

				if (position != -1)
				{
					glUniform1f(position, currentFrameTimestamp);
				}
			}
		}
		else
		{
			for (const auto paramNameToValue : shader->getFloatIUParams())
			{
				const auto position = getUniformLocationForCurrentProgram(paramNameToValue.first, currentProgramId);

				if (position != -1)
				{
					if (paramNameToValue.first == "frameTime" && paramNameToValue.second == -1.f)
					{
						glUniform1f(position, currentFrameTimestamp);
					}
					else
					{
						glUniform1f(position, paramNameToValue.second);
					}
				}

				shader->setCachedParam(position, paramNameToValue.second);
			}
		}
	}

	SDL_GLContext Renderer::getOpenGLContext()
	{
		return glContext;
	}

	void Renderer::reportOpenGLErrors()
	{
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			Logger::instance()->logCritical("OpenGL::Error reported: " + std::to_string(err));
		}
	}

	bool Renderer::isOpenGLEnabled()
	{
		return glContext != NULL;
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
	}

	void Renderer::prepareRender(Screen* screen, double frameTimestamp)
	{
		currentFrameTimestamp = frameTimestamp;
		screenWidth = screen->getWidth();
		screenHeight = screen->getHeight();
		camera = screen->getCamera();
	}

	void Renderer::renderRecursive(std::shared_ptr<Renderable> rend, Rect computed, RenderPackage renderPackage)
	{
		switch (rend->renderPositionMode)
		{
		case RenderPositionMode::Absolute:
		case RenderPositionMode::Positioned:
			renderPositionModeStack.push(rend->renderPositionMode);
		}

		if (rend->getHasClipRect())
		{
			if (renderPackage.stencilDepth == 0)
			{
				glClear(GL_STENCIL_BUFFER_BIT);
				glEnable(GL_STENCIL_TEST);
			}

			renderPackage.stencilDepth++;
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

		bool pushedColorStack = false;
		std::shared_ptr<Rectangle> debugObject = nullptr;
		switch (rend->renderType)
		{
		case RenderType::Element:
		case RenderType::Backdrop:
		case RenderType::TileMap:
		{
			const auto ele = std::static_pointer_cast<Element>(rend);

			pushedColorStack = updateColorStack(ele);
			renderElement(ele, &computed, &renderPackage);
		}
		break;

		case RenderType::Primitive:
		case RenderType::ParticleSystem:
		{
			const auto prim = std::static_pointer_cast<Primitive>(rend);

			pushedColorStack = updateColorStack(prim);
			renderPrimitive(prim, &computed, &renderPackage);
		}
		break;

		case RenderType::Container:
		{
			const auto container = std::dynamic_pointer_cast<Container>(rend);
			if (container != nullptr)
			{
				container->performAutoLayoutIfNeeded();

				pushedColorStack = updateColorStack(container);
				renderUpdateRect(container, &computed, &renderPackage);
				container->setWorldRect(&computed);
				container->updateScreenRect(&renderPackage, renderPositionModeStack.top());

				if (currentScreenConfig.visualizeContainers)
				{
					debugObject = std::make_shared<Rectangle>();
					debugObject->markIsDebugElement();
					debugObject->matchSize(container);
					debugObject->topLeftAlignSelf();
					debugObject->setAlpha(0.20);
					debugObject->setColor(container->debugColor);
					debugObject->zIndex = 1;
					container->add(debugObject);
				}

				if (container->getHasClipRect())
				{
					updateClipRectOpenGL(container, &computed, &renderPackage);
				}
			}
		}
		break;
		}

		if (rend->visible && rend->renderType != RenderType::NoneAndBlockChildren)
		{
			const auto aoPtr = std::dynamic_pointer_cast<GameObject>(rend);
			if (aoPtr != nullptr)
			{
				renderPackage.depth += aoPtr->zIndex;
				for (const auto child : aoPtr->getChildrenOrdered())
				{
					const auto renderableChildPtr = std::dynamic_pointer_cast<Renderable>(child);
					if (renderableChildPtr != nullptr)
					{
						renderRecursive(renderableChildPtr, computed, renderPackage);
					}
				}
			}
		}

		if (debugObject != nullptr)
		{
			debugObject->removeFromParent();
			debugObject = nullptr;
		}

		if (pushedColorStack)
		{
			revertColorStack();
		}

		if (rend->getHasClipRect())
		{
			if (currentScreenConfig.visualizeClipRects)
			{
				const auto debugObject = std::make_shared<Rectangle>();
				debugObject->markIsDebugElement();
				debugObject->setSizeAndPosition(-2000.0, -2000.0, 30000.0, 30000.0);
				debugObject->zIndex = 20;
				debugObject->setAlpha(0.25);
				debugObject->onInitialAttach();
				colorStack.push(AW::Color::red());
				auto rect = Rect();
				auto renderPackage = RenderPackage();
				renderPrimitive(debugObject, &rect, &renderPackage);
				colorStack.pop();
			}

			renderPackage.stencilDepth--;

			if (renderPackage.stencilDepth == 0)
			{
				glDisable(GL_STENCIL_TEST);
			}
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

	void Renderer::renderUpdateRect(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage)
	{
		auto rect = rend->getRect();
		auto rectMiddleX = (rect.x - rect.w / 2.0);
		auto rectMiddleY = (rect.y - rect.h / 2.0);

		if (renderProcessingStack.top() == RenderPositionProcessing::Floor)
		{
			rectMiddleX = std::floor(rectMiddleX);
			rectMiddleY = std::floor(rectMiddleY);
		}

		double originW = computed->w;
		double originH = computed->h;
		computed->w = rect.w;
		computed->h = rect.h;

		if (rend->rotateInParentSpace)
		{
			double rotationRad = renderPackage->rotation * AW::NumberHelper::degToRad;
			double newX = computed->x + rectMiddleX;
			double newY = computed->y + rectMiddleY;
			double oX = computed->x + (originW / 2.0) - (computed->w / 2.0);
			double oY = computed->y + (originH / 2.0) - (computed->h / 2.0);
			double cX = newX - oX;
			double cY = newY - oY;
			double xP = cX * std::cos(rotationRad) - cY * std::sin(rotationRad);
			double yP = cY * std::cos(rotationRad) + cX * std::sin(rotationRad);

			computed->x = (xP + oX);
			computed->y = (yP + oY);

		}
		else
		{
			computed->x += rect.x - rect.w / 2.0;
			computed->y += rect.y - rect.h / 2.0;
		}

		renderPackage->rotation += rend->getRotation();
		renderPackage->alpha *= rend->getAlpha();
	}

	bool Renderer::updateColorStack(std::shared_ptr<Renderable> rend)
	{
		if (rend != nullptr && rend->getColor() != nullptr)
		{
			const auto currentColor = colorStack.empty() ? Color(255, 255, 255, 255) : Color(colorStack.top());
			const auto rendColor = rend->getColor();
			const auto newColor = Color(
				(currentColor.r / 255.0) * rendColor->r,
				(currentColor.g / 255.0) * rendColor->g,
				(currentColor.b / 255.0) * rendColor->b,
				(currentColor.a / 255.0) * rendColor->a
			);
			colorStack.push(newColor);
			return true;
		}

		return false;
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

	void Renderer::revertColorStack()
	{
		colorStack.pop();
	}

	bool Renderer::renderShouldCull(Rect* rect, RenderPackage* renderPackage)
	{
		const auto modifiedCullingOffset = cullingOffset * renderPackage->zoom;
		return rect->x > screenWidth + modifiedCullingOffset || rect->x + rect->w < -modifiedCullingOffset ||
			rect->y > screenHeight + modifiedCullingOffset || rect->h + rect->h < -modifiedCullingOffset;
	}

	void Renderer::renderElement(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage)
	{
		if (ele == nullptr)
		{
			return;
		}

		renderUpdateRect(ele, computed, renderPackage);

		ele->setWorldRect(computed);
		ele->updateScreenRect(renderPackage, renderPositionModeStack.top());

		const auto shouldCull = !ele->disableCulling && renderShouldCull(ele->getScreenRect(), renderPackage) && renderPositionModeStack.top() != RenderPositionMode::Absolute;
		if (shouldCull || !ele->visible || !ele->hasTexture())
		{
			return;
		}

		if (ele->getHasClipRect())
		{
			updateClipRectOpenGL(ele, computed, renderPackage);
		}

		switch (ele->renderType)
		{
		case RenderType::TileMap:
			renderTileMapOpenGL(ele, computed, renderPackage);
			break;

		default:
			renderElementOpenGL(ele, computed, renderPackage);
			break;
		}
	}

	void Renderer::renderPrimitive(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage)
	{
		if (prim == nullptr)
		{
			return;
		}

		renderUpdateRect(prim, computed, renderPackage);

		prim->preUpdateRender(computed, renderPackage);
		prim->setWorldRect(computed);
		prim->updateScreenRect(renderPackage, renderPositionModeStack.top());
		prim->preRender(computed, renderPackage);

		if (prim->getHasClipRect())
		{
			updateClipRectOpenGL(prim, computed, renderPackage);
		}

		switch (prim->renderType)
		{
		case RenderType::ParticleSystem:
			renderParticleSystemOpenGL(prim, computed, renderPackage);
			break;

		default:
			renderPrimitiveOpenGL(prim, computed, renderPackage);
			break;
		}
	}

	void Renderer::renderElementOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage)
	{
		const auto glTextureId = ele->getTexture()->openGlTextureId();
		if (glTextureId == 0)
		{
			return;
		}

		changeProgram(ele);

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

		if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
		{
			mat4x4_dup(tP, pAbs);
		}
		else
		{
			mat4x4_dup(tP, p);
		}

		mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

		setColorModParam(renderPackage);

		bindGLTexture(glTextureId);

		openGLDrawArrays(renderPackage);
	}

	void Renderer::updateClipRectOpenGL(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage)
	{
		const auto clipRect = rend->getClipRect();

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

		auto fragmentShader = rend->getFragmentShader();
		if (fragmentShader == nullptr)
		{
			fragmentShader = defaultFragmentShader;
		}

		changeProgram(vertexShader, fragmentShader);

		rend->cachedClipRectProgram = vertexShader->getCachedProgramId();

		applyShaderUniforms(vertexShader);
		applyShaderUniforms(fragmentShader);

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

	void Renderer::renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage)
	{
		changeProgram(prim);

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
		else
		{
			mat4x4_dup(tP, p);
		}

		mat4x4_mul(mvp, tP, m);

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

		setColorModParam(renderPackage);

		openGLDrawArrays(renderPackage);

	}

	void Renderer::renderParticleSystemOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage)
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
		else
		{
			mat4x4_dup(tP, p);
		}

		for (const auto particle : particleSystem->getParticles())
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
				applyShaderUniforms(pVShader);
				applyShaderUniforms(pFShader);
			}
			else
			{
				changeProgram(prim);
			}

			glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);
			glUniform4f(inColorModLocation, particle->cModR / 255.0, particle->cModG / 255.0, particle->cModB / 255.0, (particle->alphaMod / 255.0) * renderPackage->alpha);

			bindGLTexture(glTextureId);

			openGLDrawArrays(renderPackage);
		}
	}

	void Renderer::renderTileMapOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage)
	{
		const auto tileMap = std::dynamic_pointer_cast<TileMap>(ele);
		if (tileMap == nullptr)
		{
			return;
		}

		changeProgram(ele);

		const auto width = screenWidth, height = screenHeight;
		const auto zoom = renderPackage->zoom;
		auto texture = tileMap->getTexture();

		if (texture == nullptr)
		{
			return;
		}

		const double tileW = tileMap->getTileWidth();
		const double tileH = tileMap->getTileHeight();
		Rect tileSize = {
			0,
			0,
			tileW,
			tileH
		};

		int rows = std::ceil(computed->w / tileSize.w) + 1;
		int cols = std::ceil(computed->h / tileSize.h) + 1;
		int rowOffset = std::floor(rows / 2);
		int colOffset = std::floor(cols / 2);

		const auto glTextureId = texture->openGlTextureId();
		if (glTextureId != 0)
		{
			for (int r = 0; r < rows; ++r)
			{
				for (int c = 0; c < cols; ++c)
				{
					const auto offsetX = (computed->w - tileSize.w) / 2.0;
					const auto offsetY = (computed->h - tileSize.h) / 2.0;
					const auto cW = tileSize.w / 2.0;
					const auto cH = tileSize.h / 2.0;
					const auto cX = (computed->x + cW) + offsetX + tileSize.w * (r - rowOffset);
					const auto cY = (computed->y + cH) + offsetY + tileSize.h * (c - colOffset);

					mat4x4_identity(m);

					mat4x4_scale_aniso(m, m, cW, cH, 1.0);
					mat4x4_translate(t, cX, cY, (cY + (renderPackage->depth + tileMap->zIndex) * layerFactor));
					mat4x4_mul(m, t, m);

					if (renderPositionModeStack.top() == RenderPositionMode::Absolute)
					{
						mat4x4_dup(tP, pAbs);
					}
					else
					{
						mat4x4_dup(tP, p);
					}

					mat4x4_mul(mvp, tP, m);

					glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

					setColorModParam(renderPackage);

					bindGLTexture(glTextureId);

					openGLDrawArrays(renderPackage);
				}
			}
		}
	}
}
