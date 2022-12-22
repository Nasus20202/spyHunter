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

SDL_Surface* Gui::LoadSurface(const char* name) {
	SDL_Surface* surface = SDL_LoadBMP(name);
	if (surface == NULL) {
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return NULL;
	}
	return surface;
}

void Gui::DrawText(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset, int fontSize) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = s.h = d.w = d.h = fontSize;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * fontSize;
		py = (c / 16) * fontSize;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += fontSize;
		text++;
	};
};
void Gui::DrawText(const char* text, const int x, const int y, bool big) {
	DrawText(screen, x, y, text, big ? charsetBig : charsetSmall, big ? FONT_SIZE_BIG : FONT_SIZE_SMALL);
}

void Gui::DrawPixel(SDL_Surface* surface, const int x, const int y, Uint32 color)
{
	if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) return;
	int bpp = surface->format->BytesPerPixel;
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

void Gui::NewGame()
{
	Player* player = new Player(sprites[0], SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.8);
	game.NewGame(player);
	// add temp cars
	for (int i = 1; i <= 7; i++)
		game.AddCar(new Car(sprites[i], (i + 1) * 100, 600, (i + 1) * 100));
	worldTime = 0;
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
	currentKeyStates = SDL_GetKeyboardState(NULL);
	
	charsetBig = LoadSurface(CHARSET_BIG);
	charsetSmall = LoadSurface(CHARSET_SMALL);
	LoadSprites();
	NewGame();
	SDL_SetColorKey(charsetBig, true, 0x000000); SDL_SetColorKey(charsetSmall, true, 0x000000);
	t1 = SDL_GetTicks();
	while (!quit) {
		Update();
		if (updateTimer >= UPDATE_RATE) {
			game.Update(updateTimer);
			Frame();
			frames++;
			updateTimer = 0;
		}
	}
}

void Gui::LoadSprites() {
	// files to be loaded as sprites
	const char* names[] = {  "assets/player.bmp", "assets/blackcar.bmp", "assets/bluecar.bmp", "assets/greencar.bmp", "assets/redcar.bmp",
							 "assets/whitecar.bmp" , "assets/yellowcar.bmp", "assets/pinkcar.bmp"};
	for (const char* name : names) {
		LoadSprite(name);
	}
}

void Gui::LoadSprite(const char* path)
{
	SDL_Surface* surface = LoadSurface(path);
	if (surface == NULL)
		return;
	SDL_Surface* *temp = new SDL_Surface*[spritesCount + 1];
	for (int i = 0; i < spritesCount; i++)
		temp[i] = sprites[i];
	temp[spritesCount] = surface;
	spritesCount++;
	delete[] sprites;
	sprites = temp;
}

// render the screen
void Gui::Frame() {
	// clear screen
	SDL_FillRect(screen, NULL, GetRGB(BACKGROUND));

	DrawRectangle(100, 100, 100, 100, GetRGB(FOREGROUND), GetRGB(FOREGROUND));
	DrawText("Hello world", 200, 200);

	// draw cars
	for (int i = 0; i < game.GetCarsAmount(); i++) {
		Car* car = game.GetCar(i);
		DrawSurface(car->GetSurface(), car->GetX(), car->GetY());
	}
	// draw player
	Player* player = game.GetPlayer();
	DrawSurface(player->GetSurface(), player->GetX(), player->GetY());

	// print game info
	char info[32];
	sprintf_s(info, "FPS: %.0lf", fps);
	DrawText(info, 5, 5, false);
	sprintf_s(info, "Time: %.1lfs", worldTime);
	DrawText(info, 5, 17, false);
	sprintf_s(info, "Speed: %.0lf", game.GetPlayer()->GetSpeed());
	DrawText(info, 5, 29, false);
	
	// render
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);

	// handle events
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_KEYDOWN) {
			Input(event.key.keysym.sym);
		}
		else if (event.type == SDL_QUIT) {
			quit = true;
		}
	}
	GameInput();
}

// user keyboard input
void Gui::Input(const SDL_Keycode key) {
	switch (key) {
	case SDLK_ESCAPE:
		quit = true; break;
	case SDLK_n:
		NewGame(); break;
	}
}

// fast input for steering
void Gui::GameInput()
{
	if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A])
		game.GetPlayer()->Left();
	if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D])
		game.GetPlayer()->Right();
	if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W])
		game.GetPlayer()->Accelerate();
	if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S])
		game.GetPlayer()->Brake();
	if (currentKeyStates[SDL_SCANCODE_SPACE])
		game.GetPlayer()->Shoot();
}

// update game state
void Gui::Update() {
	t2 = SDL_GetTicks();
	delta = (t2 - t1) / 1000.0;
	t1 = t2;
	worldTime += delta;
	fpsTimer += delta;
	updateTimer += delta;
	if (fpsTimer >= 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	}
}

// clear memory
void Gui::Exit() {
	SDL_FreeSurface(charsetBig); SDL_FreeSurface(charsetSmall);
	SDL_FreeSurface(screen);
	for (int i = 0; i < spritesCount; i++)
		SDL_FreeSurface(sprites[i]);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	delete[] sprites;
}

	
Gui::Gui(const int width, const int height) {
	game = Game::Game();
	Initialize(width, height);
}

Gui::~Gui() {
	Exit();
}