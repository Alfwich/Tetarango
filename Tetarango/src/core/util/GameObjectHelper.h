#pragma once

#include "engine/GameObject.h"

namespace AW
{
	class GameObjectHelper
	{
		static void calcScreenPosition(RenderPackage* renderPackage)
		{
			const auto& rend = renderPackage->obj;
			const auto scale = renderPackage->zoom * rend->getScale();
			const auto rect = rend->getRect() * Rect(renderPackage->zoom, renderPackage->zoom, scale, scale);
			const auto rotation = renderPackage->rotation;

			auto rectMiddleX = (rect.x - (rend->getScreenWidth() / 2.0) * scale) + renderPackage->xOffset;
			auto rectMiddleY = (rect.y - (rend->getScreenHeight() / 2.0) * scale) + renderPackage->yOffset;

			double originW = renderPackage->computed.w;
			double originH = renderPackage->computed.h;
			renderPackage->computed.w = rect.w;
			renderPackage->computed.h = rect.h;

			if (rend->rotateInParentSpace)
			{
				double rotationRad = renderPackage->rotation * AW::NumberHelper::degToRad;
				double newX = renderPackage->computed.x + rectMiddleX;
				double newY = renderPackage->computed.y + rectMiddleY;
				double oX = renderPackage->computed.x + (originW / 2.0) - (renderPackage->computed.w / 2.0);
				double oY = renderPackage->computed.y + (originH / 2.0) - (renderPackage->computed.h / 2.0);
				double cX = newX - oX;
				double cY = newY - oY;
				double xP = cX * std::cos(rotationRad) - cY * std::sin(rotationRad);
				double yP = cY * std::cos(rotationRad) + cX * std::sin(rotationRad);

				renderPackage->computed.x = (xP + oX);
				renderPackage->computed.y = (yP + oY);
			}
			else
			{
				renderPackage->computed.x += rect.x - rect.w / 2.0;
				renderPackage->computed.y += rect.y - rect.h / 2.0;
			}

			renderPackage->zoom *= rend->getScale();
			renderPackage->rotation += rend->getScreenRotation();
			renderPackage->alpha *= rend->getAlpha();

			renderPackage->world.x = renderPackage->world.x - (originW / 2.0) + rend->getScreenX();
			renderPackage->world.y = renderPackage->world.y - (originH / 2.0) + rend->getScreenY();
			renderPackage->world.w = rend->getScreenWidth();
			renderPackage->world.h = rend->getScreenHeight();
		}
	public:

		static Rect calcScreenRect(const std::shared_ptr<Renderable>& rend)
		{
			const auto goPtr = std::dynamic_pointer_cast<GameObject>(rend);
			if (goPtr != nullptr)
			{
				return calcScreenRect(goPtr);
			}

			return Rect();
		}

		static Rect calcScreenRect(const std::shared_ptr<GameObject>& obj)
		{
			std::vector<std::shared_ptr<GameObject>> objPath { obj };
			std::shared_ptr<GameObject> parentPtr = obj->getParent().lock();

			while ((parentPtr != nullptr) && !parentPtr->isRootElement())
			{
				objPath.push_back(parentPtr);
				parentPtr = parentPtr->getParent().lock();
			}

			RenderPackage pkg;

			while (!objPath.empty())
			{
				const auto renderablePtr = std::dynamic_pointer_cast<Renderable>(objPath.back());
				objPath.pop_back();

				if (renderablePtr != nullptr)
				{
					pkg.obj = renderablePtr;
					calcScreenPosition(&pkg);
				}
			}

			return pkg.computed;
		}

	};
}
