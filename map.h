#pragma once
#include <cstdio>

enum class MapTile : char {
	grass,
	road,
	stripes,
};

class Map {
private:
	MapTile* map;
	int mapWidth, mapHeight;
public:
	Map(const int mapWidth, const int mapHeight);
	~Map();
	MapTile* GetMap();
	int GetWidth();
	int GetHeight();
	MapTile GetMapTile(const int x, const int y);
	void SetMapTile(const int x, const int y, const MapTile value);
	void SaveToFile(FILE* file);
	void LoadFromFile(FILE* file);
};