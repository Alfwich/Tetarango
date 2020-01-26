#include "GroundChunk.h"

#include "ui/renderable/physic/Chain.h"
#include "ui/renderable/element/Polygon.h"

namespace
{
	const auto groundChunkTexture = "ground-chunk-texture";
}

namespace AWGame
{
	GroundChunk::GroundChunk(double chunkSize)
		: chunkSize(chunkSize)
	{
		GORegister(GroundChunk);
	}

	void GroundChunk::onLoadResources()
	{
		modules->texture->loadTexture("res/image/prop/environment/chunk-dirt.png", groundChunkTexture);
	}

	void GroundChunk::onCreateChildren()
	{
		const auto groundCollider = std::make_shared<AW::Chain>();
		groundCollider->visible = false;
		groundCollider->setDynamic(false);
		const auto segmentsToCreate = AW::Renderable::screenToWorldPosition(chunkSize);
		for (auto i = -(segmentsToCreate / 2.0); i < ((segmentsToCreate / 2.0) + 1.0); i += 1.0)
		{
			groundCollider->addWorldPoint(i * 1.0, AW::NumberHelper::random(-0.15, 0.15) + getWorldY());
		}
		groundCollider->centerBalancePoints();
		groundCollider->shiftWorldPoints(getWorldX(), getWorldY());
		add(groundCollider);

		const auto groundCover = std::make_shared<AW::Polygon>();
		const auto pts = groundCollider->getScreenPoints();
		groundCover->addScreenPoint(pts[0].x - getScreenX(), chunkSize - getScreenY());
		for (const auto pt : pts)
		{
			groundCover->addScreenPoint(pt.x - getScreenX(), pt.y - getScreenY());
		}
		groundCover->addScreenPoint(pts.back().x - getScreenX(), chunkSize - getScreenY());
		groundCover->setTexture(groundChunkTexture);
		groundCover->setRepeatAmount(AW::Renderable::screenToWorldPosition(chunkSize));
		add(groundCover);
	}
}
