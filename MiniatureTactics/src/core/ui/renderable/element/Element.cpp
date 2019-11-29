#include "Element.h"

namespace MT
{

	Element::Element()
	{
		renderType = RenderType::Element;
		setColor(0xff, 0xff, 0xff);
		enableSerialization<Element>();
	}

	std::shared_ptr<Rect> Element::getClipRect()
	{
		return nullptr;
	}

	void Element::setTexture(std::shared_ptr<Texture> texture)
	{
		this->texture = texture;
	}

	void Element::setTexture(std::string key)
	{
		currentTextureName = key;
		setTexture(modules->texture->getTexture(key));
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

	std::shared_ptr<SerializationClient> Element::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__element__", hint);
		colorModulation.r = client->serializeInt("cm.r", colorModulation.r);
		colorModulation.g = client->serializeInt("cm.g", colorModulation.g);
		colorModulation.b = client->serializeInt("cm.b", colorModulation.b);
		colorModulation.a = client->serializeInt("cm.a", colorModulation.a);
		currentTextureName = client->serializeString("cTexName", currentTextureName);

		Renderable::doManualSerialize(hint, client);

		if (texture == nullptr && !currentTextureName.empty())
		{
			setTexture(currentTextureName);
		}

		return ApplicationObject::doSerialize(hint);
	}
}
