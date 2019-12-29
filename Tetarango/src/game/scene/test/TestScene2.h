#pragma once

#include <vector>
#include "ui/scene/Scene.h"
#include "gui/camera/GameCamera.h"
#include "gui/IGuiListener.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Rectangle.h"
#include "ui/renderable/container/Container.h"
#include "engine/module/physic/RigidBody.h"
#include "box2d/b2_world.h"
#include "box2d/box2d.h"

namespace AWGame
{
	class dBlock : public AW::RigidBody, public AW::Element
	{
		int aliveTicks = 600;
	public:
		void onInitialAttach()
		{
			setTexture("prop-blocks");
		}

		void onBindShaders()
		{
			fragmentShader = modules->shader->getShader({ "block" });

			fragmentShader->setFloatIUParam("clipX", 32.0);
			fragmentShader->setFloatIUParam("clipY", 0.0);
			fragmentShader->setFloatIUParam("clipWidth", 64.0);
			fragmentShader->setFloatIUParam("clipHeight", 64.0);

			fragmentShader->setFloatIUParam("blockBorderSize", 2.0);
			fragmentShader->setFloatIUParam("blockEffect", 0.5);
			fragmentShader->setFloatIUParam("blockEffectP", 0.25);
			fragmentShader->setFloatIUParam("blockEffectG", 0.4);
			fragmentShader->setFloatIUParam("blockCenterFill", 1.0);

			fragmentShader->setFloatIUParam("fScanlineRetroAmount", 0.25);
		}

		b2BodyDef onDefineBody()
		{
			auto def = RigidBody::onDefineBody();

			def.type = b2_dynamicBody;
			def.position.Set(AW::NumberHelper::random(-30.0, 30.0), AW::NumberHelper::random(120.0, 180.0));

			return def;
		}

		b2FixtureDef onDefineFixture()
		{
			auto def = RigidBody::onDefineFixture();

			const auto size = AW::NumberHelper::random(1.0, 2.0);
			shape.SetAsBox(size, size);
			setSize(size * 20.0, size * 20.0);
			setAlpha(0.0);

			def.shape = &shape;
			def.density = 1.0;
			def.friction = 0.8f;

			return def;
		}

		void onPhysicUpdate(const b2Body* body)
		{
			if (--aliveTicks <= 0)
			{
				removeFromParent();
			}

			if (aliveTicks > 60)
			{
				setAlpha(getAlpha() + 1 / 60.0);
			}
			else
			{
				setAlpha(getAlpha() - 1 / 60.0);
			}

			const auto xOffset = modules->screen->getWidth() / 2.0;
			const auto yOffset = modules->screen->getHeight() / 2.0;
			const auto pos = body->GetPosition();
			setPosition(pos.x * 10.0 + xOffset, pos.y * -10.0 + yOffset);

			const auto rotation = body->GetAngle() * (180 / AW::NumberHelper::PI);
			setRotation(-rotation);
		};
	};

	class sBlock : public AW::RigidBody, public AW::Rectangle
	{
	public:
		b2BodyDef onDefineBody()
		{
			auto def = RigidBody::onDefineBody();

			def.position.Set(0.0, -10.0);

			return def;
		}

		b2FixtureDef onDefineFixture()
		{
			auto def = RigidBody::onDefineFixture();

			shape.SetAsBox(100.0, 10.0);
			setSize(2000.0, 200.0);
			setColor(AW::Color(64, 64, 64));

			def.shape = &shape;
			def.density = 1.0;
			def.friction = 1.0;

			return def;
		}

		void onPhysicUpdate(const b2Body* body)
		{
			const auto xOffset = modules->screen->getWidth() / 2.0;
			const auto yOffset = modules->screen->getHeight() / 2.0;
			const auto pos = body->GetPosition();
			setPosition(pos.x * 10.0 + xOffset, pos.y * -10.0 + yOffset);

			const auto rotation = body->GetAngle() * (180 / AW::NumberHelper::PI);
			setRotation(-rotation);
		};
	};

	class TestScene2 : public AW::Scene, public AW::ICameraListener, public IGuiListener
	{

		std::shared_ptr<GameCamera> camera;

		std::shared_ptr<AW::Container> contentContainer;
		std::shared_ptr<AW::Renderable> obj1, obj2;

	public:
		TestScene2();

		void onLoadResources();

		void onInitialAttach();
		void onAttach();
		void onCreateChildren();
		void onLayoutChildren();
		void onChildrenHydrated();

		void onTimeoutCalled();

		void onEnterFrame(const double& deltaTime);
		void onKeyPressed(SDL_Scancode key);
		void onKey(SDL_Scancode key, bool isPressed);

		void onScrollBarScroll(int id, double pos);
		void onCameraUpdate();
	};

}
