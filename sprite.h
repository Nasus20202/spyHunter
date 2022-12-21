#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

class Sprite {
private:
	SDL_Surface* sprite;
	int x, y;
public:
	Sprite(SDL_Surface* sprite, int x, int y);
	~Sprite();
	SDL_Surface* GetSurface();
	void SetSurface(SDL_Surface* sprite);
	void Move(int x, int y);
	void MoveX(int x);
	void MoveY(int y);
	void SetX(int x);
	void SetY(int y);
	void Update();
	int GetX();
	int GetY();
};
