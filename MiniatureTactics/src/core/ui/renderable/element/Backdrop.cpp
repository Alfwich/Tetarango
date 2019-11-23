#include "Backdrop.h"

namespace
{
	// Roughly the maximum that OpenGL can render(nicely) for texture wrapping
	const double effectiveInfinity = 50000;
}

namespace MT
{

	Backdrop::Backdrop()
	{
		renderType = RenderType::Backdrop;
		disableCulling = true;
		zIndex = -20;

		enableSerialization<Backdrop>();
	}

	void Backdrop::setSizeToEffectiveInfinity()
	{
		this->setSize(effectiveInfinity, effectiveInfinity);
	}


}
