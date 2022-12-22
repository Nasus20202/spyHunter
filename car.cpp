#include "car.h"

Car::Car(SDL_Surface* sprite, int x, int y, double speed) : Sprite(sprite, x, y) {
	this->speed = speed;
}
Car::Car() : Sprite() {
	this->speed = 0;
}

Car::~Car()
{
	int a = 2;
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

void Player::Shoot()
{
	
}

void Player::Accelerate()
{
	this->speed += 0.1;
}

void Player::Brake()
{
	this->speed -= 0.1;
}
