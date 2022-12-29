#include "map.h"

Map::Map(const int mapWidth, const int mapHeight)
{
	this->map = new MapTile[mapHeight * mapWidth];
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
}

Map::~Map()
{
	delete[] map;
}

MapTile* Map::GetMap()
{
	return map;
}


MapTile Map::GetMapTile(const int x, const int y)
{
	int id = y * mapWidth + x;
	if (x < 0 || x >= mapWidth)
		return MapTile::grass;
	if (y < 0 || y >= mapHeight)
		return MapTile::road;
	return map[id];
}

void Map::SetMapTile(const int x, const int y, const MapTile value)
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