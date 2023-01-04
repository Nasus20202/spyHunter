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

	void Car::Crash(SDL_Surface* crashedSprite, CarType type)
	{
		this->type = type;
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

	MapTile Car::CheckForCollisionWithMap(const int screenWidth, const int screenHeight, Map* map)
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

	void Car::SaveToFile(FILE* file) {
		fwrite(&x, sizeof(x), 1, file);
		fwrite(&y, sizeof(y), 1, file);
		fwrite(&speed, sizeof(speed), 1, file);
		fwrite(&type, sizeof(type), 1, file);
	}

	void Car::LoadFromFile(FILE* file) {
		fread(&x, sizeof(x), 1, file);
		fread(&y, sizeof(y), 1, file);
		fread(&speed, sizeof(speed), 1, file);
		fread(&type, sizeof(type), 1, file);
	}

#pragma endregion

#pragma region Player 
	
	Player::Player(SDL_Surface* sprite, const int x, const int y, const double speed) : Car(sprite, x, y, speed)
	{
	}

	AmmoType Player::Shoot()
	{
		if (ammo > 0)
		{
			ammo--;
			return ammoType;
		}
		ammoType = AmmoType::missle;
		return ammoType;
	}

	void Player::Accelerate()
	{
		if ((this->GetSpeed() + speedBuffer) < MAX_SPEED)
			this->speedBuffer += AccelerationSpeed();
		if (this->GetSpeed() > MAX_SPEED) {
			this->SetSpeed(MAX_SPEED);
			if(speedBuffer > 0)
				speedBuffer = 0;
		}
	}

	void Player::Brake()
	{
		if ((this->GetSpeed() + speedBuffer) > 0)
			this->speedBuffer -= AccelerationSpeed();
		if (this->GetSpeed() < 0) {
			this->SetSpeed(0);
			if (speedBuffer < 0)
				speedBuffer = 0;
		}
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
		return pow(speed, 0.5) * STEERING_FORCE * delta;
	}

	double Player::AccelerationSpeed() {
		return ACCELERATION * delta;
	}

	void Player::Crash(SDL_Surface* crashedSprite)
	{
		this->type = CarType::crashedPlayer;
		SetSurface(crashedSprite);
		SetSpeed(0);
	}

	void Player::Update(const double delta)
	{
		this->delta = delta;
		if (this->type == CarType::crashedPlayer)
			return;
		const double steeringSpeed = SteeringSpeed(), accelerationSpeed = AccelerationSpeed();
		if (this->moveBuffer > 0) {
			this->moveBuffer-= steeringSpeed;
			this->MoveX(steeringSpeed);
		}
		else if (this->moveBuffer < 0) {
			this->moveBuffer+= steeringSpeed;
			this->MoveX(-steeringSpeed);
		}
		if (this->speedBuffer > 0) {
			this->speedBuffer-= accelerationSpeed;
			this->SetSpeed(this->GetSpeed() + accelerationSpeed);
		}
		else if (this->speedBuffer < 0) {
			this->speedBuffer+= accelerationSpeed;
			this->SetSpeed(this->GetSpeed() - accelerationSpeed);
		}
		if (this->moveBuffer > -steeringSpeed && this->moveBuffer < steeringSpeed)
			this->moveBuffer = 0;
		if (this->speedBuffer > -accelerationSpeed && this->speedBuffer < accelerationSpeed)
			this->speedBuffer = 0;
	}

	int Player::GetAmmo()
	{
		return ammo;
	}

	void Player::SetAmmo(int ammo)
	{
		this->ammo = ammo;
	}

	AmmoType Player::GetAmmoType()
	{
		return ammoType;
	}

	void Player::SetAmmoType(AmmoType ammoType)
	{
		this->ammoType = ammoType;
	}

	void Player::SaveToFile(FILE* file) {
		fwrite(&x, sizeof(x), 1, file);
		fwrite(&y, sizeof(y), 1, file);
		fwrite(&speed, sizeof(speed), 1, file);
		fwrite(&type, sizeof(type), 1, file);
		fwrite(&moveBuffer, sizeof(moveBuffer), 1, file);
		fwrite(&speedBuffer, sizeof(speedBuffer), 1, file);
		fwrite(&ammo, sizeof(ammo), 1, file);
		fwrite(&ammoType, sizeof(ammoType), 1, file);
	}

	void Player::LoadFromFile(FILE* file) {
		fread(&x, sizeof(x), 1, file);
		fread(&y, sizeof(y), 1, file);
		fread(&speed, sizeof(speed), 1, file);
		fread(&type, sizeof(type), 1, file);
		fread(&moveBuffer, sizeof(moveBuffer), 1, file);
		fread(&speedBuffer, sizeof(speedBuffer), 1, file);
		fread(&ammo, sizeof(ammo), 1, file);
		fread(&ammoType, sizeof(ammoType), 1, file);
	}

#pragma endregion
