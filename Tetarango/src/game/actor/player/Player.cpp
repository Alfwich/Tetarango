#include "Player.h"

#include "ui/physic/body/BodySensor.h"

namespace
{
	const auto impulse = 10000;
}

namespace AWGame
{
	Player::Player()
	{
		setColor(AW::Color(255, 64, 32));
		setSize(200.0, 200.0);
		registerGameObject<Player>(__FUNCTION__);
	}

	void Player::onBindShaders()
	{
		fragmentShader = modules->shader->getShader({ "f-color", "f-circle" });
		fragmentShader->setFloatIUParam("fCircleEdge", 0.1);
	}

	void Player::onInitialAttach()
	{
		modules->input->keyboard->registerKeys({
			AWKey::UP,
			AWKey::DOWN,
			AWKey::LEFT,
			AWKey::RIGHT
			}, weak_from_this());

		enableEnterFrame();
	}

	void Player::onCreateChildren()
	{
		body = std::make_shared<AW::Body>();
		body->name = "body";
		body->setDensity(5.0);
		body->setBodyType(AW::BodyType::Circle);
		body->setDynamicBody();
		body->setDensity(5.0);
		add(body);

		const auto bodySensor = std::make_shared<AW::BodySensor>();
		bodySensor->width = getWidth();
		bodySensor->height = getHeight();
		body->add(bodySensor);

		const auto oDot = std::make_shared<AW::Rectangle>();
		oDot->setColor(AW::Color::random());
		oDot->setSize(50, 50);
		oDot->setFragmentShader(modules->shader->getShader({ "f-color", "f-circle" }));
		oDot->centerAlignWithin(this, 0.0, 75.0);
		add(oDot);
	}

	void Player::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>("body");
	}

	void Player::onEnterFrame(const double& deltaTime)
	{
		if (up) body->applyForce(0.0, 1.0, impulse);
		if (down) body->applyForce(0.0, -1.0, impulse);
		if (left) body->applyForce(-1.0, 0.0, impulse);
		if (right) body->applyForce(1.0, 0.0, impulse);
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

	void Player::BeginContact(b2Contact * contact)
	{
		std::cout << "Player Contacted With Something" << std::endl;
	}

	void Player::EndContact(b2Contact * contact)
	{
		std::cout << "Player Ended Contacted With Something" << std::endl;
	}
}
