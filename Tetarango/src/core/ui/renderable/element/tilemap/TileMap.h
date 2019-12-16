#pragma once

#include <memory>
#include "ui/renderable/element/Element.h"

namespace AWCore
{

	class TileMap : public Element
	{
		int tileW = 1, tileH = 1;

	public:
		TileMap();

		int getTileWidth();
		int getTileHeight();
		void setTileSize(int w, int h);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};


}
