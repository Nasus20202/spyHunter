#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"

class Game {
private:
	Car* *cars;
	Player* player;
	int carsAmount = 0, mapHeight, mapWidth;
	double worldTime = 0;
	char* map;
public:
	Game(const int mapHeight = 0, const int mapWidth = 0);
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
	char GetMap(const int x, const int y);
	char* GetMap();
	void SetMap(const int x, const int y, const char value);
};
