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
	delete[] missiles;
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
	delete[] missiles;
	missiles = NULL;
	missilesAmount = 0;
	NewPlayer();
	worldTime = 0; frame = 0; distance = 0; distanceDiff = 0; frame = 0; score = 0; 
	shootCooldown = SHOOT_COOLDOWN; lastShot = 0;
	lives = START_LIVES;
	penaltyTime = -1;
	NewMap();
	for (int i = 0; i < Random(1, 10); i++) {
		GenerateNewCar(true);
	}
}

Player* Game::GetPlayer()
{
	return player;
}

void Game::NewPlayer()
{
	delete player;
	player = new Player(sprites[PLAYER_SPRITE], screenWidth / 2, screenHeight * 0.8, START_SPEED);
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

void Game::GenerateNewCar(bool onScreen) {
	double mapTileWidth = screenWidth / (double)mapWidth;
	int x = Random(rightRoadBorder * mapTileWidth, leftRoadBorder * mapTileWidth), y = Random(0, player->GetY() - player->GetHeight());
	int speed = Random(0, 2*START_SPEED);
	if (!onScreen) {
		if (Random(0, 1) == 0)
			y = screenHeight + CAR_SPAWN_DISTANCE;
		else
			y = -CAR_SPAWN_DISTANCE;
		if (y < 0)
			speed = Random(MIN_CAR_SPEED, player->GetSpeed());
		else
			speed = Random(player->GetSpeed(), MAX_CAR_SPEED);
	}
	Car* car;
	if (Random(0, 1) == 0) {
		// create enemy car
		car = new Car(sprites[Random(ENEMY_SPRITES_START, spritesAmount - 1)], x, y, speed, CarType::enemy);
	}
	else {
		// create civil car
		car = new Car(sprites[Random(CARS_SPRITES_START, ENEMY_SPRITES_START - 1)], x, y, speed, CarType::civil);
	}
	bool ok = false;
	while (!ok) {
		ok = true;
		if(player->CheckForCollision(car))
			ok = false;
		if(ok)
			for (int i = 0; i < GetCarsAmount(); i++)
				if (GetCar(i)->CheckForCollision(car)) {
					ok = false;
					break;
				}
		if (!ok)
			car->SetY(car->GetY() + 20);
	}
	AddCar(car);
}

void Game::Update(const double delta)
{
	worldTime += delta;
	Player* player = this->GetPlayer();
	player->Update();
	if (player->GetX() - player->GetWidth()/2 < 0)
		player->SetX(player->GetWidth() / 2);
	if (player->GetX() + player->GetWidth() / 2 > screenWidth)
		player->SetX(screenWidth - player->GetWidth() / 2);
	const double playerSpeed = player->GetSpeed();
	for (int i = 0; i < GetCarsAmount(); i++)
		GetCar(i)->Update(delta, playerSpeed);
	for (int i = 0; i < missilesAmount; i++)
		GetMissile(i)->Update(delta, playerSpeed);
	double localDiff = playerSpeed * delta;
	distanceDiff += localDiff;
	distance += localDiff;
	const double tileHeight = screenHeight / (double)mapHeight;
	double scoreDiff = 0;
	while (distanceDiff > tileHeight) {
		UpdateMap(); frame++;
		distanceDiff -= tileHeight;
		scoreDiff +=(1/(double)SCORE_DIVIDER);
	}
	RemoveUnncessarySprites();
	// player not on the grass
	if (!CheckForCollision()) {
		AddPoints(scoreDiff);
	} // grass penalty
	else {
		double speed = player->GetSpeed();
		if (speed > MAX_SPEED/2) {
			player->SetSpeed(speed - 1.5*ACCELERATION);
		}
		int shaking = Random(-1, 1);
		if (player->GetX() - player->GetWidth() / 2 - shaking >= 0 && player->GetX() + player->GetWidth() / 2 + shaking <= screenWidth)
			player->MoveX(shaking);
	}
	EnemyAction();
	
}

void Game::RemoveUnncessarySprites()
{
	// remove cars that are out of the screen
	bool done = false;
	while (!done) {
		done = true;
		for (int i = 0; i < GetCarsAmount(); i++) {
			if (GetCar(i)->GetY() > screenHeight + CLEAN_SPRITES_DISTANCE || GetCar(i)->GetY() < -CLEAN_SPRITES_DISTANCE) {
				RemoveCar(i);
				done = false; break;
			}
		}
	} done = false;
	while (!done) {
		done = true;
		for (int i = 0; i < GetMissilesAmount(); i++) {
			if (GetMissile(i)->GetY() > screenHeight + CLEAN_SPRITES_DISTANCE || GetMissile(i)->GetY() < -CLEAN_SPRITES_DISTANCE) {
				RemoveMissile(i);
				done = false; break;
			}
		}
	}
	
}

// return true if player touches grass
bool Game::CheckForCollision()
{
	bool result = false;
	for (int i = 0; i < GetMissilesAmount(); i++) {
		Car* missile = GetMissile(i); CarType type = missile->GetType();
		if (type == CarType::enemyMissile || type == CarType::enemyBomb) {
			if (missile->CheckForCollision(player) == true) {
				missile->SetX(player->GetX()); missile->SetY(player->GetY());
				missile->Crash(sprites[CRASH_SPRITE], CarType::explosion);
				Crash();
			}
		}
		else {
			for (int j = 0; j < GetCarsAmount(); j++) {
				Car* car = GetCar(j);
				if (car->GetType() != CarType::enemy && car->GetType() != CarType::civil || missile->GetType() != CarType::missile)
					continue;
				if (missile->CheckForCollision(car) == true) {
					missile->SetX(car->GetX()); missile->SetY(car->GetY());
					missile->Crash(sprites[CRASH_SPRITE], CarType::explosion);
					CarDestroyed(car);
				}
			}
		}
	}
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* car = GetCar(i);
		if (GetPlayer()->CheckForCollision(car)) {
			if (car->GetType() != CarType::enemy && car->GetType() != CarType::civil)
				continue;
			int speedDiff = car->GetSpeed() - GetPlayer()->GetSpeed();
			if(speedDiff < 0)
				speedDiff = -speedDiff;
			if (speedDiff > SOFT_CRASH_SPEED) {
				Crash();
				CarDestroyed(car);
			}
			else {
				PushCar(player, car, true);
			}
		}
	}
	// basic driving between the road edges
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* car = GetCar(i);
		if (car->GetType() != CarType::civil && car->GetType() != CarType::enemy)
			continue;
		const int x = car->GetX();
		MapTile tile = car->CheckForCollisionWithMap(screenWidth, screenHeight, map);
		if (tile == MapTile::grass || tile == MapTile::stripes) {
			bool right = false; int oldX = car->GetX();
			for (int x = car->GetX(); x < screenWidth - car->GetWidth() / 2; x += 20) {
				car->SetX(x);
				if (car->CheckForCollisionWithMap(screenWidth, screenHeight, map) != MapTile::grass) {
					right = true; break;
				}
			}
			car->SetX(oldX);
			if (right)
				car->MoveX(1);
			else
				car->MoveX(-1);
			for (int j = 0; j < GetCarsAmount(); j++) {
				if (i == j) continue;
				Car* otherCar = GetCar(j);
				if (car->CheckForCollision(otherCar) == true) {
					car->SetX(oldX);
				}
			}
		}
	}
	MapTile tile = GetPlayer()->CheckForCollisionWithMap(screenWidth, screenHeight, map);
	if (tile == MapTile::grass) {
		result = true;
	}
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* car = GetCar(i);
		if (car->GetType() != CarType::civil && car->GetType() != CarType::enemy)
			continue;
		for (int j = 0; j < GetCarsAmount(); j++) {
			if (i == j) continue;
			Car* otherCar = GetCar(j);
			PushCar(car, otherCar);
		}
	}
	return result;
}

