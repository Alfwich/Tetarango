#pragma once

#include <SDL.h>
#include <string>
#include "engine/module/IBaseModule.h"
#include "ScreenConfig.h"
#include "engine/module/collision/Collision.h"

namespace MT
{
	class ApplicationObject;
	class Collision;
	class Renderable;
	class Renderer;
	class Element;
	class Primitive;
	class Camera;

	class Screen : public IBaseModule
	{
		SDL_Window* window;
		std::shared_ptr<Collision> collision;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Camera> camera;
		int windowWidth, windowHeight;

	public:
		virtual ~Screen();

		void bindCollision(std::shared_ptr<Collision> collision);

		void init(const ScreenConfig& config, std::string name = "Window");
		int getWidth();
		int getHeight();
		void setClearColor(int r, int g, int b, int a = 0xff);
		void setWindowColorMod(int r, int g, int b);

		SDL_Window* getWindow();

		bool isOpenGLEnabled();
		SDL_GLContext getOpenGLContext();

		std::shared_ptr<Camera> getCamera();
		void setCamera(std::shared_ptr<Camera> camera);
		void clearCamera();

		void render(std::shared_ptr<ApplicationObject> obj);
	};
}

#include "engine/ApplicationObject.h"
#include "Renderer.h"
#include "ui/Camera.h"
#include "ui/renderable/Renderable.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Primitive.h"
#include "engine/module/collision/Collision.h"
