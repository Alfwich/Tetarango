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
		int width = 1024, height = 768, windowFlags = 0;
		ScreenModes mode = ScreenModes::Windowed;
		bool openGlWireframeMode = false, openGLCompatibilityMode = false, useDoubleBuffer = true, visualizeContainers = false, visualizeClipRects = false;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 4;
	};
}

