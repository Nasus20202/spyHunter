#include "gui.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
#define main SDL_main

int main(int argc, char** argv) {
	Gui gui = Gui::Gui();
	gui.Initialize();
	return 0;
}