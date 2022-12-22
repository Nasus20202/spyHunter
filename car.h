#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"

class Car : public Sprite {
protected:
	double speed;
public:
	Car(SDL_Surface* sprite, int x, int y, double speed);
	Car();
	~Car();
	void SetSpeed(double speed);
	double GetSpeed();
	void Update();
};

class Player : public Car {
private:
public:
	void Shoot();
	void Accelerate();
	void Brake();
};