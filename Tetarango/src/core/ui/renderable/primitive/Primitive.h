#pragma once

#include "GL/glew.h"
#include "engine/GameObject.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/RenderPackage.h"

namespace AW
{

	class Primitive : public GameObject, public Renderable
	{
	public:
		Primitive();

		virtual void preUpdateRender(Rect* renderRect, RenderPackage* renderPackage) { /* NO-OP */ };
		virtual void preRender(Rect* renderRect, RenderPackage* renderPackage) { /* NO-OP */ };

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual double getAlpha();
	};

}
