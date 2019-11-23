#pragma once
namespace MT
{
	struct ScreenConfig
	{
		int width = 640, height = 480, windowFlags = 0;
		bool isFullscreen = false, openGlWireframeMode = false, useForwardCompatible = true, useMSAA = true;
		int openGLMajorVersion = 3, openGLMinorVersion = 1, msaaSamples = 8;
	};
}

