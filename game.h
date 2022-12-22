#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"

class Game {
private:
	Car* *cars;
	int carsAmount = 0;
public:
	Game();
	~Game();
	void AddCar(Car* car);
	Car* GetCar(const int index);
	Car** GetCars();
	int GetCarsAmount();
};
