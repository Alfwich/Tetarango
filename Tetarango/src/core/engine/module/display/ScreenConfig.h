#pragma once
namespace AWCore
{
	enum class ScreenModes
	{
		Fullscreen,
		FullscreenDesktop,
		Windowed
	};

	enum class VsyncModes
	{
		Disabled,
		Enabled,
		Adaptive
	};

	class ScreenConfig
	{
	public:
		int width = 1280, height = 1024, windowFlags = 0;
		ScreenModes mode = ScreenModes::Windowed;
		VsyncModes vMode = VsyncModes::Adaptive;
		bool openGlWireframeMode = false, openGLCompatibilityMode = false, useDoubleBuffer = true, visualizeContainers = false, visualizeClipRects = false;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 4, frameLimiter = 0;
	};
}

