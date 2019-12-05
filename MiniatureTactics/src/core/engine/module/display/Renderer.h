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
		GLuint vertexBuffer = 0, textureUVBuffer = 0, vertexShader = 0, fragmentShader = 0, program = 0, vao = 0;
		GLuint inMatrixLocation = 0, inUVMatrixLocation = 0, inColorModLocation = 0;

		std::shared_ptr<Camera> camera;
		std::stack<RenderPositionMode> renderPositionModeStack;
		std::stack<Color> colorStack;
		int screenWidth = 0, screenHeight = 0, layerFactor = 1, maxLayers = 60, cullingOffset = 500;

		void prepareRender(Screen* screen);
		void renderRecursive(std::shared_ptr<ApplicationObject> ao, Rect computed, RenderPackage parentRotation);
		void renderUpdateRect(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);

		bool updateColorStack(std::shared_ptr<Renderable> rend);
		void setColorModParam(RenderPackage* renderPackage);
		void revertColorStack();

		bool renderShouldCull(Rect* r, RenderPackage* renderPackage);

		void initOpenGL(SDL_Window* window, const ScreenConfig& screenConfig);
		void harvestFromPreviousRenderer(std::shared_ptr<Renderer> previous);
		void releaseOpenGLObjects();

		void renderOpenGL(std::shared_ptr<ApplicationObject> obj, Rect rootRect, Screen* screen, RenderPackage* package);
		void renderElement(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitive(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);

		void updateClipRectOpenGL(std::shared_ptr<ApplicationObject> ele, Rect* computed, RenderPackage* renderPackage);

		void renderElementOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);
		void renderParticleSystemOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);
		void renderTileMapOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);

		void openGLDrawArrays(RenderPackage* renderPackage);
		void openGLDrawArraysStencil(RenderPackage* renderPackage);
	public:
		Renderer(SDL_Window* window, const ScreenConfig& screenConfig, std::shared_ptr<Renderer> oldRenderer);
		virtual ~Renderer();

		bool isOpenGLEnabled();

		void setClearColor(int r, int g, int b, int a);
		void setGlobalColorMod(int r, int g, int b);
		void render(std::shared_ptr<ApplicationObject> obj, Screen* screen, std::shared_ptr<QuadMap> qm);

		SDL_GLContext getOpenGLContext();

		void reportOpenGLErrors();
	};
}

