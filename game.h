#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "sprite.h"
#include "car.h"
#include "map.h"
#define MAP_WIDTH 100
#define MAP_HEIGHT 200
#define TILE_HEIGHT 10
#define SCORE_DIVIDER 100
#define START_LIVES 3
#define PLAYER_SPRITE 0
#define CRASH_SPRITE 1
#define MISSLE_SPRITE 2
#define CARS_SPRITES_START 3
#define START_ROAD_WIDTH 40
#define MIN_ROAD_WIDTH 30
#define GENERATION_DELAY 10
#define MAX_ISLAND_WIDTH 10
#define MIN_ISLAND_LENGHT 10
#define MAX_ISLAND_LENGTH 100
#define MIN_ISLAND_SPACE 200

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
	Player* player;
	int carsAmount = 0, mapWidth, mapHeight, screenWidth, screenHeight, frame = 0, lives, rightRoadBorder, leftRoadBorder, trafficIsland, mapUpdate, islandLength;
	double worldTime = 0, distance = 0, distanceDiff = 0, score;
	Map* map;
	State state;
	SDL_Surface** sprites; int spritesAmount;
public:
	Game(const int screenWidth, const int screenHeight, const int mapWidth = MAP_WIDTH, const int mapHeight = MAP_HEIGHT);
	~Game();
	void AddCar(Car* car);
	void RemoveCar(const int index);
	void NewGame();
	Player* GetPlayer();
	void NewPlayer();
	Car* GetCar(const int index);
	Car** GetCars();
	int GetCarsAmount();
	double GetTime();
	double GetDistance();
	int GetScore();
	int GetLives();

	void Update(const double delta);
	void NewMap();
	bool CheckForCollision();
	void Crash();
	
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

