#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"
#define MAP_WIDTH 100
#define MAP_HEIGHT 200
#define TILE_HEIGHT 10

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
	int GetMapWidth();
	int GetMapHeight();
	mapTile GetMapTile(const int x, const int y);
	void SetMapTile(const int x, const int y, const mapTile value);
};

class Game {
private:
	Car* *cars;
	Player* player;
	int carsAmount = 0, mapWidth, mapHeight;
	double worldTime = 0, distance = 0, distanceDiff = 0; int frame = 0;
	Map* map;
	void SetMapTile(const int x, const int y, const char value, char* map);
	char GetMapTile(const int x, const int y, char* map);
public:
	Game(const int mapWidth = MAP_WIDTH, const int mapHeight = MAP_HEIGHT);
	~Game();
	void AddCar(Car* car);
	void RemoveCar(const int index);
	void NewGame(Player* player);
	Player* GetPlayer();
	Car* GetCar(const int index);
	Car** GetCars();
	int GetCarsAmount();
	double GetTime();
	double GetDistance();

	void Update(const double delta);
	void NewMap();
	void UpdateMap();
	mapTile GetMapTile(const int x, const int y);
	Map* GetMap();
	void SetMapTile(const int x, const int y, const mapTile value);
	int GetMapWidth();
	int GetMapHeight();
};

