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

void Map::SaveToFile(FILE* file) {
	fwrite(&mapWidth, sizeof(mapWidth), 1, file);
	fwrite(&mapHeight, sizeof(mapHeight), 1, file);
	for (int i = 0; i < mapWidth * mapHeight; i++)
		fwrite(&map[i], sizeof(map[i]), 1, file);
}

void Map::LoadFromFile(FILE* file){
	fread(&mapWidth, sizeof(mapWidth), 1, file);
	fread(&mapHeight, sizeof(mapHeight), 1, file);
	delete[] map;
	map = new MapTile[mapWidth * mapHeight];
	for (int i = 0; i < mapWidth * mapHeight; i++)
		fread(&map[i], sizeof(map[i]), 1, file); 
}