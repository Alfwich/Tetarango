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
		int width = 1280, height = 720, windowFlags = 0;
		double deviceScaling = 1.0;
		ScreenModes mode = ScreenModes::Windowed;
		VsyncModes vMode = VsyncModes::Adaptive;
		bool openGlWireframeMode = false, visualizeContainers = false, visualizeClipRects = false, debugOverlayEnabled = false, visualizePhysic = false;
		// TODO: These need to maintain these values or bad things happen on other machines
		bool openGLCompatibilityMode = true, useDoubleBuffer = true;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 0, frameLimiter = 0;
	};
}

