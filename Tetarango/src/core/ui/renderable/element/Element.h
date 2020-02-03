#pragma once

#include "engine/GameObject.h"
#include "ui/renderable/Renderable.h"
#include "engine/module/display/Texture.h"
#include "engine/module/display/TextureText.h"
#include "engine/module/display/RenderPackage.h"

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

		void onLoadResources() override;
		void onRegisterLuaHooks() override;
		void onBindShaders() override;

		virtual void setTexture(std::string key);
		virtual void setTexture(std::shared_ptr<Texture> texture);
		virtual std::shared_ptr<Rect> getTextureClipRect();
		bool hasTexture();
		std::shared_ptr<Texture> getTexture();
		virtual double getAlpha();

		void setMatchSizeToTexture(bool flag);
		bool getMatchSizeToTexture();

		void onLuaCallback(const std::string& func, LuaBoundObject* obj);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};
}
