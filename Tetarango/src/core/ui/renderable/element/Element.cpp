#include "Element.h"

namespace
{
	const auto elementAutoSizeToTextureParam = "element-a-s-t-t";
}

namespace AW
{

	Element::Element()
	{
		renderMode = RenderMode::Element;
		GORegister(Element);
	}

	void Element::onBindLuaHooks()
	{
		GameObject::onBindLuaHooks();
		Renderable::onBindLuaHooks(modules->lua, sharedPtr());
		registerBoundLuaMethod("setTexture");
	}

	void Element::onLoadResources()
	{
		modules->shader->registerShaderComposition({ "f-texture", "f-color" }, "element");
	}

	void Element::onBindShaders()
	{
		if (fragmentShader == nullptr)
		{
			fragmentShader = modules->shader->getShader({ "element" });
		}
	}

	std::shared_ptr<Rect> Element::getTextureClipRect()
	{
		return nullptr;
	}

	void Element::setTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;

		if (getMatchSizeToTexture())
		{
			setScreenSize(texture->getWidth(), texture->getHeight());
		}
	}

	void Element::setTexture(std::string key)
	{
		if (modules->texture->hasTexture(key))
		{
			currentTextureName = key;
			setTexture(modules->texture->getTexture(key));
		}
	}

	bool Element::hasTexture()
	{
		return (bool)texture;
	}

	std::shared_ptr<Texture> Element::getTexture()
	{
		return texture;
	}

	double Element::getAlpha()
	{
		const auto parentPtr = parent.lock();
		const auto renderable = std::dynamic_pointer_cast<Renderable>(parentPtr);

		if (renderable != nullptr)
		{
			return Renderable::getAlpha() * renderable->getAlpha();
		}

		return Renderable::getAlpha();
	}

	void Element::setMatchSizeToTexture(bool flag)
	{
		serializationClient->setBool(elementAutoSizeToTextureParam, flag);

		if (flag && texture != nullptr)
		{
			setScreenSize(texture->getWidth(), texture->getHeight());
		}
	}

	bool Element::getMatchSizeToTexture()
	{
		return serializationClient->getBool(elementAutoSizeToTextureParam, false);
	}

	void Element::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		GameObject::onLuaCallback(func, obj);
		Renderable::onLuaCallback(func, obj);

		if (func == "setTexture" && obj->args.size() == 1) setTexture(obj->args[0]);
	}

	std::shared_ptr<SerializationClient> Element::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__element__", hint);
		currentTextureName = client->serializeString("c-t-n", currentTextureName);

		Renderable::doManualSerialize(hint, client);

		if (texture == nullptr && !currentTextureName.empty())
		{
			setTexture(currentTextureName);
		}

		return GameObject::doSerialize(hint);
	}
}