void Game::PushCar(Car* pushingCar, Car* pushedCar, bool destroyCars) {
	if ((pushingCar->GetType() != CarType::player && pushingCar->GetType() != CarType::civil && pushingCar->GetType() != CarType::enemy) ||
		(pushedCar->GetType() != CarType::player && pushedCar->GetType() != CarType::civil && pushedCar->GetType() != CarType::enemy))
		return;
	const double ySensitivity = 0.45, xSensitivity = 0.25;
	// bottom push
	if (pushingCar->GetY() - pushingCar->GetHeight() / 2 >= pushedCar->GetY() + pushedCar->GetHeight() * ySensitivity) {
		int maxSpeed = pushingCar->GetSpeed();
		if (pushedCar->GetSpeed() > maxSpeed) {
			maxSpeed = pushedCar->GetSpeed();
		}
		pushingCar->SetSpeed(maxSpeed);
		pushedCar->SetSpeed(maxSpeed);
	}
	// being pushed from bottom
	else if (pushingCar->GetY() + pushingCar->GetHeight() / 2 <= pushedCar->GetY() - pushedCar->GetHeight() * ySensitivity) {
		int maxSpeed = pushingCar->GetSpeed();
		if (pushedCar->GetSpeed() > maxSpeed) {
			maxSpeed = pushedCar->GetSpeed();
		}
		pushingCar->SetSpeed(maxSpeed);
		pushedCar->SetSpeed(maxSpeed);
	}
	// right push
	else if (pushingCar->GetX() - pushingCar->GetWidth() / 2 <= pushedCar->GetX() + pushedCar->GetWidth() * xSensitivity) {
		pushedCar->SetX(pushingCar->GetX() + pushingCar->GetWidth() / 2 + pushedCar->GetWidth() / 2);
	}
	// left push
	else if (pushingCar->GetX() + pushingCar->GetWidth() / 2 >= pushedCar->GetX() - pushedCar->GetWidth() * xSensitivity) {
		pushedCar->SetX(pushingCar->GetX() - pushingCar->GetWidth() / 2 - pushedCar->GetWidth() / 2);
	}
	if (pushedCar->CheckForCollisionWithMap(screenWidth, screenHeight, map) == MapTile::grass) {
		if(destroyCars)
			CarDestroyed(pushedCar);
	}
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* secondPushedCar = GetCar(i);
		CarType type = secondPushedCar->GetType();
		if (secondPushedCar == pushingCar || secondPushedCar == pushedCar || (type != CarType::player && type != CarType::civil && type != CarType::enemy))
			continue;
		if (pushedCar->CheckForCollision(secondPushedCar) == true) {
			PushCar(pushedCar, secondPushedCar, destroyCars);
		}
	}
}

