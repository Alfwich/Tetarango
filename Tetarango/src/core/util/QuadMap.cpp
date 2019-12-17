#include "QuadMap.h"


namespace AW
{
	QuadMap::QuadMap(double x, double y, double w, double h)
	{
		dim.x = x;
		dim.y = y;
		dim.w = w;
		dim.h = h;
	}

	void QuadMap::find(const std::shared_ptr<ICollidable>& col, double radius, std::vector<std::shared_ptr<ICollidable>>& results)
	{
		findRecursive(col, radius, results);
	}

	bool QuadMap::insertRecursive(std::shared_ptr<ICollidable>& col)
	{
		if (hasSplit)
		{
			return nw->insertRecursive(col) ||
				ne->insertRecursive(col) ||
				sw->insertRecursive(col) ||
				se->insertRecursive(col);
		}
		else if (inMap(col))
		{
			if (children.size() + 1 == splitThreshold && dim.w > minDim)
			{
				hasSplit = true;
				if (nw == nullptr)
				{
					const auto halfWidth = dim.w / 2.0;
					const auto halfHeight = dim.h / 2.0;
					nw = std::make_shared<QuadMap>(dim.x, dim.y, halfWidth, halfHeight);
					ne = std::make_shared<QuadMap>(dim.x + halfWidth, dim.y, halfWidth, halfHeight);
					sw = std::make_shared<QuadMap>(dim.x, dim.y + halfHeight, halfWidth, halfHeight);
					se = std::make_shared<QuadMap>(dim.x + halfWidth, dim.y + halfHeight, halfWidth, halfHeight);
				}

				for (auto child : children)
				{
					insertRecursive(child);
				}

				children.clear();

				return insertRecursive(col);
			}
			else
			{
				children.push_back(col);
			}

			return true;
		}

		return false;
	}

	void QuadMap::findRecursive(const std::shared_ptr<ICollidable>& col, double radius, std::vector<std::shared_ptr<ICollidable>>& results)
	{
		if (hasSplit)
		{
			if (nw->inMap(col, radius))
			{
				nw->findRecursive(col, radius, results);
			}

			if (ne->inMap(col, radius))
			{
				ne->findRecursive(col, radius, results);
			}

			if (sw->inMap(col, radius))
			{

				sw->findRecursive(col, radius, results);
			}

			if (se->inMap(col, radius))
			{
				se->findRecursive(col, radius, results);
			}
		}
		else
		{
			const auto r2 = col->getCollisionRect();
			for (const auto child : children)
			{
				const auto r = child->getCollisionRect();
				const auto d = std::sqrt(std::pow(r->x - r2->x, 2) + std::pow(r->y - r2->y, 2));
				if (child != col && d <= radius)
				{
					results.push_back(child);
				}
			}
		}
	}

	bool QuadMap::inMap(const std::shared_ptr<ICollidable>& col, double radius)
	{
		return col->getCollisionRect()->intersects(dim, radius);
	}

	bool QuadMap::insert(std::shared_ptr<ICollidable> col)
	{
		const auto r = col->getCollisionRect();

		if (r->w > 0 && r->h > 0)
		{
			return insertRecursive(col);
		}

		return false;
	}

	void QuadMap::clearRecursive()
	{
		children.clear();
		if (hasSplit)
		{
			nw->clearRecursive();
			ne->clearRecursive();
			se->clearRecursive();
			sw->clearRecursive();
		}
		hasSplit = false;
	}

	void QuadMap::clear()
	{
		clearRecursive();
	}

}
