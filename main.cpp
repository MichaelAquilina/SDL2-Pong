#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>

void sdl_bomb() {
	std::cerr << SDL_GetError() << std::endl;
	exit(-1);
}


int main(int argc, char* argv[]) {

	std::cout << "Starting SDL Application..." << std::endl;

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		sdl_bomb();

	SDL_Window *win = nullptr;
	win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(win == nullptr)
		sdl_bomb();

	SDL_Renderer *ren = nullptr;
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr)
		sdl_bomb();

	SDL_Surface *bmp = nullptr;
	bmp = SDL_LoadBMP("hello.bmp");
	if(bmp == nullptr)
		sdl_bomb();

	SDL_Texture *tex = nullptr;
	tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);

	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);

	SDL_Delay(2000);

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
