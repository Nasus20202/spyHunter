#include "sprite.h"

Sprite::Sprite(SDL_Surface* sprite, const double x, const double y)
{
	this->SetSurface(sprite);
	this->SetX(x);
	this->SetY(y);
}

Sprite::Sprite()
{
	this->SetSurface(NULL);
	this->SetX(0);
	this->SetY(0);
}

Sprite::~Sprite()
{
}

SDL_Surface* Sprite::GetSurface()
{
	return sprite;
}

void Sprite::SetSurface(SDL_Surface* sprite)
{
	this->sprite = sprite;
}

void Sprite::Move(const double x, const double y)
{
	this->MoveX(x); this->MoveY(y);
}

void Sprite::MoveX(const double x)
{
	this->x += x;
}

void Sprite::MoveY(const double y)
{
	this->y += y;
}

void Sprite::SetX(const double x)
{
	this->x = x;
}

void Sprite::SetY(const double y)
{
	this->y = y;
}

void Sprite::Update(const double delta)
{
}

int Sprite::GetX()
{
	return (int)x;
}

int Sprite::GetY()
{
	return (int)y;
}
