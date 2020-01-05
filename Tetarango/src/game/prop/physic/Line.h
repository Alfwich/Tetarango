#pragma once

#include "ui/renderable/primitive/Primitive.h"
#include "ui/physic/body/Body.h"

namespace AWGame
{
	class Line : public AW::Primitive, public AW::IBodyListener
	{
		std::shared_ptr<AW::Body> body;

	public:
		Line();

		void onCreateChildren();
		void onChildrenHydrated();

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Renderable> getRenderableBody() override;
		virtual std::shared_ptr<AW::Body> getBodyObject() override;
	};
}
