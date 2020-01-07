#pragma once

#include "ui/renderable/primitive/Polygon.h"
#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"
#include "ui/physic/body/IBodyFixture.h"

namespace AWGame
{
	class Poly : public AW::Polygon, public AW::IBodyListener, public AW::IBodyFixture
	{
		std::shared_ptr<AW::Body> body;

	public:
		Poly();

		void onCreateChildren();
		void onChildrenHydrated();

		void setDynamic(bool flag);
		bool getDynamic();

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Renderable> getRenderableBody() override;

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Body> getBodyObject() override;

		// Inherited via IBodyFixture
		virtual const AW::BodyType getBodyType() const override;
	};
}
