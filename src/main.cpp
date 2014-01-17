#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "main.hpp"
#include "util.hpp"

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int points;
	int speed;
} player;

int main(int argc, char* argv[]) {

	std::cout << "Starting SDL Application..." << std::endl;
	SDL_Event e;
	SDL_Renderer *ren = nullptr;
	SDL_Window *win = nullptr;

	Initialise(&ren,&win);

	int board_width;
	int board_height;
	SDL_Texture *pongBoard = IMG_LoadTexture(ren, "../img/pong_board.png");
	SDL_QueryTexture(pongBoard, NULL, NULL, &board_width, &board_height);

	SDL_Color textColor = {255, 255, 255};
	SDL_Texture *helloWorldTex = renderText("Welcome to Pong!", "../fonts/sample.ttf", textColor, 30, ren);

	if(helloWorldTex == nullptr)
		sdl_bomb("Unable to render text to texture!");

	// Define Players
	player p1;
	player p2;

	p1.width = p2.width = board_width;
	p1.height = p2.height = 150;
	p1.speed = p2.speed = 10;

	p1.x = board_width/2 + 10;
	p2.x = SCREEN_WIDTH - p2.width - 10 - p2.width/2;

	p1.y = SCREEN_HEIGHT/2 - p1.height/2;
	p2.y = SCREEN_HEIGHT/2 - p2.height/2;

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
						p1.y -= p1.speed;
						break;
					case SDL_SCANCODE_DOWN:
						p1.y += p1.speed;
						break;
				}
			}
		}

		SDL_RenderClear(ren);

		renderTexture(pongBoard, ren, p1.x, p1.y, p1.width, p1.height);
		renderTexture(pongBoard, ren, p2.x, p2.y, p1.width, p1.height);

		renderTexture(helloWorldTex, ren, 0, 0);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(pongBoard);
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

	 	const int flags = IMG_INIT_PNG | IMG_INIT_JPG;
		if(IMG_Init(flags) !=flags)
			sdl_bomb("Failed to load the Image loading extensions");

		if(TTF_Init() != 0)
			sdl_bomb("Failed to load TTF extension");
 }

 void Cleanup(SDL_Renderer **ren, SDL_Window **win) {
		SDL_DestroyRenderer(*ren);
		SDL_DestroyWindow(*win);

		IMG_Quit();
		SDL_Quit();
 }
