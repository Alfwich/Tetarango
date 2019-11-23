#include "Screen.h"
namespace MT
{

	Screen::~Screen()
	{
		SDL_DestroyWindow(window);
	}

	void Screen::bindCollision(std::shared_ptr<Collision> collision)
	{
		this->collision = collision;
	}

	void Screen::init(const ScreenConfig& config, std::string name)
	{
		if (window != nullptr)
		{
			SDL_DestroyWindow(window);
		}

		int windowFlags = windowFlags = SDL_WINDOW_OPENGL;
		if (config.useForwardCompatible)
		{
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		}

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, config.openGLMajorVersion);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, config.openGLMinorVersion);

		if (config.useMSAA)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, config.msaaSamples);
		}

		window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.width, config.height, windowFlags | (config.isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

		renderer = std::make_unique<Renderer>(window, config);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
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
