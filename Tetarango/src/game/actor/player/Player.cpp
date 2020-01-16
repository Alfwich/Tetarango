#include "Player.h"

#include "ui/physic/body/BodySensor.h"

#include "ui/renderable/physic/Box.h"
#include "ui/renderable/physic/Poly.h"

namespace
{
	const auto jumpImpulse = 20.0;
	const auto moveForce = 120.0;
	const auto maxLeftRightVelocity = 1.0;
	const auto maxJumps = 2;
	const auto playerStartTextureName = "player-start";
	const auto playerAnimationSetName = "player-animations";
}

namespace AWGame
{
	Player::Player()
	{
		layoutSpace = AW::LayoutSpace::World;
		setScreenSize(128, 128);
		GORegister(Player);
	}

	void Player::onLoadResources()
	{
		modules->texture->loadTexture("res/image/actor/player/player-start.png", playerStartTextureName);

		auto animationSet = std::make_shared<AW::AnimationSet>();
		{
			auto row = 0;
			auto col = 0;

			AW::RectI frameSize = {
				0,
				0,
				128,
				128
			};

			{
				auto anim = animationSet->startNewAnimation("default");
				anim->addGeneralFrames(col * frameSize.w, (row++) * frameSize.h, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("right");
				anim->addGeneralFrames(col * frameSize.w, (row++) * frameSize.h, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("left");
				anim->addGeneralFrames(col * frameSize.w, (row++) * frameSize.h, frameSize.w, frameSize.h, 1);
			}

			{
				auto anim = animationSet->startNewAnimation("duck");
				anim->addGeneralFrames(col * frameSize.w, (row++) * frameSize.h, frameSize.w, frameSize.h, 1);
			}
		}

		modules->animation->addAnimationSet(animationSet, playerAnimationSetName);

	}

	void Player::onInitialAttach()
	{
		AW::Animated::onInitialAttach();

		modules->input->keyboard->registerKeys({
			AWKey::UP,
			AWKey::DOWN,
			AWKey::LEFT,
			AWKey::RIGHT
			}, weak_from_this());

		setTexture(playerStartTextureName);
		setAnimationSet(playerAnimationSetName);
		setDefaultAnimationName("default");
		enableEnterFrame();
	}

	void Player::onCreateChildren()
	{
		body = std::make_shared<AW::Body>();
		body->name = "body";
		body->setFixedRotation(true);
		body->setDynamicBody();
		body->setFriction(0.2);
		add(body);

		const auto bodyCollider = std::make_shared<AW::Poly>();
		const auto offset = (AW::NumberHelper::PI * 2.0) / 16.0;
		const auto step = (AW::NumberHelper::PI * 2.0) / 8.0;
		for (auto i = 0; i < 8; ++i)
		{
			bodyCollider->addScreenPoint(std::cosf(offset + i * step) * 22.0, std::sinf(offset + i * step) * 24.0);
		}
		bodyCollider->centerBalancePoints();
		body->add(bodyCollider);

		const auto bodySensor = std::make_shared<AW::BodySensor>();
		bodySensor->setScreenWidth(44);
		bodySensor->setScreenHeight(48);
		body->add(bodySensor);
	}

	void Player::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>("body");
	}

	void Player::onEnterFrame(const double& deltaTime)
	{
		if (up && jumpsAllowed > 0)
		{
			body->applyImpulse(0.0, 1.0, jumpImpulse);
			--jumpsAllowed;
			up = false;
		}
		if (left && body->getVelocity().x > -maxLeftRightVelocity) body->applyForce(-1.0, 0.0, jumpsAllowed == maxJumps ? moveForce : moveForce / 10);
		if (right && body->getVelocity().x < maxLeftRightVelocity) body->applyForce(1.0, 0.0, jumpsAllowed == maxJumps ? moveForce : moveForce / 10);
		if (down) body->applyForce(0.0, -1.0, moveForce);

		if (up) play("default");
		else if (left) play("left");
		else if (right) play("right");
		else if (down) play("duck");
	}

	void Player::onKey(AWKey key, bool isPressed)
	{
		if (key == AWKey::UP) up = isPressed;
		if (key == AWKey::DOWN) down = isPressed;
		if (key == AWKey::LEFT) left = isPressed;
		if (key == AWKey::RIGHT) right = isPressed;
	}

	std::shared_ptr<AW::Renderable> Player::getRenderableBody()
	{
		return std::dynamic_pointer_cast<AW::Renderable>(shared_from_this());
	}

	std::shared_ptr<AW::Body> Player::getBodyObject()
	{
		return body;
	}

	void Player::onBeginContact(std::unique_ptr<AW::ContactBundle> bundle)
	{
		jumpsAllowed = maxJumps;
	}

	void Player::onEndContact(std::unique_ptr<AW::ContactBundle> bundle)
	{
	}
}
