#pragma once
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"

class Sprite {
private:
	SDL_Surface* sprite;
	double x, y;
public:
	Sprite(SDL_Surface* sprite, const double x, const double y);
	Sprite();
	~Sprite();
	SDL_Surface* GetSurface();
	void SetSurface(SDL_Surface* sprite);
	void Move(const double x, const double y);
	void MoveX(const double x);
	void MoveY(const double y);
	void SetX(const double x);
	void SetY(const double y);
	void Update(const double delta);
	int GetX();
	int GetY();
};
