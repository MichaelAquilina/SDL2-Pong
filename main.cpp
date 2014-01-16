#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(const std::string &msg, std::ostream &os = std::cout) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}


void sdl_bomb(const std::string &msg) {
	logSDLError(msg, std::cerr);
	exit(-1);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	if(w == -1 || h == -1)
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	else
	{
		dest.w = w;
		dest.h = h;
	}

	SDL_RenderCopy(ren, tex, NULL, &dest);
}

SDL_Texture* loadTexture(const std::string &path, SDL_Renderer *ren) {
	SDL_Texture *tex = nullptr;
	SDL_Surface *bmp = SDL_LoadBMP(path.c_str());

	if(bmp != nullptr) {
		tex = SDL_CreateTextureFromSurface(ren, bmp);
		SDL_FreeSurface(bmp);

		if(tex == nullptr)
			logSDLError("Failed to Create Texture from surface");
	}
	else logSDLError("Failed to load image to surface");

	return tex;
}


int main(int argc, char* argv[]) {

	std::cout << "Starting SDL Application..." << std::endl;

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		sdl_bomb("Failed to Initialise SDL");

	SDL_Window *win = nullptr;
	win = SDL_CreateWindow(
			"Hello World!",
			100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
	);
	if(win == nullptr)
		sdl_bomb("Failed to create SDL Window");

	SDL_Renderer *ren = nullptr;
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr)
		sdl_bomb("Failed to create SDL Renderer");

	SDL_Texture *background = loadTexture("background.bmp", ren);
	SDL_Texture *image = loadTexture("image.bmp", ren);

	SDL_RenderClear(ren);

	renderTexture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	int img_width;
	int img_height;
	SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);
	renderTexture(image, ren, SCREEN_WIDTH/2 - img_width/2, SCREEN_HEIGHT/2 - img_height/2);

	SDL_RenderPresent(ren);

	SDL_Delay(2000);

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
