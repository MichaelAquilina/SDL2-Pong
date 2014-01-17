#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "main.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {

	std::cout << "Starting SDL Application..." << std::endl;
	SDL_Event e;
	SDL_Renderer *ren = nullptr;
	SDL_Window *win = nullptr;

	Initialise(&ren,&win);

	SDL_Color textColor = {255, 255, 255};
	SDL_Texture *helloWorldTex = renderText("Welcome to Pong!", "../fonts/sample.ttf", textColor, 30, ren);

	if(helloWorldTex == nullptr)
		sdl_bomb("Unable to render text to texture!");

	std::cout << "Starting Game Loop" << std::endl;
	bool quit = false;

	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT)  quit = true;
			if(e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						quit = true;
						break;
					case SDL_SCANCODE_LEFT:
						break;
					case SDL_SCANCODE_RIGHT:
						break;
					case SDL_SCANCODE_UP:
						break;
					case SDL_SCANCODE_DOWN:
						break;
				}
			}
		}

		SDL_RenderClear(ren);

		renderTexture(helloWorldTex, ren, 0, 0);

		SDL_RenderPresent(ren);
	}

	Cleanup(&ren, &win);
	return 0;
}

 void Initialise(SDL_Renderer **ren, SDL_Window **win) {
		if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
			sdl_bomb("Failed to Initialise SDL");

	 	 *win = SDL_CreateWindow(
	 			"SDL Pong - Michael Aquilina",
	 			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	 			SCREEN_WIDTH, SCREEN_HEIGHT,
	 			SDL_WINDOW_SHOWN
	 	);
	 	if(*win == nullptr)
	 		sdl_bomb("Failed to create SDL Window");

	 	*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	 	if(*ren == nullptr)
	 		sdl_bomb("Failed to create SDL Renderer");

		if((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG != IMG_INIT_JPG))
			sdl_bomb("Failed to load the JPG Image loading extensions");

		if(TTF_Init() != 0)
			sdl_bomb("Failed to load TTF extension");
 }

 void Cleanup(SDL_Renderer **ren, SDL_Window **win) {
		SDL_DestroyRenderer(*ren);
		SDL_DestroyWindow(*win);

		IMG_Quit();
		SDL_Quit();
 }
