#include "car.h"

#pragma region Car

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

	void Car::Update(const double delta)
	{
		this->MoveY(this->GetSpeed() * -delta);
	}

#pragma endregion

#pragma region Player 
	
	Player::Player(SDL_Surface* sprite, const int x, const int y, const double speed) : Car(sprite, x, y, speed)
	{
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

	void Player::Right()
	{
		this->moveBuffer+=1;
	}

	void Player::Left()
	{
		this->moveBuffer-=1;
	}

	void Player::Update()
	{
		if (this->moveBuffer > 0) {
			this->moveBuffer--;
			this->MoveX(1);
		}
		else if (this->moveBuffer < 0) {
			this->moveBuffer++;
			this->MoveX(-1);
		}
	}

#pragma endregion
