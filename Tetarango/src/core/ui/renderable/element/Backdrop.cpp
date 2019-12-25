#include "Backdrop.h"

namespace
{
	// Roughly the maximum that OpenGL can render(nicely) for texture wrapping
	const double effectiveInfinity = 50000;
}

namespace AW
{
	Backdrop::Backdrop()
	{
		renderTextureMode = RenderTextureMode::LinearWrapping;
		disableCulling = true;

		registerGameObject<Backdrop>();
	}

	void Backdrop::setSizeToEffectiveInfinity()
	{
		this->setSize(effectiveInfinity, effectiveInfinity);
	}
}
