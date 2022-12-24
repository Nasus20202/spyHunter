#include "map.h"

Map::Map(const int mapWidth, const int mapHeight)
{
	this->map = new mapTile[mapHeight * mapWidth];
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
}

Map::~Map()
{
	delete[] map;
}

mapTile* Map::GetMap()
{
	return map;
}


mapTile Map::GetMapTile(const int x, const int y)
{
	int id = y * mapWidth + x;
	if (id < 0 || id >= mapWidth * mapHeight)
		return mapTile::grass;
	return map[id];
}

void Map::SetMapTile(const int x, const int y, const mapTile value)
{
	int id = y * mapWidth + x;
	if (id < 0 || id >= mapWidth * mapHeight)
		return;
	map[id] = value;
}

int Map::GetWidth()
{
	return mapWidth;
}

int Map::GetHeight()
{
	return mapHeight;
}