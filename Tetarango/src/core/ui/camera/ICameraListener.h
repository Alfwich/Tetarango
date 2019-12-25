#pragma once

namespace AW
{
	class ICameraListener
	{
	public:
		virtual void onCameraUpdate(int cameraId) { onCameraUpdate(); }
		virtual void onCameraUpdate() { /* NO-OP */ }
	};
}
