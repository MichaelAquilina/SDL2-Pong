#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "pong.hpp"
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

	SDL_Color whiteColor = {255, 255, 255};
	SDL_Surface *fpsCounter;
	TTF_Font *sampleFont = TTF_OpenFont("../fonts/sample.ttf", 20);

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

	uint prevTime = SDL_GetTicks();
	bool quit = false;
	int frames = 0;
	float fps;

	while(!quit) {

		// FPS Calculation
		++frames;
		uint currTime = SDL_GetTicks();
		float elapsed = (currTime - prevTime);

		if(elapsed > 100) {
			fps = round(frames / (elapsed / 1000.0));
			frames = 0;
			prevTime = currTime;
		}

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

			// Boundary Collision
			if(p1.y < 0) p1.y = 0;
			if(p1.y + p1.height > SCREEN_HEIGHT) p1.y = SCREEN_HEIGHT - p1.height;
		}

		SDL_RenderClear(ren);

		renderTexture(pongBoard, ren, p1.x, p1.y, p1.width, p1.height);
		renderTexture(pongBoard, ren, p2.x, p2.y, p1.width, p1.height);

		// Extremely ineffecient way of displaying text
		char fpsBuffer[10];
		sprintf(fpsBuffer, "%.0f", fps);
		fpsCounter = TTF_RenderText_Blended(sampleFont, fpsBuffer, whiteColor);
		SDL_Texture *fpsTexture = SDL_CreateTextureFromSurface(ren, fpsCounter);
		SDL_FreeSurface(fpsCounter);
		renderTexture(fpsTexture, ren, SCREEN_WIDTH - 20, 0);
		SDL_DestroyTexture(fpsTexture);

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
	 			"SDL Pong by Michael Aquilina",
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

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
 }
