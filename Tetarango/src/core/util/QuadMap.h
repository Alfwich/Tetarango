#pragma once

#include <list>
#include <vector>
#include "Rect.h"

#include "engine/module/collision/ICollidable.h"

namespace MT
{


	class QuadMap
	{
		unsigned int splitThreshold = 10;
		double minDim = 50;
		bool hasSplit = false;

		MT::Rect dim;

		std::shared_ptr<QuadMap> nw, ne, sw, se;
		std::list<std::shared_ptr<ICollidable>> children;

		bool insertRecursive(std::shared_ptr<ICollidable>& col);
		void findRecursive(const std::shared_ptr<ICollidable>& col, double radius, std::vector<std::shared_ptr<ICollidable>>& results);
		bool inMap(const std::shared_ptr<ICollidable>& collidable, double radius = 0.0);
		void clearRecursive();

	public:
		QuadMap(double x, double y, double w, double h);

		bool insert(std::shared_ptr<ICollidable> rend);

		void find(const std::shared_ptr<ICollidable>& col, double radius, std::vector<std::shared_ptr<ICollidable>>& results);
		void clear();
	};

}
