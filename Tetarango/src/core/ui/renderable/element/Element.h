#pragma once

#include <vector>
#include <sstream>
#include <stdexcept>
#include <SDL.h>
#include "engine/GameObject.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/Texture.h"
#include "engine/module/display/TextureText.h"
#include "engine/module/display/RenderPackage.h"
#include "util/Color.h"

namespace AW
{
	class GameObject;

	class Element : public GameObject, public Renderable
	{
	protected:
		std::shared_ptr<Texture> texture;
		std::string currentTextureName;

	public:
		Element();

		void onBindShaders();

		virtual void setTexture(std::string key);
		virtual void setTexture(std::shared_ptr<Texture> texture);
		virtual std::shared_ptr<Rect> getTextureClipRect();
		bool hasTexture();
		std::shared_ptr<Texture> getTexture();
		virtual double getAlpha();

		void setMatchSizeToTexture(bool flag);
		bool getMatchSizeToTexture();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
