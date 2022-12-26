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

int Sprite::GetWidth()
{
	return sprite->w;
}

int Sprite::GetHeight()
{
	return sprite->h;
}

bool Sprite::CheckForCollision(Sprite* sprite)
{
	int thisXOffset = this->GetWidth() / 2, thisYOffset = this->GetHeight() / 2,
		otherXOffset = sprite->GetWidth() / 2, otherYOffset = sprite->GetHeight() / 2;
	int thisX = this->GetX(), thisY = this->GetY(), otherX = sprite->GetX(), otherY = sprite->GetY();
	if (thisX + thisXOffset > otherX - otherXOffset && thisX - thisXOffset < otherX + otherXOffset)
		if (thisY + thisYOffset > otherY - otherYOffset && thisY - thisYOffset < otherY + otherYOffset)
			return true;
	return false;
}
