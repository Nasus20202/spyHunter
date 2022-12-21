#include "sprite.h"

Sprite::Sprite(SDL_Surface* sprite, int x, int y)
{
	this->SetSurface(sprite);
	this->SetX(x);
	this->SetY(y);
}

Sprite::~Sprite()
{
	delete sprite;
}

SDL_Surface* Sprite::GetSurface()
{
	return sprite;
}

void Sprite::SetSurface(SDL_Surface* sprite)
{
	this->sprite = sprite;
}

void Sprite::Move(int x, int y)
{
	this->MoveX(x); this->MoveY(y);
}

void Sprite::MoveX(int x)
{
	this->x += x;
}

void Sprite::MoveY(int y)
{
	this->y += y;
}

void Sprite::SetX(int x)
{
	this->x = x;
}

void Sprite::SetY(int y)
{
	this->y = y;
}

void Sprite::Update()
{
}

int Sprite::GetX()
{
	return x;
}

int Sprite::GetY()
{
	return y;
}
