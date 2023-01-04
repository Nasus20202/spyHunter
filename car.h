#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "map.h"
#define ACCELERATION 150
#define STEERING_FORCE 25
#define MAX_SPEED 1000

enum class CarType : char {
	player,
	civil,
	enemy,
	crashed,
	crashedPlayer,
	missile,
	enemyMissile,
	bomb,
	enemyBomb,
	explosion
};

enum class AmmoType : char {
	missle,
	bomb
};

class Car : public Sprite {
protected:
	double speed;
	CarType type;
public:
	Car(SDL_Surface* sprite, const int x, const int y, const double speed, CarType type = CarType::civil);
	Car();
	CarType GetType();
	void SetType(CarType type);
	void Crash(SDL_Surface* crashedSprite, CarType type = CarType::crashed);
	void SetSpeed(double speed);
	double GetSpeed();
	void Update(const double delta, const double playerSpeed);
	MapTile CheckForCollisionWithMap(const int screenWidth, const int screenHeight, Map* map);
	void SaveToFile(FILE* file);
	void LoadFromFile(FILE* file);
};

class Player : public Car {
private:
	double moveBuffer = 0, speedBuffer = 0, delta = 0;
	int ammo = 0; AmmoType ammoType = AmmoType::missle;
public:
	Player(SDL_Surface* sprite, const int x, const int y, const double speed = 50 * ACCELERATION);
	AmmoType Shoot();
	void Accelerate();
	void Brake();
	void Right();
	void Left();
	void Update(const double delta);
	int GetAmmo();
	void SetAmmo(int ammo);
	AmmoType GetAmmoType();
	void SetAmmoType(AmmoType ammoType);
	double SteeringSpeed();
	double AccelerationSpeed();
	void Crash(SDL_Surface* crashedSprite);
	void SaveToFile(FILE* file);
	void LoadFromFile(FILE* file);
};