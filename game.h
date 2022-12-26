#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"
#include "map.h"
#define MAP_WIDTH 100
#define MAP_HEIGHT 200
#define TILE_HEIGHT 10
#define SCORE_DIVIDER 100

class Game {
private:
	Car* *cars;
	Player* player;
	int carsAmount = 0, mapWidth, mapHeight, screenWidth, screenHeight, frame = 0, score;
	double worldTime = 0, distance = 0, distanceDiff = 0;
	Map* map;
public:
	Game(const int screenWidth, const int screenHeight, const int mapWidth = MAP_WIDTH, const int mapHeight = MAP_HEIGHT);
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
	bool CheckForCollision();
	
	void UpdateMap();
	MapTile GetMapTile(const int x, const int y);
	Map* GetMap();
	void SetMap(Map* map);
	void SetMapTile(const int x, const int y, const MapTile value);
	int GetMapWidth();
	int GetMapHeight();
	int GetScore();

	int Random(const int from, const int to);
};

