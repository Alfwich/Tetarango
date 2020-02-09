#include "Primitive.h"

namespace AW
{

	Primitive::Primitive()
	{
		renderMode = RenderMode::Primitive;
	}

	void Primitive::onBindLuaHooks()
	{
		GameObject::onBindLuaHooks();
		Renderable::onBindLuaHooks(modules->lua, sharedPtr());
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

	void Primitive::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		GameObject::onLuaCallback(func, obj);
		Renderable::onLuaCallback(func, obj);
	}
}
