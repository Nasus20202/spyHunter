#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <direct.h>
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

void Gui::DrawText(SDL_Surface* screen, Point point, const char* text, SDL_Surface* charset, int fontSize) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = s.h = d.w = d.h = fontSize;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * fontSize;
		py = (c / 16) * fontSize;
		s.x = px;
		s.y = py;
		d.x = point.x;
		d.y = point.y;
		SDL_BlitSurface(charset, &s, screen, &d);
		point.x += fontSize;
		text++;
	};
};
void Gui::DrawText(const char* text, const Point point, bool big) {
	DrawText(screen, point, text, big ? charsetBig : charsetSmall, big ? FONT_SIZE_BIG : FONT_SIZE_SMALL);
}

void Gui::DrawPixel(SDL_Surface* surface, const Point point, Uint32 color)
{
	if (point.x < 0 || point.x >= surface->w || point.y < 0 || point.y >= surface->h) return;
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + point.y * surface->pitch + point.x * bpp;
	*(Uint32*)p = color;
}
void Gui::DrawPixel(const Point point, Uint32 color)
{
	DrawPixel(screen, point, color);
}

void Gui::DrawLine(SDL_Surface* screen, const Point point, const int length, const int dx, const int dy, Uint32 color)
{
	for (int i = 0; i < length; i++)
		DrawPixel(screen, { point.x + i * dx, point.y + i * dy }, color);
}
void Gui::DrawLine(const Point point, const int length, const int dx, const int dy, Uint32 color)
{
	DrawLine(screen, point, length, dx, dy, color);
}

void Gui::DrawRectangle(SDL_Surface* screen, const Point point, const int width, const int height, Uint32 color)
{
	SDL_Rect r;
	r.x = point.x;
	r.y = point.y;
	r.w = width;
	r.h = height;
	SDL_FillRect(screen, &r, color);
}
void Gui::DrawRectangle(const Point point, const int width, const int height, Uint32 color)
{
	DrawRectangle(screen, point, width, height, color);
}

void Gui::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, const Point point)
{
	SDL_Rect dest;
	dest.x = point.x - sprite->w / 2;
	dest.y = point.y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}
void Gui::DrawSurface(SDL_Surface* sprite, const Point point)
{
	DrawSurface(screen, sprite, point);
}

void Gui::NewGame()
{
	game->NewGame();
}

void Gui::Pause() {
	State state = game->GetState();
	if (state == State::playing)
		game->SetState(State::paused);
	else if (state == State::paused)
		game->SetState(State::playing);
}

void Gui::SaveGame() {
	if (_mkdir(SAVES_FOLDER) && errno != EEXIST) {
			printf("Unable to create folder: %s\n", SAVES_FOLDER);
			return;
	}
	const int size = 256;
	char name[size], timeStr[size]; FILE* file;
	time_t rawTime; struct tm* timeInfo = new tm();
	time(&rawTime);
	localtime_s(timeInfo, &rawTime);
	strftime(timeStr, size, "%Y-%m-%d-%H-%M-%S", timeInfo); // create time format
	sprintf_s(name, size, "%s%s%s", SAVES_FOLDER, timeStr, SAVE_EXTENSION); // create file name
	fopen_s(&file, name, "wb");
	if (file == NULL) {
		printf("Unable to open file for writing: %s\n", name);
		return;
	}
	SaveToFile(file);
	fclose(file);
	int savesCount = 0;
	fopen_s(&file, SAVES_FILE, "rb");
	if (file != NULL)
		fread(&savesCount, sizeof(int), 1, file);
	savesCount++;
	char** saves = new char* [savesCount];
	for (int i = 0; i < savesCount; i++) {
		saves[i] = new char[size];
		for (int j = 0; j < size; j++)
			saves[i][j] = '\0';
	}
	if (file != NULL) {
		for (int i = 0; i < savesCount-1; i++)
			for (int j = 0; j < size; j++)
				fread(&saves[i][j], sizeof(char), 1, file);
		fclose(file);
	}
	strcpy_s(saves[savesCount-1], size, timeStr);
	fopen_s(&file, SAVES_FILE, "wb");
	if (file == NULL) {
		printf("Unable to open file for writing: %s\n", SAVES_FILE);
		return;
	}
	fwrite(&savesCount, sizeof(int), 1, file);
	for (int i = 0; i < savesCount; i++)
		for (int j = 0; j < size; j++)
			fwrite(&saves[i][j], sizeof(char), 1, file);
	fclose(file);
	for (int i = 0; i < savesCount; i++)
		delete[] saves[i];
	delete[] saves;
}

