#include "Primitive.h"

namespace AW
{

	Primitive::Primitive()
	{
		renderMode = RenderMode::Primitive;
	}

	std::shared_ptr<SerializationClient> Primitive::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__primitive__", hint);

		Renderable::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
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
