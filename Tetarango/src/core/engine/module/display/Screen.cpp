#include "Screen.h"

namespace
{
	const auto VsyncOn = 1;
	const auto VsyncOff = 0;
	const auto VsyncAdaptive = -1;
}

namespace AWCore
{

	Screen::~Screen()
	{
		SDL_DestroyWindow(window);
	}

	void Screen::bindCollision(std::shared_ptr<Collision> collision)
	{
		this->collision = collision;
	}

	bool Screen::init(const ScreenConfig& config, std::string name)
	{
		currentConfig = config;

		if (window != nullptr)
		{
			SDL_DestroyWindow(window);
		}

		bool isSupportedDisplayResolution = false;
		for (const auto displayMode : getAllSupportedDisplayModes().modes)
		{
			isSupportedDisplayResolution = displayMode.w == currentConfig.width && displayMode.h == currentConfig.height;

			if (isSupportedDisplayResolution)
			{
				break;
			}
		}

		if (!isSupportedDisplayResolution)
		{
			return false;
		}

		int windowFlags = windowFlags = SDL_WINDOW_OPENGL;
		if (!currentConfig.openGLCompatibilityMode)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, currentConfig.openGLMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, currentConfig.openGLMinorVersion);

		if (currentConfig.msaaSamples > 0)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, currentConfig.msaaSamples);

		}
		else
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
		}

		switch (currentConfig.mode)
		{
		case ScreenModes::Fullscreen:
			windowFlags = (windowFlags | SDL_WINDOW_FULLSCREEN);
			break;

		case ScreenModes::FullscreenDesktop:
			windowFlags = (windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
			break;

		case ScreenModes::Windowed:
		default:
			break;
		}

		currentConfig.visualizeContainers = gameConfig->getConfigBool(Config::Param::visualizeContainers);
		currentConfig.visualizeClipRects = gameConfig->getConfigBool(Config::Param::visualizeClipRects);

		window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, currentConfig.width, currentConfig.height, windowFlags);

		renderer = std::make_shared<Renderer>(window, currentConfig, renderer);

		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		switch (currentConfig.vMode)
		{
		case VsyncModes::Adaptive:
			if (SDL_GL_SetSwapInterval(VsyncAdaptive) == -1)
			{
				currentConfig.vMode = VsyncModes::Enabled;
				if (SDL_GL_SetSwapInterval(VsyncOn) == -1)
				{
					currentConfig.vMode = VsyncModes::Disabled;
					SDL_GL_SetSwapInterval(VsyncOff);
				}
			}
			break;

		case VsyncModes::Enabled:
			if (SDL_GL_SetSwapInterval(VsyncOn) == -1)
			{
				currentConfig.vMode = VsyncModes::Disabled;
				SDL_GL_SetSwapInterval(VsyncOff);
			}
			break;

		case VsyncModes::Disabled:
		default:
			SDL_GL_SetSwapInterval(VsyncOff);
		}

		return window != nullptr && renderer->getOpenGLContext() != nullptr;
	}

	int Screen::getWidth()
	{
		return windowWidth;
	}

	int Screen::getHeight()
	{
		return windowHeight;
	}

	void Screen::setClearColor(int r, int g, int b, int a)
	{
		if (renderer != nullptr)
		{
			renderer->setClearColor(r, g, b, a);
		}
	}

	void Screen::setWindowColorMod(int r, int g, int b)
	{
		if (renderer != nullptr)
		{
			renderer->setGlobalColorMod(r, g, b);
		}
	}

	SDL_Window* Screen::getWindow()
	{
		return window;
	}

	DisplayModeInfo Screen::getCurrentDisplayMode()
	{
		std::vector<SDL_DisplayMode> result;
		if (SDL_GetNumVideoDisplays() < 0)
		{
			return DisplayModeInfo(result);
		}

		SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
		if (SDL_GetCurrentDisplayMode(0, &mode))
		{
			Logger::instance()->logCritical("Screen::Could not get display mode for video display: " + std::string(SDL_GetError()));
		}
		else
		{
			result.push_back(mode);
		}

		return DisplayModeInfo(result);
	}

	DisplayModeInfo Screen::getAllSupportedDisplayModes()
	{
		std::vector<SDL_DisplayMode> result;

		if (SDL_GetNumVideoDisplays() <= 0)
		{
			return DisplayModeInfo(result);
		}

		const auto numModes = SDL_GetNumDisplayModes(0);
		if (numModes <= 0)
		{
			return DisplayModeInfo(result);
		}

		for (auto i = 0; i < numModes; ++i)
		{
			SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

			if (SDL_GetDisplayMode(0, i, &mode))
			{
				Logger::instance()->logCritical("Screen::Could not get display mode for video display: " + std::string(SDL_GetError()));
			}

			result.push_back(mode);
		}

		return DisplayModeInfo(result);
	}

	ScreenConfig Screen::getCurrentScreenConfig()
	{
		return currentConfig;
	}

	bool Screen::isOpenGLEnabled()
	{
		return renderer->isOpenGLEnabled();
	}

	SDL_GLContext Screen::getOpenGLContext()
	{
		return renderer->getOpenGLContext();
	}

	std::shared_ptr<Camera> Screen::getCamera()
	{
		return camera;
	}

	void Screen::setCamera(std::shared_ptr<Camera> camera)
	{
		this->camera = camera;
	}

	void Screen::clearCamera()
	{
		camera = nullptr;
	}

	void Screen::render(std::shared_ptr<ApplicationObject> root)
	{
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		if (root != nullptr)
		{
			collision->collisionQuadMap->clear();
			renderer->render(root, this, collision->collisionQuadMap);
		}
	}
}
