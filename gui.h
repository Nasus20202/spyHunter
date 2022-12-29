#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#include "game.h"

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280
#define TITLE "Spy Hunter"
// fonts
#define CHARSET_BIG "assets/cs16x16.bmp"
#define CHARSET_SMALL "assets/cs8x8.bmp"
#define FONT_SIZE_BIG 16
#define FONT_SIZE_SMALL 8
// files to be loaded as sprites
#define ASSETS  "assets/player.bmp", "assets/explosion.bmp", "assets/missle.bmp", \
				 "assets/blackcar.bmp", "assets/bluecar.bmp", "assets/greencar.bmp", "assets/redcar.bmp", \
				"assets/whitecar.bmp", "assets/yellowcar.bmp", "assets/pinkcar.bmp"
// frames per second cap
#define FPS_CAP 100
#define FRAME_TIME (1/((double)FPS_CAP))
// game state refresh rate, including player input
#define UPDATE_RATE 250
#define UPDATE_TIME (1/((double)UPDATE_RATE))

enum Color : Uint32 {
	BLACK = 0x000000,
	BLUE = 0x0000FF,
	GREEN = 0x00FF00,
	CYAN = 0x00FFFF,
	RED = 0xFF0000,
	MAGENTA = 0xFF00FF,
	BROWN = 0x8B4513,
	LIGHTGRAY = 0xD3D3D3,
	DARKGRAY = 0xA9A9A9,
	LIGHTBLUE = 0xADD8E6,
	LIGHTGREEN = 0x90EE90,
	LIGHTCYAN = 0xE0FFFF,
	LIGHTRED = 0xFFB6C1,
	LIGHTMAGENTA = 0xFFB6C1,
	YELLOW = 0xFFFF00,
	WHITE = 0xFFFFFF,
	BACKGROUND = 0x075d0f,
	FOREGROUND = 0x222222
};

class Point {
public:
	Point(const int x, const int y);
	int x, y;
};


class Gui {
private:
	Game* game;
	int t1, t2, frames = 0, spritesCount = 0, width, height;
	double fpsTimer = 0, updateTimer = 0, frameTimer = 0, fps, updateTime, frameTime;
	SDL_Event event;
	SDL_Surface* screen, *charsetBig, *charsetSmall;
	SDL_Surface* *sprites;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	const Uint8* currentKeyStates;

	Uint32 GetRGB(Uint32 color);
	SDL_Surface* LoadSurface(const char* name);
	void DrawText(SDL_Surface* surface, Point point, const char* text, SDL_Surface* charset, int fontSize);
	void DrawText(const char* text, const Point point = {0, 0}, bool big = true);
	void DrawPixel(SDL_Surface* surface, const Point point, Uint32 color = BLACK);
	void DrawPixel(const Point point = {0, 0}, Uint32 color = BLACK);
	void DrawLine(SDL_Surface* screen, const Point point, const int length, const int dx, const int dy, Uint32 color = BLACK);
	void DrawLine(const Point point = { 0, 0 }, const int length = 1, const int dx = 1, const int dy = 1, Uint32 color = BLACK);
	void DrawRectangle(SDL_Surface* screen, const Point point, const int width, const int height, Uint32 color);
	void DrawRectangle(const Point point, const int width, const int height, Uint32 color = BLACK);
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, const Point point);
	void DrawSurface(SDL_Surface * sprite, const Point point);

	void NewGame();
	void Pause();
	
	void Initialize(const int width, const int height, const char* title);
	void LoadSprites();
	void LoadSprite(const char* path);
	void PrintGameInfo(const Point point);
	void PrintMap();
	void Update();
	void Frame();
	void Input(const SDL_Keycode code);
	void GameInput();
	void Exit();
public:
	Gui(const int width = SCREEN_WIDTH, const int height = SCREEN_HEIGHT, const double updateTime = UPDATE_TIME, const double frameTime = FRAME_TIME, const char* title = TITLE);
	~Gui();
};