#pragma once

#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"
#include "ui/renderable/element/Element.h"

namespace AWGame
{
	class Box : public AW::Element, public AW::IBodyListener
	{
		std::shared_ptr<AW::Body> body;
		bool dynamic = true;

	public:
		Box();

		void onBindShaders();
		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();

		void setDynamic(bool flag);
		bool getDynamic();

		// Inherited via IBodyListener
		virtual std::shared_ptr<Renderable> getRenderableBody() override;

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Body> getBodyObject() override;
	};
}