void Gui::LoadGame() {
	FILE* file; const int size = 256; // max file name length
	char name[size+1], input[size+1] = ""; int fileCount = 0, counter = 0; char** files;
	fopen_s(&file, SAVES_FILE, "rb");
	if (file == NULL) {
		printf("Unable to open file for reading: %s\n", SAVES_FILE);
		return;
	}
	fread(&fileCount, sizeof(fileCount), 1, file);
	files = new char* [fileCount];
	for (int i = 0; i < fileCount; i++) {
		const int id = fileCount - i - 1;
		files[id] = new char[size];
		for (int j = 0; j < size; j++)
			fread(&files[id][j], sizeof(char), 1, file);
	}
	fclose(file);
	
	// create menu
	int selected = 0; bool quit = false, accept = false; bool changed = true;
	while (!quit && !accept) {
		while (SDL_PollEvent(&event) != 0 && event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				selected--;
				if (selected < 0)
					selected = fileCount - 1;
				changed = true;
				break;
			case SDLK_DOWN:
				selected++;
				if (selected >= fileCount)
					selected = 0;
				changed = true;
				break;
			case SDLK_RETURN:
				accept = true;
				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
		DrawRectangle({ 0, 0 }, SCREEN_WIDTH, SCREEN_HEIGHT, FOREGROUND); //background
		const int lineHeight = FONT_SIZE_BIG;
		const int linesPerScreen = (SCREEN_HEIGHT - 5 * lineHeight) / (double)lineHeight; int printedLine = 0;
		for (int i = 0; i < fileCount; i++) {
			if ((i > selected - linesPerScreen / 2 && i < selected + linesPerScreen / 2) ||
						(selected < linesPerScreen / 2 && i < linesPerScreen) ||
						(selected > fileCount - linesPerScreen / 2 && i > fileCount - linesPerScreen)) {
				const int y = printedLine * lineHeight;
				if (i == selected)
					DrawText(">", { 10, 50 + y });
				DrawText(files[i], { 30, 50 + y });
				printedLine++;
			}
		}
		char text[64];
		sprintf_s(text, "Wczytaj gre (%d zapisow)", fileCount);
		DrawText(text, { 30, 20 });
		if (changed) {
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		changed = false;
	}
	if (selected >= 0 && selected < fileCount)
		strcpy_s(input, size, files[selected]);
	else
		quit = true;
	for (int i = 0; i < fileCount; i++)
		delete[] files[i];
	delete[] files;
	if (quit) {
		return;
	}
	sprintf_s(name, size, "%s%s%s", SAVES_FOLDER, input, SAVE_EXTENSION);
	fopen_s(&file, name, "rb");
	if (file == NULL) {
		printf("Unable to open file for reading: %s\n", name);
		return;
	}
	LoadFromFile(file);
	fclose(file);
}

// create and customize GUI
void Gui::Initialize(const char* title) {
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
	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(window, title);
	screen = SDL_CreateRGBSurface(0, width, height, 32 ,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);
	SDL_ShowCursor(SDL_DISABLE);
	currentKeyStates = SDL_GetKeyboardState(NULL);
	
	charsetBig = LoadSurface(CHARSET_BIG);
	charsetSmall = LoadSurface(CHARSET_SMALL);
	LoadSprites();
	LoadResults();
	game->SetSprites(sprites, spritesCount);
	srand(time(NULL));
	NewGame();
	SDL_SetColorKey(charsetBig, true, 0x000000); SDL_SetColorKey(charsetSmall, true, 0x000000);
	t1 = SDL_GetTicks();
	while (game->GetState() != State::quit) {
		Update();
	}
}

// load all sprites
void Gui::LoadSprites() {
	const char* names[] = { ASSETS };
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

	// draw road
	PrintMap();

	// draw missles
	for (int i = 0; i < game->GetMissilesAmount(); i++) {
		Car* missle = game->GetMissile(i);
		DrawSurface(missle->GetSurface(), { missle->GetX(), missle->GetY() });
	}
	// draw cars
	for (int i = 0; i < game->GetCarsAmount(); i++) {
		Car* car = game->GetCar(i);
		DrawSurface(car->GetSurface(), { car->GetX(), car->GetY() });
	}
	// draw player
	Player* player = game->GetPlayer();
	if(game->GetState() != State::dead)
		DrawSurface(player->GetSurface(), { player->GetX(), player->GetY() });

	// fps, time, score, game state...
	PrintGameInfo();
	
	// render
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}

// user keyboard input
void Gui::Input(const SDL_Keycode key) {
	switch (key) {
	case SDLK_ESCAPE:
		game->SetState(State::quit); break;
	case SDLK_n:
		NewGame(); break;
	case SDLK_p:
		Pause(); break;
	case SDLK_s:
		SaveGame(); break;
	case SDLK_l:
		menu = true; LoadGame(); break;
	case SDLK_f:
		game->SetState(State::finished); break;
	case SDLK_r:
		menu = true; ShowResults(); break;
	case SDLK_SPACE:
		if(game->GetState() == State::playing)
			game->Shoot(); break;
	}
}

// fast input for steering
void Gui::GameInput()
{
	if (currentKeyStates[SDL_SCANCODE_LEFT])
		game->GetPlayer()->Left();
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
		game->GetPlayer()->Right();
	if (currentKeyStates[SDL_SCANCODE_UP])
		game->GetPlayer()->Accelerate();
	if (currentKeyStates[SDL_SCANCODE_DOWN])
		game->GetPlayer()->Brake();
}

// update game state
void Gui::Update() {
	t2 = SDL_GetTicks();
	double delta = (t2 - t1) / 1000.0;
	t1 = t2;
	if (menu) {
		delta = 0;
		updateTimer = 0;
		menu = false;
	}
	fpsTimer += delta;
	updateTimer += delta;
	frameTimer += delta;
	// count fps
	if (fpsTimer >= 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	}
	// update game state
	State state = game->GetState();
	if (updateTimer >= updateTime) {
		// handle events
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_KEYDOWN) {
				Input(event.key.keysym.sym); // key down event
			}
			else if (event.type == SDL_QUIT) {
				game->SetState(State::quit);
			}
		}
		if (game->GetState() == State::quit)
			return;
		// handle pressed keys, this happens every game update
		if (game->GetState() == State::playing) {
			GameInput();
			game->Update(updateTimer); // update game state
		}
		updateTimer = 0;
	}
	// update screen, game update has priority
	if (frameTimer >= frameTime) {
		Frame();
		frames++;
		frameTimer = 0;
	}
	if (state != game->GetState() && (game->GetState() == State::finished || game->GetState() == State::dead)) {
		AddResult(Result(game->GetScore(), game->GetTime()));
	}
}

