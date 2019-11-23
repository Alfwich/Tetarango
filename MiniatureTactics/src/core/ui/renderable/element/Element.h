#pragma once

#include <vector>
#include <sstream>
#include <stdexcept>
#include <SDL.h>
#include "engine/ApplicationObject.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/Texture.h"
#include "engine/module/display/TextureText.h"
#include "engine/module/display/RenderPackage.h"
#include "util/Color.h"

namespace MT
{
	class ApplicationObject;

	class Element : public ApplicationObject, public Renderable
	{
	protected:
		std::shared_ptr<Texture> texture;
		std::string currentTextureName;

	public:
		Element();

		void setTexture(std::string key);
		void setTexture(std::shared_ptr<Texture> texture);
		virtual std::shared_ptr<Rect> getClipRect();
		bool hasTexture();
		std::shared_ptr<Texture> getTexture();
		virtual double getAlpha();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
