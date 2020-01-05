#pragma once

#include "ui/renderable/primitive/Polygon.h"
#include "ui/physic/body/Body.h"
#include "ui/physic/body/IBodyListener.h"

namespace AWGame
{
	class Poly : public AW::Polygon, public AW::IBodyListener
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
		const std::vector<AWVec2<float>> getBodyWorldPoints();

		// Inherited via IBodyListener
		virtual std::shared_ptr<AW::Body> getBodyObject() override;
	};
}
