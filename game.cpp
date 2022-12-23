#include "game.h"

Game::Game(const int mapWidth, const int mapHeight)
{
	cars = NULL;
	player = NULL;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
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
	NewMap();
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
	UpdateMap();
}

void Game::UpdateMap()
{
	
}

void Game::NewMap()
{
	this->map = new char[mapHeight * mapWidth];
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (x == 0 || x == mapWidth - 1)
				SetMapTile(x, y, mapTile::grass);
			else if (x == 1 || x == mapWidth - 2 || x == mapWidth / 2 && (y % 6 < 3))
				SetMapTile(x, y, mapTile::stripes);
			else
				SetMapTile(x, y, mapTile::road);
		}
	}
}

char Game::GetMapTile(const int x, const int y)
{
	return map[y * mapWidth + x];
}

char* Game::GetMap()
{
	return map;
}

void Game::SetMapTile(const int x, const int y, const char value)
{
	map[y * mapWidth + x] = value;
}

int Game::GetMapWidth()
{
	return mapWidth;
}

int Game::GetMapHeight()
{
	return mapHeight;
}