void Gui::PrintGameInfo() {
	// left top
	int x = 5, y = 5, dy = 12;
	char info[128];
	sprintf_s(info, "Krzysztof Nasuta");
	DrawText(info, Point(x, y), false);
	sprintf_s(info, "193328");
	DrawText(info, Point(x, y + dy), false);
	x = width / 2 - 150, y = height / 2;
	switch (game->GetState()) {
	case State::paused:
		sprintf_s(info, "       PAUZA"); break;
	case State::dead:
	case State::finished:
		sprintf_s(info, "KONIEC GRY (wynik %d)", game->GetScore()); break;
	default:
		sprintf_s(info, ""); break;
	}
	DrawText(info, Point(x, y));
	// right top
	x = width, y = -2;
	//print shoot cooldown
	for (int i = game->GetShootCooldown(true) / 10; i > 0; i--)
		DrawText("-", { x - i * 9, y });
	// print hearths
	for (int i = game->GetLives(); i > 0; i--) {
		DrawText("\3", { x - i * 20 - 5 , y + dy});
	}
	// print weapon type
	const int ammo = game->GetPlayer()->GetAmmo();
	switch (game->GetPlayer()->GetAmmoType())
	{
	case AmmoType::missile:
		sprintf_s(info, "       Rakiety"); break;
	case AmmoType::multiMissile:
		sprintf_s(info, "%dx MultiRakiety", ammo); break;
	case AmmoType::bomb:
		sprintf_s(info, "     %dx Bomby", ammo); break;
	case AmmoType::laser:
		sprintf_s(info, "     %dx Laser", ammo); break;
	}
	DrawText(info, { x - 120 , y + 3 * dy }, false);
	// left bottom
	
	// right bottom
	x = width - 145, y = height - 80, dy = 20;
	sprintf_s(info, "%d pkt.", game->GetScore());
	DrawText(info, { x, y });
	sprintf_s(info, "%d km/h", (int)(game->GetPlayer()->GetSpeed()*0.35));
	DrawText(info, { x, y + dy});
	sprintf_s(info, "%ds", (int)game->GetTime());
	DrawText(info, { x, y + 2 * dy });
	sprintf_s(info, "%d FPS", (int)fps);
	DrawText(info, { x, y + 3 * dy },false);
	sprintf_s(info, "a-n");
	DrawText(info, { x, (int)(y + 3.5 * dy) }, false);
}

