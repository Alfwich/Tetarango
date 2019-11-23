#include "Primitive.h"

namespace MT
{

	Primitive::Primitive()
	{
		renderType = RenderType::Primitive;
		setColor(0xff, 0xff, 0xff, 0xff);
	}

	GLuint Primitive::getTextureId()
	{
		const auto tex = modules->texture->getTexture("__solid__");

		if (tex != nullptr)
		{
			return tex->openGlTextureId();
		}

		return 0;
	}

	std::shared_ptr<SerializationClient> Primitive::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__primitive__", hint);

		Renderable::doManualSerialize(hint, client);

		return ApplicationObject::doSerialize(hint);
	}

	double Primitive::getAlpha()
	{
		const auto parentPtr = parent.lock();
		const auto renderable = std::dynamic_pointer_cast<Renderable>(parentPtr);

		if (renderable != nullptr)
		{
			return Renderable::getAlpha() * renderable->getAlpha();
		}

		return Renderable::getAlpha();
	}
}
