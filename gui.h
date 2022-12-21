#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280
#define TITLE "Spy Hunter"
#define CHARSET "assets/cs8x8.bmp"

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

class Gui {
private:
	bool quit = false;
	int t1, t2, frames = 0;
	double delta, worldTime = 0, fpsTimer, fps;
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 GetRGB(Uint32 color);
	void DrawText(SDL_Surface* surface, int x, int y, const char* text, SDL_Surface* charset);
	void DrawText(const char* text, const int x = 0, const int y  = 0);
	void DrawPixel(SDL_Surface* surface, const int x, const int y, Uint32 color = BLACK);
	void DrawPixel(const int x = 0, const int y = 0, Uint32 color = BLACK);
	void DrawLine(SDL_Surface* screen, const int x, const int y, const int length, const int dx, const int dy, Uint32 color = BLACK);
	void DrawLine(const int x = 0, const int y = 0, const int length = 1, const int dx = 1, const int dy = 1, Uint32 color = BLACK);
	void DrawRectangle(SDL_Surface* screen, const int x, const int y, const int width, const int height, Uint32 outlineColor, Uint32 fillColor);
	void DrawRectangle(const int x, const int y, const int width, const int height, Uint32 outlineColor, Uint32 fillColor);
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, const int x, const int y);
	void DrawSurface(SDL_Surface * sprite, const int x = 0, const int y = 0);

	
	void Initialize(const int width, const int height);
	void Frame();
	void Input(const SDL_Keycode code);
	void Exit();
public:
	Gui(const int width = SCREEN_WIDTH, const int height = SCREEN_HEIGHT);
	~Gui();
};