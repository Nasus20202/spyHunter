#pragma once

enum class MapTile : char {
	road = 'r',
	grass = 'g',
	stripes = 's',
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
};