#include "Element.h"

namespace MT
{

	Element::Element()
	{
		renderType = RenderType::Element;
		enableSerialization<Element>();
	}

	std::shared_ptr<Rect> Element::getTextureClipRect()
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
		currentTextureName = client->serializeString("cTexName", currentTextureName);

		Renderable::doManualSerialize(hint, client);

		if (texture == nullptr && !currentTextureName.empty())
		{
			setTexture(currentTextureName);
		}

		return ApplicationObject::doSerialize(hint);
	}
}
