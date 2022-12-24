#pragma once

enum mapTile : char {
	road = 'r',
	grass = 'g',
	stripes = 's',
};

class Map {
private:
	mapTile* map;
	int mapWidth, mapHeight;
public:
	Map(const int mapWidth, const int mapHeight);
	~Map();
	mapTile* GetMap();
	int GetWidth();
	int GetHeight();
	mapTile GetMapTile(const int x, const int y);
	void SetMapTile(const int x, const int y, const mapTile value);
};