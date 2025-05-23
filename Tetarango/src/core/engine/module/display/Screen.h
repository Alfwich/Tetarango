#pragma once

#include "ScreenConfig.h"
#include "engine/module/IBaseModule.h"

namespace AW
{
	class Renderable;
	class Renderer;
	class Element;
	class Primitive;
	class Camera;
	class ShaderContainer;
	class TextureContainer;
	class Time;
	class Physic;

	class DisplayModeInfo
	{
	public:
		DisplayModeInfo() {}
		DisplayModeInfo(std::vector<SDL_DisplayMode> modes)
		{
			this->modes = modes;
			for (const auto mode : modes)
			{
				const auto k = std::to_string(mode.w) + "x" + std::to_string(mode.h);
				if (std::find(resolutions.begin(), resolutions.end(), k) == resolutions.end())
				{
					resolutions.push_back(k);
				}
			}

			std::sort(resolutions.begin(), resolutions.end(), [](const std::string& a, const std::string& b) { return AW::StringHelper::getDisplayComponentForDisplayString(a, 0) > AW::StringHelper::getDisplayComponentForDisplayString(b, 0); });
		};

		std::vector<SDL_DisplayMode> modes;
		std::vector<std::string> resolutions;

		int widthForResolution(std::string resolution) { return StringHelper::getDisplayComponentForDisplayString(&resolution, 0); }
		int heightForResolution(std::string resolution) { return StringHelper::getDisplayComponentForDisplayString(&resolution, 1); }
	};

	class Screen : public IBaseModule
	{
		SDL_Window* window;
		ScreenConfig currentConfig;
		int windowWidth = 0, windowHeight = 0, maxWindowedWidth = 0, maxWindowedHeight = 0;

		std::shared_ptr<TextureContainer> texture;
		std::shared_ptr<ShaderContainer> shader;
		std::shared_ptr<Time> time;
		std::shared_ptr<Physic> physic;

		std::shared_ptr<Renderer> renderer;

	public:
		virtual ~Screen();

		void bindTexture(std::shared_ptr<TextureContainer> texture);
		void bindShader(std::shared_ptr<ShaderContainer> shader);
		void bindTime(std::shared_ptr<Time> time);
		void bindPhysic(std::shared_ptr<Physic> physic);

		bool init(const ScreenConfig& config, std::string name = "Window");
		void bindDefaultShaders();

		int getWidth();
		int getHeight();

		int getMaxWindowedHeight();
		int getMaxWindowedWidth();

		double getDeviceScaling();

		void setClearColor(int r, int g, int b, int a = 0xff);
		void disableClear();
		void enableClear();

		SDL_Window* getWindow();

		DisplayModeInfo getCurrentDisplayMode();
		DisplayModeInfo getAllSupportedDisplayModes();
		ScreenConfig getCurrentScreenConfig();

		void render(std::shared_ptr<Renderable> obj);

		void onReady();

		virtual void onBindLuaHooks(const std::shared_ptr<Lua>& lua) override;

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override { return "screen"; };
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
	};
}

#include "Renderer.h"
#include "ui/camera/Camera.h"
#include "ui/renderable/Renderable.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Primitive.h"
#include "engine/module/shader/ShaderContainer.h"
#include "engine/module/display/TextureContainer.h"
#include "engine/module/time/Time.h"
#include "engine/module/physic/Physic.h"
