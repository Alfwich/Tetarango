#pragma once

#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"
#include "ui/renderable/element/Element.h"
#include "ui/physic/body/IBodyFixture.h"

namespace AW
{
	class Box : public AW::Element, public AW::IBodyListener, public AW::IBodyFixture
	{
		std::shared_ptr<AW::Body> body;
		bool dynamic = false;

	public:
		Box();

		void onBindShaders();

		void onCreateChildren();
		void onChildrenHydrated();

		void setDynamic(bool flag);
		bool getDynamic();

		// Inherited via IBodyListener
		virtual std::shared_ptr<Renderable> getRenderableBody() override;
		virtual std::shared_ptr<AW::Body> getBodyObject() override;

		// Inherited via IBodyFixture
		virtual const AW::BodyType getBodyType() const override;

		virtual const AWVec2<float> getBodyWorldSize() override;
	};
}
