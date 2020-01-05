#pragma once

#include "ui/renderable/primitive/Rectangle.h"
#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"

namespace AWGame
{
	class Player : public AW::Rectangle, public AW::IBodyListener
	{
		std::shared_ptr<AW::Body> body;

		bool down = false, left = false, right = false, up = false;
	public:
		Player();

		void onBindShaders();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(const double& deltaTime);

		void onKey(AWKey key, bool isPressed);

		// Inherited via IBodyListener
		virtual std::shared_ptr<Renderable> getRenderableBody() override;

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Body> getBodyObject() override;

		virtual void onBeginContact(std::unique_ptr<AW::ContactBundle> bundle);
		virtual void onEndContact(std::unique_ptr<AW::ContactBundle> bundle);
	};
}
