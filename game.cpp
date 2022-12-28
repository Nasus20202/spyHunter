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
	worldTime = 0; frame = 0; distance = 0; distanceDiff = 0; frame = 0; score = 0;
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
	int x = 0;
	while (player->CheckForCollisionWithMap(screenWidth, screenHeight, map) == MapTile::grass) {
		player->SetX(x);
		x+=20;
	}
	if (x != 0) {
		int newX = x + 20;
		player->SetX(newX);
		if (player->CheckForCollisionWithMap(screenWidth, screenHeight, map) == MapTile::grass)
			player->SetX(newX - 20);
	}
	bool noCollisionWithCars = false;
	while (noCollisionWithCars) {
		for (int i = 0; i < carsAmount; i++)
			if (player->CheckForCollision(GetCar(i)) == true) {
				RemoveCar(i);
				break;
			}
		noCollisionWithCars = true;
	}
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
	const double tileHeight = screenHeight / (double)mapHeight;
	while (distanceDiff > tileHeight) {
		UpdateMap(); frame++;
		distanceDiff -= tileHeight;
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
	player->Crash(sprites[CRASH_SPRITE]);
	lives--;
	if (lives <= 0) {
		//state = State::dead;
	}
	Car* destroyed = new Car(sprites[1], player->GetX(), player->GetY(), 0, CarType::crashedPlayer);
	AddCar(destroyed);
	NewPlayer();
}

void Game::UpdateMap()
{
	// copy old map
	for (int y = mapHeight - 1; y >= 1; y--) {
		for (int x = 0; x < mapWidth; x++) {
			map->SetMapTile(x, y, map->GetMapTile(x, y - 1));
		}
	}
	// update generation values
	if (mapUpdate % GENERATION_DELAY == 0)
		CalculateRoadBorders();
	
	// generate new map
	const int roadWidth = leftRoadBorder - rightRoadBorder;
	const int middle = rightRoadBorder + roadWidth / 2;
	for (int x = 0; x < mapWidth; x++) {
		if (x < rightRoadBorder || x > leftRoadBorder)
			SetMapTile(x, 0, MapTile::grass);
		else if (trafficIsland > 0 && x > middle - (trafficIsland / 2 + 1) && x < middle + (trafficIsland / 2 + 1)) {
			SetMapTile(x, 0, MapTile::grass);
		}
		else
			SetMapTile(x, 0, MapTile::road);
	}
	// add stripes at borded of the road
	for (int x = 0; x < mapWidth; x++) {
		if (GetMapTile(x, 0) == MapTile::grass && GetMapTile(x + 1, 0) == MapTile::road)
			SetMapTile(x + 1, 0, MapTile::stripes);
		else if (GetMapTile(x, 0) == MapTile::road && GetMapTile(x + 1, 0) == MapTile::grass)
			SetMapTile(x, 0, MapTile::stripes);
	}
	mapUpdate++;
}

void Game::CalculateRoadBorders() {
	int rightDiff = Random(-1, 1), leftDiff = Random(-1, 1), islandDiff = Random(-5, 1);
	if (rightRoadBorder < mapWidth * 0.1)
		rightDiff = Random(-1, 3);
	else if (rightRoadBorder < mapWidth * 0.2)
		rightDiff = Random(-1, 2);
	if (leftRoadBorder > mapWidth * 0.9)
		leftDiff = Random(-1, 2);
	else if (leftRoadBorder > mapWidth * 0.8)
		leftDiff = Random(-1, 3);
	if (rightDiff > 0)
		rightDiff = 1;
	if (leftDiff > 0)
		leftDiff = 1;
	if (islandDiff < -1)
		islandDiff = -1;
	if (islandLength > 0 && islandLength < MIN_ISLAND_LENGHT)
		islandDiff = Random(0, 1);
	if (islandLength > -MIN_ISLAND_SPACE && islandLength < 0)
		islandDiff = 0;
	if (islandLength > MAX_ISLAND_LENGTH)
		islandDiff = -1;
	rightRoadBorder += rightDiff;
	leftRoadBorder -= leftDiff;
	trafficIsland += islandDiff;
	if (rightRoadBorder < 0)
		rightRoadBorder = 0;
	if (leftRoadBorder > mapWidth - 1)
		leftRoadBorder = mapWidth - 1;
	if (trafficIsland < 0)
		trafficIsland = 0;
	if (trafficIsland > MAX_ISLAND_WIDTH)
		trafficIsland = MAX_ISLAND_WIDTH;
	int width = leftRoadBorder - rightRoadBorder - trafficIsland;
	if (width < MIN_ROAD_WIDTH) {
		int diff = MIN_ROAD_WIDTH - width;
		if (leftRoadBorder < mapWidth / 2) {
			leftRoadBorder += diff;
		}
		else if (rightRoadBorder >= mapWidth / 2) {
			rightRoadBorder -= diff;
		}
		else {
			leftRoadBorder += diff / 2;
			rightRoadBorder -= diff / 2 + diff % 2;
		}
	}
	if (trafficIsland > 0) {
		if (islandLength < 0)
			islandLength = 0;
		islandLength++;
	}
	else
		islandLength = islandLength > 0 ? -1 : islandLength - 1;
}

void Game::NewMap()
{
	delete map; const int middle = mapWidth / 2;
	rightRoadBorder = middle - START_ROAD_WIDTH / 2, leftRoadBorder = middle + START_ROAD_WIDTH / 2, trafficIsland = 0, islandLength = 0, mapUpdate = 0;
	this->map = new Map(mapWidth, mapHeight);
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (x < rightRoadBorder || x > leftRoadBorder)
				SetMapTile(x, y, MapTile::grass);
			else if (x == rightRoadBorder || x == leftRoadBorder/*|| x == mapWidth / 2 && (y % 20 < 7)*/)
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
