#include "Primitive.h"

namespace AW
{

	Primitive::Primitive()
	{
		renderType = RenderType::Primitive;
	}

	void Primitive::onBindShaders()
	{
		if (vertexShader == nullptr)
		{
			vertexShader = modules->shader->getShader("vertex-default");
		}

		if (fragmentShader == nullptr)
		{
			fragmentShader = modules->shader->getShader("fragment-texture");
		}

		if (clipRectVertexShader == nullptr)
		{
			clipRectVertexShader = modules->shader->getShader("vertex-default");
		}

		if (clipRectFragmentShader == nullptr)
		{
			clipRectFragmentShader = modules->shader->getShader("fragment-cliprect");
		}

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
