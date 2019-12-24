#include "TileMap.h"

namespace AW
{

	TileMap::TileMap()
	{
		renderType = RenderType::TileMap;
		registerGameObject<TileMap>();
	}

	int TileMap::getTileWidth()
	{
		return tileW;
	}

	int TileMap::getTileHeight()
	{
		return tileH;
	}

	void TileMap::setTileSize(int w, int h)
	{
		tileW = w;
		tileH = h;
	}

	std::shared_ptr<SerializationClient> TileMap::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__tile_map__", hint);
		tileW = client->serializeInt("tileW", tileW);
		tileH = client->serializeInt("tileH", tileH);

		return Element::doSerialize(hint);
	}

}
