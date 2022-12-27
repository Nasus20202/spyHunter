#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "map.h"
#define ACCELERATION 2
#define MAX_SPEED 3000

enum class CarType : char {
	player,
	civil,
	enemy,
	crashed,
	crashedPlayer
};

class Car : public Sprite {
protected:
	double speed;
	CarType type;
public:
	Car(SDL_Surface* sprite, const int x, const int y, const double speed, CarType type = CarType::civil);
	Car();
	CarType GetType();
	void Crash(SDL_Surface* crashedSprite);
	void SetSpeed(double speed);
	double GetSpeed();
	void Update(const double delta, const double playerSpeed);
};

class Player : public Car {
private:
	double moveBuffer = 0, speedBuffer = 0;
public:
	Player(SDL_Surface* sprite, const int x, const int y, const double speed = 50 * ACCELERATION);
	void Shoot();
	void Accelerate();
	void Brake();
	void Right();
	void Left();
	void Update();
	double SteeringSpeed();
	void Crash(SDL_Surface* crashedSprite);
	MapTile CheckForCollisionWithMap(const int screenWidth, const int screenHeight, Map* map);
};