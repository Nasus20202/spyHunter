#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"
#include "map.h"
// map scaling factor
#define SCALE 20
#define MAP_WIDTH (10*SCALE)
#define MAP_HEIGHT (20*SCALE)
#define START_ROAD_WIDTH (4*SCALE)
#define MIN_ROAD_WIDTH (4*SCALE)
#define GENERATION_DELAY (1*SCALE)
#define MAX_ISLAND_WIDTH (1*SCALE)
#define MAX_ISLAND_LENGTH (10*SCALE)
#define MIN_ISLAND_LENGHT (1*SCALE)
#define MIN_ISLAND_SPACE (10*SCALE)

#define SCORE_DIVIDER 100
#define START_LIVES 3
#define MISSLE_SPEED 500
#define SHOOT_COOLDOWN 0.5
#define SOFT_CRASH_SPEED 250

// max Y distance from screen before sprite is deleted
#define CLEAN_SPRITES_DISTANCE 1000
#define CAR_SPAWN_DISTANCE 200

#define PLAYER_SPRITE 0
#define CRASH_SPRITE 1
#define MISSLE_SPRITE 2
#define CARS_SPRITES_START 3

enum class State : char {
	playing,
	paused,
	dead,
	menu,
	quit
};

class Game {
private:
	Car* *cars;
	Car* *missiles;
	Player* player;
	int carsAmount = 0, missilesAmount = 0, mapWidth, mapHeight, screenWidth, screenHeight, frame = 0, lives, rightRoadBorder, leftRoadBorder, trafficIsland, mapUpdate, islandLength;
	double worldTime = 0, distance = 0, distanceDiff = 0, score, lastShot, shootCooldown = SHOOT_COOLDOWN;
	Map* map;
	State state;
	SDL_Surface** sprites; int spritesAmount;
public:
	Game(const int screenWidth, const int screenHeight, const int mapWidth = MAP_WIDTH, const int mapHeight = MAP_HEIGHT);
	~Game();
	void NewGame();
	
	Player* GetPlayer();
	void NewPlayer();
	
	Car* GetCar(const int index);
	Car** GetCars();
	int GetCarsAmount();
	void AddCar(Car* car);
	void RemoveCar(const int index);

	Car* GetMissile(const int index);
	Car** GetMissiles();
	int GetMissilesAmount();
	void AddMissile(Car* missle);
	void RemoveMissile(const int index);
	
	double GetTime();
	double GetDistance();
	int GetScore();
	int GetLives();

	void Update(const double delta);
	void RemoveUnncessarySprites();
	void NewMap();
	bool CheckForCollision();
	void Crash();
	void Shoot();
	
	void UpdateMap();
	void CalculateRoadBorders();
	MapTile GetMapTile(const int x, const int y);
	Map* GetMap();
	void SetSprites(SDL_Surface** sprites, const int amount);
	void SetMap(Map* map);
	void SetMapTile(const int x, const int y, const MapTile value);
	int GetMapWidth();
	int GetMapHeight();

	State GetState();
	void SetState(State state);

	int Random(const int from, const int to);
};

