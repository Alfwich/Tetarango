#include "Renderer.h"
#include "ui/renderable/element/Text.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "ui/renderable/element/tilemap/TileMap.h"
#include "ui/renderable/primitive/trace/Trace.h"

namespace
{
	const char* inlineDefaultFragmentShader =
		R"(
			#version 330 core
			in vec2 UV;
			in vec4 colorMod;
			in mat4 UVp;
			out vec4 color;
			uniform sampler2D myTextureSampler;
			void main() {
			   vec4 c = texture(myTextureSampler, (UVp* vec4(UV, 1, 1)).xy).rgba;
			   vec4 fC = c * colorMod;
				if (fC.a == 0)
					discard;

				color = fC;
			};
		)";

	const char* inlineDefaultVertexShader =
		R"(
			#version 330 core
			layout(location = 0) in vec3 vp;
			layout(location = 1) in vec2 vertexUV;
			out vec2 UV;
			out mat4 UVp;
			out vec4 colorMod;
			uniform mat4 mvp;
			uniform mat4 UVproj;
			uniform vec4 cMod;
			void main(){
				vec4 p = mvp * vec4(vp, 1);
				gl_Position = p;
				UV = vertexUV;
				UVp = UVproj;
				colorMod = cMod;
			};
		)";

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

namespace MT
{
	Renderer::Renderer(SDL_Window* window, const ScreenConfig& screenConfig, std::shared_ptr<Renderer> oldRenderer)
	{
		renderPositionModeStack.push(RenderPositionMode::Positioned);

		if (oldRenderer != nullptr)
		{
			harvestFromPreviousRenderer(oldRenderer);
			oldRenderer->releaseOpenGLObjects();
		}
		else
		{
			setClearColor(0xff, 0xff, 0xff, 0xff);
			setGlobalColorMod(0xff, 0xff, 0xff);
		}

		initOpenGL(window, screenConfig);
	}

	void Renderer::initOpenGL(SDL_Window* window, const ScreenConfig& screenConfig)
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

		if (major != screenConfig.openGLMajorVersion || minor != screenConfig.openGLMinorVersion)
		{
			Logger::instance()->logCritical(
				"Renderer::OpenGL::Did not get expected context "
				+ std::to_string(screenConfig.openGLMajorVersion) + "." + std::to_string(screenConfig.openGLMinorVersion)
				+ ", got major=" + std::to_string(major) + ", minor=" + std::to_string(minor)
			);
		}

		GLint compileResult = GL_FALSE;
		int infoLength;

		if (vao == 0)
		{
			glGenVertexArrays(1, &vao);
		}
		glBindVertexArray(vao);

		if (vertexBuffer == 0)
		{
			glGenBuffers(1, &vertexBuffer);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(inlineVerticies), inlineVerticies, GL_STATIC_DRAW);

		if (textureUVBuffer == 0)
		{
			glGenBuffers(1, &textureUVBuffer);
		}
		glBindBuffer(GL_ARRAY_BUFFER, textureUVBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(inlineUVCoords), inlineUVCoords, GL_STATIC_DRAW);