void Game::CarDestroyed(Car* car, CarType type) {
	if (car->GetType() == CarType::enemy)
		AddPoints(ENEMY_POINTS);
	else if (car->GetType() == CarType::civil)
		penaltyTime = worldTime + PENALTY_TIME;
	car->Crash(sprites[CRASH_SPRITE], type);
}

void Game::Crash() {
	player->Crash(sprites[CRASH_SPRITE]);
	if(worldTime >= IMMORTAL_TIMER)
		lives--;
	if (lives <= 0) {
		state = State::dead;
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

Car** Game::GetMissiles()
{
	return missiles;
}

Car* Game::GetMissile(const int index)
{
	if (index < 0 || index >= missilesAmount)
		return NULL;
	return missiles[index];
}

int Game::GetMissilesAmount()
{
	return missilesAmount;
}

void Game::AddMissile(Car* missile)
{
	Car** temp = new Car * [missilesAmount + 1];
	for (int i = 0; i < missilesAmount; i++)
		temp[i] = missiles[i];
	temp[missilesAmount] = missile;
	missilesAmount++;
	delete[] missiles;
	missiles = temp;
}

void Game::RemoveMissile(const int index)
{
	Car** temp = new Car * [missilesAmount - 1];
	for (int i = 0; i < index; i++)
		temp[i] = missiles[i];
	for (int i = index; i < missilesAmount - 1; i++)
		temp[i] = missiles[i + 1];
	missilesAmount--;
	delete[] missiles;
	missiles = temp;
}

void Game::Shoot()
{
	if (worldTime - lastShot < shootCooldown)
		return;
	lastShot = worldTime;
	AmmoType ammo = player->Shoot();
	if (ammo == AmmoType::missle) {
		Car* missle = new Car(sprites[MISSLE_SPRITE], player->GetX(), player->GetY() + player->GetHeight() / 2, player->GetSpeed() + MISSLE_SPEED, CarType::missile);
		AddMissile(missle);
	}
}

void Game::EnemyAction() {
	for (int i = 0; i < GetCarsAmount(); i++) {
		Car* car = GetCar(i);
		if(car->GetType() != CarType::enemy)
			continue;
		// same X
		if (car->GetX() + car->GetWidth() / 2 > player->GetX() - player->GetWidth() / 2 && car->GetX() - car->GetWidth() / 2 < player->GetX() + player->GetWidth() / 2) {
			// place bomb behind the car
			if (car->GetY() < player->GetY()) {
				int placeBombProbability = Random(0, DIFFICULTY);
				if (placeBombProbability == 0) {
					AddMissile(new Car(sprites[BOMB_SPRITE], car->GetX(), car->GetY() + car->GetHeight() / 2, 0, CarType::enemyBomb));
				}
			} // shoot a missile
			else if (car->GetY() > player->GetY()) {
				int shootProbability = Random(0, DIFFICULTY);
				if (shootProbability == 0) {
					AddMissile(new Car(sprites[MISSLE_SPRITE], car->GetX(), car->GetY() + car->GetHeight() / 2, car->GetSpeed() + ENEMY_MISSILE_SPEED, CarType::enemyMissile));
				}
			}
		}
	}
}

int Game::GetScore()
{
	return score;
}

void Game::AddPoints(const double points)
{
	if(worldTime > penaltyTime)
		score += points;
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

int Game::Random(int from, int to)
{
	if (to < from) {
		int temp = from;
		from = to;
		to = temp;
	}
	int diff = to - from + 1;
	return rand() % diff + from;
}
