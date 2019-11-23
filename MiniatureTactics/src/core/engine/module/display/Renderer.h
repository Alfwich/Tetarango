#pragma once

#include <algorithm>
#include <stack>
#include <SDL.h>
#include "GL/glew.h"
#include "engine/ApplicationObject.h"
#include "ui/Camera.h"
#include "util/Rect.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Primitive.h"
#include "RenderPackage.h"
#include "util/linmath.h"
#include "util/QuadMap.h"
#include "util/Color.h"

namespace MT
{

	class Renderer
	{
		SDL_GLContext glContext;
		Color clearColor, globalColorMod;

		mat4x4 mvp, p, pAbs, m, t, UVp, tP;
		GLuint vertexBuffer, textureUVBuffer, vertexShader, fragmentShader, program, vao;
		GLuint inMatrixLocation, inUVMatrixLocation, inColorModLocation;

		std::shared_ptr<Camera> camera;
		std::stack<RenderPositionMode> renderPositionMode;
		int screenWidth = 0, screenHeight = 0, layerFactor = 1, maxLayers = 60, cullingOffset = 500;

		void prepareRender(Screen* screen);
		void renderRecursive(std::shared_ptr<ApplicationObject> ao, Rect computed, RenderPackage parentRotation);
		void setTextureColorMod(SDL_Texture* texture, Color* eleColorMod);
		void setTextureAlphaMod(SDL_Texture* texture, RenderPackage* renderPackage);
		void renderUpdateRect(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);

		bool renderShouldCull(Rect* r, RenderPackage* renderPackage);

		void initOpenGL(SDL_Window* window, const ScreenConfig& screenConfig);

		void renderOpenGL(std::shared_ptr<ApplicationObject> obj, Rect rootRect, Screen* screen, RenderPackage* package);
		void renderElement(std::shared_ptr<ApplicationObject> ao, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitive(std::shared_ptr<ApplicationObject> ao, Rect* computed, RenderPackage* renderPackage);

		void renderElementOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);
		void renderParticleSystemOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);
		void renderTileMapOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);

		void openGLDrawArrays();
	public:
		Renderer(SDL_Window* window, const ScreenConfig& screenConfig);
		virtual ~Renderer();

		bool isOpenGLEnabled();

		void setClearColor(int r, int g, int b, int a);
		void setGlobalColorMod(int r, int g, int b);
		void render(std::shared_ptr<ApplicationObject> obj, Screen* screen, std::shared_ptr<QuadMap> qm);

		SDL_GLContext getOpenGLContext();
	};
}

