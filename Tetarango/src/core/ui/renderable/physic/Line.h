#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyFixture.h"

namespace AW
{
	class Line : public AW::Primitive, public AW::IBodyListener, public AW::IBodyFixture
	{
		std::shared_ptr<AW::Body> body;

	public:
		Line();

		void onCreateChildren();
		void onChildrenHydrated();

		void setDynamic(bool flag);
		bool getDynamic();

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Renderable> getRenderableBody() override;
		virtual std::shared_ptr<AW::Body> getBodyObject() override;

		// Inherited via IBodyFixture
		virtual const AW::BodyType getBodyType() const override;
	};
}
