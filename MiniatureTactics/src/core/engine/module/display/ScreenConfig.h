#pragma once
namespace MT
{
	enum class ScreenModes
	{
		Fullscreen,
		FullscreenDesktop,
		Windowed
	};

	class ScreenConfig
	{
	public:
		int width = 640, height = 480, windowFlags = 0;
		ScreenModes mode = ScreenModes::Windowed;
		bool openGlWireframeMode = false, useForwardCompatible = true, useMSAA = true;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 8;
	};
}