void Gui::PrintMap()
{
	const int mapWidth = game->GetMapWidth(), mapHeight = game->GetMapHeight();
	const double blockWidth = width / (double)mapWidth, blockHeight = height / (double)mapHeight;
	MapTile tile;
	for (int y = 0; y < mapHeight; y++) {
		int roadWidth = 0;
		for (int x = 0; x < mapWidth; x++) {
			bool drawRoad = false;
			tile = game->GetMapTile(x, y);
			switch (tile) {
			case MapTile::road:
				roadWidth++;
				break;
			case MapTile::stripes:
				DrawRectangle({ (int)(x * blockWidth), (int)(y * blockHeight) }, blockWidth, blockHeight + 1, GetRGB(WHITE)); drawRoad = true;
				break;
			case MapTile::powerUp:
				DrawRectangle({ (int)(x * blockWidth), (int)(y * blockHeight) }, blockWidth, blockHeight + 1, GetRGB(YELLOW)); drawRoad = true;
				break;
			default:
				drawRoad = true;
				break;
			}
			if (drawRoad && roadWidth > 0) {
				DrawRectangle({ (int)((x - roadWidth) * blockWidth), (int)(y * blockHeight) }, roadWidth * blockWidth, blockHeight + 1, GetRGB(FOREGROUND));
				roadWidth = 0;
				drawRoad = false;
			}
		}
		if (roadWidth > 0) {
			DrawRectangle({ (int)((mapWidth - roadWidth) * blockWidth), (int)(y * blockHeight) }, roadWidth * blockWidth, blockHeight + 1, GetRGB(FOREGROUND));
		}
	}
}

// clear memory
void Gui::Exit() {
	SaveResults();
	SDL_FreeSurface(charsetBig); SDL_FreeSurface(charsetSmall);
	SDL_FreeSurface(screen);
	for (int i = 0; i < spritesCount; i++)
		SDL_FreeSurface(sprites[i]);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	delete[] sprites;
	delete[] results;
	delete game;
}

void Gui::SaveToFile(FILE* file) {
	game->SaveToFile(file);
}

