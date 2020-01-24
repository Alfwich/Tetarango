#include "Ground.h"

#include "ui/renderable/physic/Chain.h"
#include "ui/renderable/element/Polygon.h"

namespace AWGame
{
	Ground::Ground()
	{
		GORegister(Ground);
	}

	void Ground::onCreateChildren()
	{
		const auto ground = std::make_shared<AW::Chain>();
		ground->visible = false;
		ground->setAlpha(0.5);
		ground->setDynamic(false);
		for (auto i = -5; i < 6; ++i)
		{
			if (i == -5 || i == 5)
				ground->addWorldPoint(i * 0.5, getWorldY());
			else
				ground->addWorldPoint(i * 0.5, AW::NumberHelper::random(-0.15, 0.15) + getWorldY());
		}
		ground->centerBalancePoints();
		ground->shiftWorldPoints(getWorldX(), getWorldY());
		add(ground);

		const auto groundCover = std::make_shared<AW::Polygon>();
		const auto pts = ground->getScreenPoints();
		groundCover->addScreenPoint(pts[0].x - getScreenX(), 2000 - getScreenY());
		for (const auto pt : pts)
		{
			groundCover->addScreenPoint(pt.x - getScreenX(), pt.y - getScreenY());
		}
		groundCover->addScreenPoint(pts.back().x - getScreenX(), 2000 - getScreenY());
		groundCover->setTexture("noise-solid-512");
		groundCover->setRepeatAmount(5.0);
		add(groundCover);
	}
}
