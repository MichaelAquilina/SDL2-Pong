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
	int vx;
	int vy;
} ball;

typedef struct {
	int x;
	int y;
	int width;
	int height;
	int score;
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

	// Define Players
	player p1;
	player p2;

	// Define Ball
	ball b;
	b.x = SCREEN_WIDTH / 2;
	b.y = SCREEN_HEIGHT / 2;
	b.vx = -4;
	b.vy = -2;

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
	char buffer[512];

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
		}

		// Update Ball coordinates
		b.x += b.vx;
		b.y += b.vy;

		if(b.x < 0) {
			b.x = 0;
			b.vx *= -1;
		}
		if(b.x > SCREEN_WIDTH) {
			b.x = SCREEN_WIDTH;
			b.vx *= -1;
		}
		if(b.y < 0) {
			b.y = 0;
			b.vy *= -1;
		}
		if(b.y > SCREEN_HEIGHT) {
			b.y = SCREEN_HEIGHT;
			b.vy *= -1;
		}

		// Boundary Collision
		if(p1.y < 0) p1.y = 0;
		if(p1.y + p1.height > SCREEN_HEIGHT) p1.y = SCREEN_HEIGHT - p1.height;
		if(p2.y < 0) p2.y = 0;
		if(p2.y + p2.height > SCREEN_HEIGHT) p2.y = SCREEN_HEIGHT - p2.height;

		SDL_RenderClear(ren);

		renderTexture(pongBoard, ren, p1.x, p1.y, p1.width, p1.height);
		renderTexture(pongBoard, ren, p2.x, p2.y, p1.width, p1.height);

		// Draw the center line
		renderTexture(pongBoard, ren, SCREEN_WIDTH/2 - CENTER_WIDTH/2, 0, CENTER_WIDTH, SCREEN_HEIGHT);

		// Draw the Ball
		renderTexture(pongBoard, ren, b.x - BALL_WIDTH/2, b.y - BALL_HEIGHT/2, BALL_WIDTH, BALL_HEIGHT);

		// Display the score
		sprintf(buffer, "%d", p1.score);
		SDL_Texture *p1score = renderText(buffer, "../fonts/sample.ttf", whiteColor, 40, ren);
		sprintf(buffer, "%d", p2.score);
		SDL_Texture *p2score = renderText(buffer, "../fonts/sample.ttf", whiteColor, 40, ren);

		int width;
		SDL_QueryTexture(p1score, NULL, NULL, &width, NULL);

		renderTexture(p1score, ren, SCREEN_WIDTH/2 - width - 10, 10);
		renderTexture(p1score, ren, SCREEN_WIDTH/2 + 10, 10);

		SDL_DestroyTexture(p1score);
		SDL_DestroyTexture(p2score);

		// Extremely ineffecient way of displaying text
		sprintf(buffer, "%.0f", fps);
		SDL_Texture *fpsCounter = renderText(buffer, "../fonts/sample.ttf", whiteColor, 20, ren);
		renderTexture(fpsCounter, ren, SCREEN_WIDTH - 20, 0);
		SDL_DestroyTexture(fpsCounter);

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
