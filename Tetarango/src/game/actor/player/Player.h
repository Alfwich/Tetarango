#pragma once

#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"
#include "ui/renderable/element/Animated.h"

namespace AWGame
{
	class Player : public AW::Animated, public AW::IBodyListener
	{
		std::shared_ptr<AW::Body> body;

		bool down = false, left = false, right = false, up = false;

		int contacts = 0;
		int airJumpsAllowed = 0;

	public:
		Player();

		void onLoadResources();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void onEnterFrame(const double& deltaTime);

		void onKey(AWKey key, bool isPressed);


		// Inherited via IBodyListener
		virtual std::shared_ptr<Renderable> getRenderableBody() override;
		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Body> getBodyObject() override;

		virtual void onBeginContact(std::unique_ptr<AW::ContactBundle> bundle) override;
		virtual void onEndContact(std::unique_ptr<AW::ContactBundle> bundle) override;
	};
}
