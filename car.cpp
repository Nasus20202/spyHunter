#include "car.h"
#include "game.h"
#include <cmath>

#pragma region Car

	Car::Car(SDL_Surface* sprite, const int x, const int y, const double speed, CarType type) : Sprite(sprite, x, y) {
		this->speed = speed;
		this->type = type;
	}
	Car::Car() : Sprite() {
		this->speed = 0;
		this->type = CarType::civil;
	}

	CarType Car::GetType()
	{
		return type;
	}

	void Car::SetType(CarType type)
	{
		this->type = type;
	}

	void Car::Crash(SDL_Surface* crashedSprite)
	{
		type = CarType::crashed;
		SetSurface(crashedSprite);
		SetSpeed(0);
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

	void Player::Crash(SDL_Surface* crashedSprite)
	{
		this->type = CarType::crashedPlayer;
		SetSurface(crashedSprite);
		SetSpeed(0);
	}

	MapTile Player::CheckForCollisionWithMap(const int screenWidth, const int screenHeight, Map* map)
	{
		const int mapWidth = map->GetWidth(), mapHeight = map->GetHeight();
		const double blockWidth = screenWidth / (double)mapWidth, blockHeight = screenHeight / (double)mapHeight;
		const int carX = this->GetX(), carY = this->GetY();
		const int width = this->GetWidth(), height = this->GetHeight();
		const int rightX = carX + width / 2, leftX = carX - width / 2, bottomY = carY + height / 2, topY = carY - height / 2;
		const int rightBlock = rightX / blockWidth, leftBlock = leftX / blockWidth, bottomBlock = bottomY / blockHeight, topBlock = topY / blockHeight;
		MapTile resultTile = MapTile::road;
		for (int y = bottomBlock; y >= topBlock; y--)
			for (int x = leftBlock; x <= rightBlock; x++) {
				MapTile tile = map->GetMapTile(x, y);
				if (tile == MapTile::grass) {
					return tile; // return the first grass tile
				}
				else if (tile != MapTile::road && tile != MapTile::stripes) {
					resultTile = tile; // return the last tile that is not road or stripes
				}
			}
		return resultTile;
	}

	void Player::Update()
	{
		if (this->type == CarType::crashedPlayer)
			return;
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
