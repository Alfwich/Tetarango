#include "Screen.h"

namespace
{
	const auto VsyncOn = 1;
	const auto VsyncOff = 0;
	const auto VsyncAdaptive = -1;
}

namespace AW
{

	Screen::~Screen()
	{
		SDL_DestroyWindow(window);
	}

	void Screen::bindTexture(std::shared_ptr<TextureContainer> texture)
	{
		this->texture = texture;
	}

	void Screen::bindShader(std::shared_ptr<ShaderContainer> shader)
	{
		this->shader = shader;
	}

	void Screen::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
	}

	void Screen::bindPhysic(std::shared_ptr<Physic> physic)
	{
		this->physic = physic;
	}

	bool Screen::init(const ScreenConfig& config, std::string name)
	{
		currentConfig = config;

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
			Logger::instance()->logCritical("Screen::Attempting to set unsupported display resolution width=" + std::to_string(currentConfig.width) + ", height=" + std::to_string(currentConfig.height));
		}

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

		SDL_DisplayMode desktopMode, displayMode;

		SDL_GetDesktopDisplayMode(0, &desktopMode);
		SDL_GetDisplayMode(0, 0, &displayMode);

		maxWindowedWidth = desktopMode.w;
		maxWindowedHeight = desktopMode.h;

		double scaling = desktopMode.w / (double)displayMode.w;
		currentConfig.deviceScaling = scaling;

		if (currentConfig.mode == ScreenModes::FullscreenDesktop
			|| currentConfig.width > desktopMode.w
			|| currentConfig.height > desktopMode.h)
		{
			currentConfig.width = desktopMode.w;
			currentConfig.height = desktopMode.h;
		}

		if (window == nullptr)
		{
			int windowFlags = windowFlags = SDL_WINDOW_OPENGL;

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

			currentConfig.visualizeContainers = currentConfig.visualizeContainers || gameConfig->getConfigBool(Config::Param::visualizeContainers);
			currentConfig.visualizeClipRects = currentConfig.visualizeClipRects || gameConfig->getConfigBool(Config::Param::visualizeClipRects);

			window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, currentConfig.width, currentConfig.height, windowFlags);
		}
		else
		{
			SDL_DisplayMode currentMode;
			SDL_GetWindowDisplayMode(window, &currentMode);

			currentMode.w = currentConfig.width;
			currentMode.h = currentConfig.height;

			switch (currentConfig.mode)
			{
			case ScreenModes::Fullscreen:
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				SDL_SetWindowBordered(window, SDL_FALSE);
				break;

			case ScreenModes::FullscreenDesktop:
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				SDL_SetWindowBordered(window, SDL_FALSE);
				currentMode.w = desktopMode.w;
				currentMode.h = desktopMode.h;
				break;

			case ScreenModes::Windowed:
			default:
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowBordered(window, SDL_TRUE);
				break;
			}

			SDL_SetWindowDisplayMode(window, &currentMode);
			SDL_SetWindowSize(window, currentConfig.width, currentConfig.height);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			SDL_GL_SwapWindow(window);
		}

		if (window == nullptr)
		{
			Logger::instance()->logCritical("Screen::Failed to create SDL window");
			return false;
		}

		bool rendererInit = true;
		if (renderer == nullptr)
		{
			renderer = std::make_shared<Renderer>(currentConfig, renderer);
			rendererInit = renderer->initOpenGL(window);
		}
		else
		{
			renderer->updateScreenConfig(currentConfig);
		}

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

		return window != nullptr && rendererInit;
	}

	void Screen::bindDefaultShaders()
	{
		if (renderer != nullptr)
		{
			renderer->setDefaultShaders(shader->getLockedShader({ "v-default" }), shader->getLockedShader({ "f-default" }));
		}
	}

	int Screen::getWidth()
	{
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		return windowWidth;
	}

	int Screen::getHeight()
	{
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		return windowHeight;
	}

	int Screen::getMaxWindowedWidth()
	{
		return maxWindowedWidth;
	}

	int Screen::getMaxWindowedHeight()
	{
		return maxWindowedHeight;
	}

	double Screen::getDeviceScaling()
	{
		return currentConfig.deviceScaling;
	}

	void Screen::setClearColor(int r, int g, int b, int a)
	{
		if (renderer != nullptr)
		{
			renderer->setClearColor(r, g, b, a);
		}
	}

	void Screen::disableClear()
	{
		if (renderer != nullptr)
		{
			renderer->setClearEnabled(false);
		}
	}

	void Screen::enableClear()
	{
		if (renderer != nullptr)
		{
			renderer->setClearEnabled(true);
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
			else
			{
				result.push_back(mode);
			}
		}

		return DisplayModeInfo(result);
	}

	ScreenConfig Screen::getCurrentScreenConfig()
	{
		return currentConfig;
	}

	void Screen::render(std::shared_ptr<Renderable> root)
	{
		if (root != nullptr)
		{
			renderer->render(root, this, time->getHighResolutionTicks() / 1000.0);

#if _DEBUG
			if (gameConfig->getConfigBool(Config::Param::immediateDebugOutput))
			{
				renderer->reportOpenGLErrors();
			}
#endif // _DEBUG

			if (currentConfig.visualizePhysic)
			{
				physic->performDebugDraw(currentConfig.width, currentConfig.height);
			}

			SDL_GL_SwapWindow(window);
		}
	}

	void Screen::onReady()
	{
		bindDefaultShaders();
	}

	void Screen::onCleanup()
	{
	}
}
