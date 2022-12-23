#include "game.h"

enum road : char {
	road = 'r',
	grass = 'g',
	stripes = 's',
};

Game::Game(const int mapHeight, const int mapWidth)
{
	cars = NULL;
	player = NULL;
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;
	NewMap();
}

Game::~Game()
{
	delete[] cars;
	delete player;
	delete[] map;
}

void Game::AddCar(Car *car)
{
	Car* *temp = new Car*[carsAmount + 1];
	for (int i = 0; i < carsAmount; i++)
		temp[i] = cars[i];
	temp[carsAmount] = car;
	carsAmount++;
	delete[] cars;
	cars = temp;
}

void Game::RemoveCar(const int index)
{
	Car** temp = new Car * [carsAmount - 1];
	for (int i = 0; i < index; i++)
		temp[i] = cars[i];
	for (int i = index; i < carsAmount - 1; i++)
		temp[i] = cars[i + 1];
	carsAmount--;
	delete[] cars;
	cars = temp;
}

void Game::NewGame(Player* player)
{
	delete[] cars;
	cars = NULL;
	carsAmount = 0;
	delete this->player;
	this->player = player;
	worldTime = 0;
}

Player* Game::GetPlayer()
{
	return player;
}

Car* Game::GetCar(const int index)
{
	if (index < 0 || index >= carsAmount)
		return NULL;
	return cars[index];
}

Car** Game::GetCars()
{
	return cars;
}

int Game::GetCarsAmount()
{
	return carsAmount;
}

double Game::GetTime()
{
	return worldTime;
}

void Game::Update(const double delta)
{
	worldTime += delta;
	Player* player = this->GetPlayer();
	player->Update();
	const double playerSpeed = player->GetSpeed();
	for (int i = 0; i < GetCarsAmount(); i++) {
		GetCar(i)->Update(delta, playerSpeed);
	}
}

void Game::NewMap()
{
	this->map = new char[mapHeight * mapWidth];
	for (int i = 0; i < mapHeight * mapWidth; i++)
		this->map[i] = grass;
}

char Game::GetMap(const int x, const int y)
{
	return map[y * mapWidth + x];
}

char* Game::GetMap()
{
	return map;
}

void Game::SetMap(const int x, const int y, const char value)
{
	map[y * mapWidth + x] = value;
}