		if (vertexShader == 0)
		{
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &inlineDefaultVertexShader, NULL);
			glCompileShader(vertexShader);

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileResult);
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLength);
			if (compileResult == GL_FALSE && infoLength > 0) {
				std::vector<char> infoText(infoLength + 1);
				glGetShaderInfoLog(vertexShader, infoLength, NULL, &infoText[0]);
				Logger::instance()->logFatal("Renderer::OpenGL::Failed to compile vertex shader: " + std::string(infoText.begin(), infoText.end()));
			}
		}

		if (fragmentShader == 0)
		{
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &inlineDefaultFragmentShader, NULL);
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileResult);
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLength);
			if (compileResult == GL_FALSE && infoLength > 0) {
				std::vector<char> infoText(infoLength + 1);
				glGetShaderInfoLog(fragmentShader, infoLength, NULL, &infoText[0]);
				Logger::instance()->logFatal("Renderer::OpenGL::Failed to compile fragment shader: " + std::string(infoText.begin(), infoText.end()));
			}
		}

		if (program == 0)
		{
			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			GLint programLinkSuccess = GL_TRUE;
			glGetProgramiv(program, GL_LINK_STATUS, &programLinkSuccess);

			if (programLinkSuccess != GL_TRUE)
			{
				std::vector<char> infoText(infoLength + 1);
				glGetProgramInfoLog(program, infoLength, NULL, &infoText[0]);
				Logger::instance()->logFatal("Renderer::OpenGL::Failed to link program: " + std::string(infoText.begin(), infoText.end()));
			}
		}

		if (screenConfig.openGlWireframeMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		layerFactor = (1 << 16) / maxLayers;
	}

	void Renderer::harvestFromPreviousRenderer(std::shared_ptr<Renderer> previous)
	{
		clearColor = previous->clearColor;
		globalColorMod = previous->globalColorMod;
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

		if (vertexShader != 0)
		{
			glDeleteShader(vertexShader);
			vertexShader = 0;
		}

		if (fragmentShader != 0)
		{
			glDeleteShader(fragmentShader);
			fragmentShader = 0;
		}

		if (program != 0)
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	void Renderer::setClearColor(int r, int g, int b, int a)
	{
		clearColor.r = r;
		clearColor.g = g;
		clearColor.b = b;
		clearColor.a = a;
	}

	void Renderer::setGlobalColorMod(int r, int g, int b)
	{
		globalColorMod.r = r;
		globalColorMod.g = g;
		globalColorMod.b = b;
	}

	void Renderer::render(std::shared_ptr<ApplicationObject> root, Screen* screen, std::shared_ptr<QuadMap> qm)
	{
		prepareRender(screen);

		Rect rootRect;
		RenderPackage renderPackage;
		renderPackage.zoom = camera == nullptr ? 1.0 : camera->getZoom();
		renderPackage.cameraX = camera == nullptr ? 0.0 : camera->getX() - screenWidth / 2;
		renderPackage.cameraY = camera == nullptr ? 0.0 : camera->getY() - screenHeight / 2;
		renderPackage.xOffset = camera == nullptr ? 0.0 : camera->getXOffset();
		renderPackage.yOffset = camera == nullptr ? 0.0 : camera->getYOffset();
		renderPackage.quadMap = qm;

		if (root->getTag(AOTags::IsRootElement))
		{
			rootRect.w = screen->getWidth();
			rootRect.h = screen->getHeight();
		}

		renderOpenGL(root, rootRect, screen, &renderPackage);
		reportOpenGLErrors();
	}

	void Renderer::renderOpenGL(std::shared_ptr<ApplicationObject> root, Rect rootRect, Screen* screen, RenderPackage* renderPackage)
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

		glUseProgram(program);

		inMatrixLocation = glGetUniformLocation(program, "mvp");
		inUVMatrixLocation = glGetUniformLocation(program, "UVproj");
		inColorModLocation = glGetUniformLocation(program, "cMod");

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, textureUVBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderRecursive(root, rootRect, *renderPackage);

		SDL_GL_SwapWindow(screen->getWindow());

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glUseProgram(0);
	}

	void Renderer::openGLDrawArrays(RenderPackage* renderPackage)
	{
		if (renderPackage->stencilDepth > 0)
		{
			glStencilFunc(GL_EQUAL, 1, 0xFF);
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

	void Renderer::prepareRender(Screen* screen)
	{
		screenWidth = screen->getWidth();
		screenHeight = screen->getHeight();
		camera = screen->getCamera();
	}

	void Renderer::renderRecursive(std::shared_ptr<ApplicationObject> ao, Rect computed, RenderPackage renderPackage)
	{
		switch (ao->renderPositionMode)
		{
		case RenderPositionMode::Absolute:
		case RenderPositionMode::Positioned:
			renderPositionModeStack.push(ao->renderPositionMode);
		}

		if (ao->getHasClipRect())
		{
			if (renderPackage.stencilDepth == 0)
			{
				glClear(GL_STENCIL_BUFFER_BIT);
				glEnable(GL_STENCIL_TEST);
			}

			renderPackage.stencilDepth++;
		}

		bool pushedColorStack = false;
		switch (ao->renderType)
		{
		case RenderType::Element:
		case RenderType::Backdrop:
		case RenderType::TileMap:
		{
			const auto ele = std::static_pointer_cast<Element>(ao);

			pushedColorStack = updateColorStack(ele);
			renderElement(ele, &computed, &renderPackage);
		}
		break;

		case RenderType::Primitive:
		case RenderType::ParticleSystem:
		{
			const auto prim = std::static_pointer_cast<Primitive>(ao);

			pushedColorStack = updateColorStack(prim);
			renderPrimitive(prim, &computed, &renderPackage);
		}
		break;

		case RenderType::Container:
		{
			const auto renderable = std::dynamic_pointer_cast<Renderable>(ao);
			if (renderable != nullptr)
			{
				pushedColorStack = updateColorStack(renderable);
				renderUpdateRect(renderable, &computed, &renderPackage);
				ao->setWorldRect(&computed);
				ao->updateScreenRect(&renderPackage);
			}

			if (ao->getHasClipRect())
			{
				updateClipRectOpenGL(ao, &computed, &renderPackage);
			}
		}
		break;
		}

		if (ao->getTag(AOTags::IsZone))
		{
			const auto collidable = std::dynamic_pointer_cast<ICollidable>(ao);
			renderPackage.quadMap->insert(collidable);
		}

		if (ao->visible && ao->renderType != RenderType::NoneAndBlockChildren)
		{
			renderPackage.depth += ao->zIndex;
			for (const auto child : ao->getChildrenRenderOrder())
			{
				renderRecursive(child, computed, renderPackage);
			}
		}

		if (pushedColorStack)
		{
			revertColorStack();
		}

		if (ao->getHasClipRect())
		{
			renderPackage.stencilDepth--;

			if (renderPackage.stencilDepth == 0)
			{
				glDisable(GL_STENCIL_TEST);
			}
		}

		switch (ao->renderPositionMode)
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

		double originW = computed->w;
		double originH = computed->h;
		computed->w = rect.w;
		computed->h = rect.h;

		if (rend->rotateInParentSpace)
		{
			double rotationRad = renderPackage->rotation * MT::NumberHelper::degToRad;
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
		ele->updateScreenRect(renderPackage);

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
		prim->updateScreenRect(renderPackage);
		prim->preRender(computed, renderPackage);

		const auto shouldCull = !prim->disableCulling && renderShouldCull(prim->getScreenRect(), renderPackage);
		if (shouldCull || prim == nullptr || !prim->visible)
		{
			return;
		}

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

		const auto cW = computed->w / 2.0;
		const auto cH = computed->h / 2.0;
		const auto cX = computed->x + cW;
		const auto cY = computed->y + cH;

		mat4x4_identity(m);

		mat4x4_rotate_Z(m, m, renderPackage->rotation * MT::NumberHelper::degToRad);
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

		mat4x4_identity(UVp);

		if (ele->renderType == RenderType::Backdrop)
		{
			mat4x4_scale_aniso(UVp, UVp, ele->getWidth() / ele->getTexture()->getWidth(), ele->getHeight() / ele->getTexture()->getHeight(), 1.0);
		}

		const auto textureClipRect = ele->getTextureClipRect();
		if (textureClipRect != nullptr)
		{
			double scaleX = textureClipRect->w / ele->getTexture()->getWidth();
			double scaleY = textureClipRect->h / ele->getTexture()->getHeight();
			double xOffset = textureClipRect->x / ele->getTexture()->getWidth();
			double yOffset = textureClipRect->y / ele->getTexture()->getHeight();

			mat4x4_translate_in_place(UVp, xOffset, yOffset, 0.0);
			mat4x4_scale_aniso(UVp, UVp, scaleX, scaleY, 1.0);
		}

		glUniformMatrix4fv(inUVMatrixLocation, 1, GL_FALSE, (const GLfloat*)UVp);

		setColorModParam(renderPackage);

		glBindTexture(GL_TEXTURE_2D, glTextureId);

		openGLDrawArrays(renderPackage);
	}

	void Renderer::updateClipRectOpenGL(std::shared_ptr<ApplicationObject> ao, Rect* computed, RenderPackage* renderPackage)
	{
		const auto clipRect = ao->getClipRect();

		const auto cW = clipRect->w / 2.0;
		const auto cH = clipRect->h / 2.0;
		const auto cX = computed->x + clipRect->x + computed->w / 2.0;
		const auto cY = computed->y + clipRect->y + computed->h / 2.0;

		mat4x4_identity(m);

		mat4x4_rotate_Z(m, m, renderPackage->rotation * MT::NumberHelper::degToRad);
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

		glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);

		mat4x4_identity(UVp);
		glUniformMatrix4fv(inUVMatrixLocation, 1, GL_FALSE, (const GLfloat*)UVp);

		glBindTexture(GL_TEXTURE_2D, 0);

		openGLDrawArraysStencil(renderPackage);

	}

	void Renderer::renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage)
	{
		// TODO - If needed; Right now Primitives either render through a special render method (ParticleSystem) or composition (Trace)
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
			const auto glTextureId = particle->getTexture()->openGlTextureId();
			if (glTextureId != 0)
			{
				const auto cW = particle->w / 2.0;
				const auto cH = particle->h / 2.0;
				const auto cX = computed->x + particle->x + cW;
				const auto cY = computed->y + particle->y + cH;
				const auto rotation = (renderPackage->rotation + particle->r) * MT::NumberHelper::degToRad;

				mat4x4_identity(m);

				mat4x4_rotate_Z(m, m, rotation);
				mat4x4_scale_aniso(m, m, cW, cH, 1.0);
				mat4x4_translate(t, cX, cY, cY + (particleSystem->zIndex + particle->zIndex + renderPackage->depth) * layerFactor);
				mat4x4_mul(m, t, m);
				mat4x4_mul(mvp, tP, m);

				mat4x4_identity(UVp);

				const auto clipRect = particle->clip;
				double scaleX = clipRect.w / particle->getTexture()->getWidth();
				double scaleY = clipRect.h / particle->getTexture()->getHeight();
				double xOffset = clipRect.x / particle->getTexture()->getWidth();
				double yOffset = clipRect.y / particle->getTexture()->getHeight();

				mat4x4_translate_in_place(UVp, xOffset, yOffset, 0.0);
				mat4x4_scale_aniso(UVp, UVp, scaleX, scaleY, 1.0);

				glUniformMatrix4fv(inMatrixLocation, 1, GL_FALSE, (const GLfloat*)mvp);
				glUniformMatrix4fv(inUVMatrixLocation, 1, GL_FALSE, (const GLfloat*)UVp);
				glUniform4f(inColorModLocation, particle->cModR / 255.0, particle->cModG / 255.0, particle->cModB / 255.0, (particle->alphaMod / 255.0) * renderPackage->alpha);

				glBindTexture(GL_TEXTURE_2D, glTextureId);

				openGLDrawArrays(renderPackage);
			}
		}
	}

	void Renderer::renderTileMapOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage)
	{
		const auto tileMap = std::dynamic_pointer_cast<TileMap>(ele);
		if (tileMap == nullptr)
		{
			return;
		}

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

		mat4x4_identity(UVp);
		glUniformMatrix4fv(inUVMatrixLocation, 1, GL_FALSE, (const GLfloat*)UVp);

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

					glBindTexture(GL_TEXTURE_2D, glTextureId);

					openGLDrawArrays(renderPackage);
				}
			}
		}
	}
}
