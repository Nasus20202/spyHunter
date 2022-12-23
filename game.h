#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

enum mapTile : char {
	road = 'r',
	grass = 'g',
	stripes = 's',
};

class Game {
private:
	Car* *cars;
	Player* player;
	int carsAmount = 0, mapWidth, mapHeight;
	double worldTime = 0;
	char* map;
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

	void Update(const double delta);
	void NewMap();
	void UpdateMap();
	char GetMapTile(const int x, const int y);
	char* GetMap();
	void SetMapTile(const int x, const int y, const char value);
	int GetMapWidth();
	int GetMapHeight();
};
