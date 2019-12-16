#pragma once

#include <SDL.h>
#include <string>
#include "engine/module/IBaseModule.h"
#include "ScreenConfig.h"
#include "engine/module/collision/Collision.h"

namespace AWCore
{
	class ApplicationObject;
	class Collision;
	class Renderable;
	class Renderer;
	class Element;
	class Primitive;
	class Camera;

	class DisplayModeInfo
	{
	public:
		DisplayModeInfo() { }
		DisplayModeInfo(std::vector<SDL_DisplayMode> modes) {
			this->modes = modes;
			for (const auto mode : modes)
			{
				const auto k = std::to_string(mode.w) + "x" + std::to_string(mode.h);
				if (std::find(resolutions.begin(), resolutions.end(), k) == resolutions.end())
				{
					resolutions.push_back(k);
				}
			}

			std::sort(resolutions.begin(), resolutions.end(), [](const std::string& a, const std::string& b) { return AWCore::StringHelper::getDisplayComponentForDisplayString(a, 0) > AWCore::StringHelper::getDisplayComponentForDisplayString(b, 0); });
		};

		std::vector<SDL_DisplayMode> modes;
		std::vector<std::string> resolutions;

		int widthForResolution(std::string resolution) { return StringHelper::getDisplayComponentForDisplayString(&resolution, 0); }
		int heightForResolution(std::string resolution) { return StringHelper::getDisplayComponentForDisplayString(&resolution, 1); }
	};

	class Screen : public IBaseModule
	{
		SDL_Window* window;
		std::shared_ptr<Collision> collision;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<Camera> camera;
		ScreenConfig currentConfig;
		int windowWidth, windowHeight;

	public:
		virtual ~Screen();

		void bindCollision(std::shared_ptr<Collision> collision);

		bool init(const ScreenConfig& config, std::string name = "Window");
		int getWidth();
		int getHeight();
		void setClearColor(int r, int g, int b, int a = 0xff);
		void setWindowColorMod(int r, int g, int b);

		SDL_Window* getWindow();

		DisplayModeInfo getCurrentDisplayMode();
		DisplayModeInfo getAllSupportedDisplayModes();
		ScreenConfig getCurrentScreenConfig();

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
