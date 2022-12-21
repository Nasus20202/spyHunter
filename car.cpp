#include "car.h"

Car::Car(SDL_Surface* sprite, int x, int y, double speed) : Sprite(sprite, x, y) {
	this->speed = speed;
}

Car::~Car()
{
}

void Car::SetSpeed(double speed)
{
	this->speed = speed;
}

double Car::GetSpeed()
{
	return this->speed;
}

void Car::Update()
{
	this->MoveX(this->speed);
}
