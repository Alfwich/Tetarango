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
		renderType = RenderType::Backdrop;
		disableCulling = true;

		enableSerialization<Backdrop>();
	}

	void Backdrop::setSizeToEffectiveInfinity()
	{
		this->setSize(effectiveInfinity, effectiveInfinity);
	}
}
