#include "game.h"
#include <ctime>
#include <cstdlib>
#include <cstdio>

Game::Game(const int screenWidth, const int screenHeight, const int mapWidth, const int mapHeight)
{
	cars = NULL;
	player = NULL;
	sprites = NULL;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
	NewMap();
}

Game::~Game()
{
	delete[] cars;
	delete player;
	delete map;
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

void Game::NewGame()
{
	srand(time(NULL));
	state = State::playing;
	delete[] cars;
	cars = NULL;
	carsAmount = 0;
	NewPlayer();
	worldTime = 0; frame = 0; distance = 0; distanceDiff = 0, frame = 0, score = 0;
	lives = START_LIVES;
	NewMap();
}

Player* Game::GetPlayer()
{
	return player;
}

void Game::NewPlayer()
{
	delete player;
	player = new Player(sprites[PLAYER_SPRITE], screenWidth / 2, screenHeight * 0.8, 100);
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
		score+=(1/(double)SCORE_DIVIDER);
	}
	CheckForCollision();
}


bool Game::CheckForCollision()
{
	bool result = false;
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* car = GetCar(i);
		if (GetPlayer()->CheckForCollision(car)) {
			if (car->GetType() != CarType::enemy && car->GetType() != CarType::civil)
				continue;
			Crash();
			car->Crash(sprites[CRASH_SPRITE]);
			result = true;
		}
	}
	MapTile tile = GetPlayer()->CheckForCollisionWithMap(screenWidth, screenHeight, map);
	if (tile == MapTile::grass) {
		Crash();
		result = true;
	}
	return result;
}

void Game::Crash() {
	lives--;
	if (lives <= 0) {
		//state = State::dead;
	}
	player->Crash(sprites[CRASH_SPRITE]);
	Car* destroyed = new Car(sprites[1], player->GetX(), player->GetY(), 0, CarType::crashedPlayer);
	AddCar(destroyed);
	NewPlayer();
}

void Game::UpdateMap()
{
	for (int y = mapHeight - 1; y >= 1; y--) {
		for (int x = 0; x < mapWidth; x++) {
			map->SetMapTile(x, y, map->GetMapTile(x, y - 1));
		}
	}
	for (int x = 0; x < mapWidth; x++) {
		if (x <= 10 || x >= mapWidth - 10)
			SetMapTile(x, 0, MapTile::grass);
		else if (x == 1 || x == mapWidth - 2 || x == mapWidth / 2 && (frame % 20 < 7))
			SetMapTile(x, 0, MapTile::stripes);
		else
			SetMapTile(x, 0, MapTile::road);
	}
}

void Game::NewMap()
{
	delete map;
	this->map = new Map(mapWidth, mapHeight);
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (x == 0 || x == mapWidth - 1)
				SetMapTile(x, y, MapTile::grass);
			else if (x == 1 || x == mapWidth - 2 || x == mapWidth / 2 && (y % 20 < 7))
				SetMapTile(x, y, MapTile::stripes);
			else
				SetMapTile(x, y, MapTile::road);
		}
	}
}

MapTile Game::GetMapTile(const int x, const int y)
{
	return map->GetMapTile(x, y);
}

Map* Game::GetMap()
{
	return map;
}

void Game::SetSprites(SDL_Surface** sprites, const int amount)
{
	this->sprites = sprites;
	spritesAmount = amount;
}

void Game::SetMap(Map* map)
{
	delete this->map;
	this->map = map;
}

void Game::SetMapTile(const int x, const int y, const MapTile value)
{
	map->SetMapTile(x, y, value);
}

int Game::GetMapWidth()
{
	return map->GetWidth();
}

int Game::GetMapHeight()
{
	return map->GetHeight();
}

int Game::GetScore()
{
	return score;
}

int Game::GetLives()
{
	return lives;
}

State Game::GetState()
{
	return state;
}

void Game::SetState(State state)
{
	this->state = state;
}

int Game::Random(const int from, const int to)
{
	if(to < from)
		return 0;
	int diff = to - from + 1;
	return rand() % diff + from;
}
