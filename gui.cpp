#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include "gui.h"
#include "game.h"

int Gui::GetRGB(Color color) {
	return SDL_MapRGB(screen->format, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
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
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

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
	SDL_FillRect(screen, NULL, GetRGB(BLACK));
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