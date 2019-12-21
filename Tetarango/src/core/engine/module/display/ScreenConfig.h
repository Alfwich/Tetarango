#pragma once
namespace AW
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
		bool openGlWireframeMode = false, visualizeContainers = false, visualizeClipRects = false, debugOverlayEnabled = false;
		// TODO: These need to maintain these values or bad things happen on other machines
		bool openGLCompatibilityMode = true, useDoubleBuffer = true;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 4, frameLimiter = 0;
	};
}