void Gui::LoadFromFile(FILE* file) {
	game->LoadFromFile(file);
}

	
Gui::Gui(const int width, const int height, const double updateTime, const double frameTime, const char* title) {
	game = new Game(width, height);
	this->width = width;
	this->height = height;
	this->updateTime = updateTime;
	this->frameTime = frameTime;
}

Gui::~Gui() {
	Exit();
}

void Gui::AddResult(Result result)
{
	Result* newResults = new Result[resultsCount + 1];
	for (int i = 0; i < resultsCount; i++)
		newResults[i] = results[i];
	newResults[resultsCount] = result;
	delete[] results;
	results = newResults;
	resultsCount++;
}

void Gui::ShowResults() {
	SortResults();
	bool quit = false, sortedByPoints = true; bool changed = true; int selected = 0; char text[128];
	const int lineHeight = FONT_SIZE_BIG;
	const int linesPerScreen = (SCREEN_HEIGHT - 5 * lineHeight) / (double)lineHeight;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0 && event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_p:
				SortResults(); changed = true; selected = 0; sortedByPoints = true;
				break;
			case SDLK_t:
				SortResults(true); changed = true; selected = 0; sortedByPoints = false;
				break;
			case SDLK_UP:
				if (selected > 0) {
					selected--;
					changed = true;
				}
				break;
			case SDLK_DOWN:
				if (selected < resultsCount - linesPerScreen - 1) {
					selected++;
					changed = true;
				}
				break;
			case SDLK_RETURN:
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
		DrawRectangle({ 0, 0 }, SCREEN_WIDTH, SCREEN_HEIGHT, FOREGROUND); //background
		int printedLine = 0;
		for (int i = 0; i < resultsCount; i++) {
			if ((i >= selected) && (i <= selected + linesPerScreen)) {
				const int y = printedLine * lineHeight;
				if(sortedByPoints)
					sprintf_s(text, "%d. %d pkt. (%.1lfs)", i + 1, results[i].points, results[i].time);
				else
					sprintf_s(text, "%d. %.1lfs (%d pkt)", i + 1, results[i].time, results[i].points);
				DrawText(text, { 30, 50 + y });
				printedLine++;
			}
		}
		if(sortedByPoints)
			sprintf_s(text, "Wyniki posortowane po liczbie punktow (%d)", resultsCount);
		else
			sprintf_s(text, "Wyniki posortowane po czasie (%d)", resultsCount);
		DrawText(text, { 30, 20 });
		if (changed) {
			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		changed = false;
	}
}

void Gui::SortResults(bool byTime)
{
	for (int i = 0; i < resultsCount; i++)
		for (int j = i + 1; j < resultsCount; j++)
			if (byTime ? results[i].time < results[j].time : results[i].points < results[j].points) {
				Result temp = results[i];
				results[i] = results[j];
				results[j] = temp;
			}
}

void Gui::LoadResults()
{
	FILE* file;
	fopen_s(&file, RESULTS_FILE, "rb");	
	if (file == NULL) {
		printf("Unable to open file for reading: %s\n", RESULTS_FILE);
		return;
	}
	delete[] results;
	fread(&resultsCount, sizeof(resultsCount), 1, file);
	results = new Result[resultsCount];
	for (int i = 0; i < resultsCount; i++)
		fread(&results[i], sizeof(Result), 1, file);
	fclose(file);
}

void Gui::SaveResults()
{
	FILE* file;
	fopen_s(&file, RESULTS_FILE, "wb");
	if (file == NULL) {
		printf("Unable to open file for writing: %s\n", RESULTS_FILE);
		return;
	}
	fwrite(&resultsCount, sizeof(resultsCount), 1, file);
	for (int i = 0; i < resultsCount; i++)
		fwrite(&results[i], sizeof(Result), 1, file);
	fclose(file);
}

Point::Point(const int x, const int y) : x(x), y(y) {}

Result::Result(const int points, const double time) : points(points), time(time) {}
