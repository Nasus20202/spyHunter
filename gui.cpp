#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "gui.h"
#include "game.h"
#include "car.h"
#include "sprite.h"

Uint32 Gui::GetRGB(Uint32 color) {
	return SDL_MapRGB(screen->format, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

void Gui::DrawText(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};
void Gui::DrawText(const char* text, const int x, const int y) {
	DrawText(screen, x, y, text, charset);
}

void Gui::DrawPixel(SDL_Surface* surface, const int x, const int y, Uint32 color)
{
	int bpp = surface->format->BytesPerPixel;
	if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}
void Gui::DrawPixel(const int x, const int y, Uint32 color)
{
	DrawPixel(screen, x, y, color);
}

void Gui::DrawLine(SDL_Surface* screen, const int x, const int y, const int length, const int dx, const int dy, Uint32 color)
{
	for (int i = 0; i < length; i++)
		DrawPixel(screen, x + i * dx, y + i * dy, color);
}
void Gui::DrawLine(const int x, const int y, const int length, const int dx, const int dy, Uint32 color)
{
	DrawLine(screen, x, y, length, dx, dy, color);
}

void Gui::DrawRectangle(SDL_Surface* screen, const int x, const int y, const int width, const int height, Uint32 outlineColor, Uint32 fillColor)
{
	int i;
	DrawLine(screen, x, y, width, 0, 1, outlineColor);
	DrawLine(screen, x + width - 1, y, height, 0, 1, outlineColor);
	DrawLine(screen, x, y, width, 1, 0, outlineColor);
	DrawLine(screen, x, y + height - 1, width, 1, 0, outlineColor);
	for (i = y + 1; i < y + height - 1; i++)
		DrawLine(screen, x + 1, i, width - 2, 1, 0, fillColor);
}
void Gui::DrawRectangle(const int x, const int y, const int width, const int height, Uint32 outlineColor, Uint32 fillColor)
{
	DrawRectangle(screen, x, y, width, height, outlineColor, fillColor);
}

void Gui::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, const int x, const int y)
{
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}
void Gui::DrawSurface(SDL_Surface* sprite, const int x, const int y)
{
	DrawSurface(screen, sprite, x, y);
}

void Gui::Initialize(const int width, const int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(1);
	}
	if (SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, TITLE);
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32 ,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE);
	
	charset = SDL_LoadBMP(CHARSET);
	if (charset == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", CHARSET, SDL_GetError());
		exit(1);
	};
	SDL_SetColorKey(charset, true, 0x000000);
	t1 = SDL_GetTicks();
	while (!quit) {
		Frame();
	}
}

void Gui::Frame() {
	t2 = SDL_GetTicks();
	delta = (t2 - t1) / 1000.0;
	t1 = t2;
	worldTime += delta;
	SDL_FillRect(screen, NULL, GetRGB(BACKGROUND));

	DrawRectangle(100, 100, 100, 100, GetRGB(FOREGROUND), GetRGB(FOREGROUND));
	DrawText("Hello world", 200, 200);
	
	// render
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			quit = true;
		}
		if (event.type == SDL_KEYDOWN) {
			Input(event.key.keysym.sym);
		}
	}
}

void Gui::Input(const SDL_Keycode key) {
	if (key == SDLK_ESCAPE) {
		quit = true;
	}
}

void Gui::Exit() {
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
	
Gui::Gui(const int width, const int height) {
	Initialize(width, height);
}

Gui::~Gui() {
	Exit();
}