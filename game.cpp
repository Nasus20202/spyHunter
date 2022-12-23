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
	worldTime = 0; frame = 0; distance = 0; distanceDiff = 0;
	frame = 0;
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

double Game::GetDistance()
{
	return distance;
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
	double localDiff = playerSpeed * delta;
	distanceDiff += localDiff;
	distance += localDiff;
	if (distanceDiff > TILE_HEIGHT) {
		UpdateMap(); frame++;
		distanceDiff -=TILE_HEIGHT;

	}
}

void Game::UpdateMap()
{
	char* newMap = new char[mapWidth * mapHeight];
	for (int y = 0; y < mapHeight - 1; y++) {
		for (int x = 0; x < mapWidth; x++) {
			newMap[(y + 1) * mapWidth + x] = GetMapTile(x, y);
		}
	}
	for (int x = 0; x < mapWidth; x++) {
		if (x == 0 || x == mapWidth - 1)
			SetMapTile(x, 0, mapTile::grass, newMap);
		else if (x == 1 || x == mapWidth - 2 || x == mapWidth / 2 && (frame % 20 < 7))
			SetMapTile(x, 0, mapTile::stripes, newMap);
		else
			SetMapTile(x, 0, mapTile::road, newMap);
	}
	delete[] map;
	map = newMap;
}

void Game::NewMap()
{
	this->map = new char[mapHeight * mapWidth];
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (x == 0 || x == mapWidth - 1)
				SetMapTile(x, y, mapTile::grass);
			else if (x == 1 || x == mapWidth - 2 || x == mapWidth / 2 && (y % 20 < 7))
				SetMapTile(x, y, mapTile::stripes);
			else
				SetMapTile(x, y, mapTile::road);
		}
	}
}

char Game::GetMapTile(const int x, const int y)
{
	return GetMapTile(x, y, map);
}

char Game::GetMapTile(const int x, const int y, char* map)
{
	return map[y * mapWidth + x];
}

char* Game::GetMap()
{
	return map;
}

void Game::SetMapTile(const int x, const int y, const char value, char* map)
{
	map[y * mapWidth + x] = value;
}

void Game::SetMapTile(const int x, const int y, const char value)
{
	SetMapTile(x, y, value, map);
}

int Game::GetMapWidth()
{
	return mapWidth;
}

int Game::GetMapHeight()
{
	return mapHeight;
}


