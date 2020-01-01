#include "Player.h"

namespace
{
	const auto impulse = 2000;
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
			SDL_SCANCODE_UP,
			SDL_SCANCODE_DOWN,
			SDL_SCANCODE_LEFT,
			SDL_SCANCODE_RIGHT
			}, weak_from_this());

		enableEnterFrame();
	}

	void Player::onCreateChildren()
	{
		body = std::make_shared<AW::Body>();
		body->name = "body";
		body->setBodyType(AW::BodyType::Circle);
		body->setDynamicBody();
		add(body);

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
		if (up) body->applyForce(0.0, 1.0, impulse * (deltaTime / 1000.0));
		if (down) body->applyForce(0.0, -1.0, impulse * (deltaTime / 1000.0));
		if (left) body->applyForce(-1.0, 0.0, impulse * (deltaTime / 1000.0));
		if (right) body->applyForce(1.0, 0.0, impulse * (deltaTime / 1000.0));
	}

	void Player::onKey(SDL_Scancode key, bool isPressed)
	{
		if (key == SDL_SCANCODE_UP) up = isPressed;
		if (key == SDL_SCANCODE_DOWN) down = isPressed;
		if (key == SDL_SCANCODE_LEFT) left = isPressed;
		if (key == SDL_SCANCODE_RIGHT) right = isPressed;
	}
}
