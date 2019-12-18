#pragma once

#include "GL/glew.h"
#include "engine/ApplicationObject.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/RenderPackage.h"
#include "util/Color.h"

namespace AW
{

	class Primitive : public ApplicationObject, public Renderable
	{
	public:
		Primitive();

		void onBindShaders();

		virtual void preUpdateRender(Rect* renderRect, RenderPackage* renderPackage) { /* NO-OP */ };
		virtual void preRender(Rect* renderRect, RenderPackage* renderPackage) { /* NO-OP */ };

		virtual GLuint getTextureId();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual double getAlpha();
	};

}
