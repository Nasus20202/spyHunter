#include "car.h"
#include <cmath>

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

	void Car::Update(const double delta, const double playerSpeed)
	{
		this->MoveY((this->GetSpeed() - playerSpeed) * -delta);
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
		if ((this->GetSpeed() + speedBuffer) < MAX_SPEED)
			this->speedBuffer += ACCELERATION;
	}

	void Player::Brake()
	{
		if ((this->GetSpeed() + speedBuffer) > 0)
			this->speedBuffer -= ACCELERATION;
	}

	void Player::Right()
	{
		this->moveBuffer += SteeringSpeed();
	}

	void Player::Left()
	{
		this->moveBuffer-= SteeringSpeed();
	}

	double Player::SteeringSpeed()
	{
		double speed = this->GetSpeed();
		return pow(speed, 0.5) * 0.1;
	}

	void Player::Update()
	{
		if (this->moveBuffer > 0) {
			this->moveBuffer-= SteeringSpeed();
			this->MoveX(SteeringSpeed());
		}
		else if (this->moveBuffer < 0) {
			this->moveBuffer+= SteeringSpeed();
			this->MoveX(-SteeringSpeed());
		}
		if (this->speedBuffer > 0) {
			this->speedBuffer-= ACCELERATION;
			this->SetSpeed(this->GetSpeed() + ACCELERATION);
		}
		else if (this->speedBuffer < 0) {
			this->speedBuffer+= ACCELERATION;
			this->SetSpeed(this->GetSpeed() - ACCELERATION);
		}
	}

#pragma endregion
