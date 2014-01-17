#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(const std::string &msg, std::ostream &os = std::cerr) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

void sdl_bomb(const std::string &msg) {
	logSDLError(msg);
	exit(-1);
}

SDL_Texture *renderText(const std::string &msg, const std::string &fontPath, SDL_Color color, int fontSize, SDL_Renderer *ren) {
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if(font == nullptr) {
		logSDLError("Unable to open font");
		return nullptr;
	}

	SDL_Surface *surface = TTF_RenderText_Blended(font, msg.c_str(), color);
	if(surface == nullptr) {
		TTF_CloseFont(font);
		logSDLError("Unable to render text to a surface");
		return nullptr;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
	if(tex == nullptr) {
		logSDLError("Unable to render surface to texture");
	}

	SDL_FreeSurface(surface);
	TTF_CloseFont(font);

	return tex;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	// If no width and height are specified, use the texture's actual width and height
	if(w == -1 || h == -1)
		SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(ren, tex, NULL, &dest);
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

	if((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG != IMG_INIT_JPG))
		sdl_bomb("Failed to load the JPG Image loading extensions");

	if(TTF_Init() != 0)
		sdl_bomb("Failed to load TTF extension");

	SDL_Texture *background = IMG_LoadTexture(ren, "../img/background.bmp");
	SDL_Texture *image = IMG_LoadTexture(ren, "../img/wolf.jpg");

	int img_width;
	int img_height;

	SDL_QueryTexture(image, NULL, NULL, &img_width, &img_height);

	int img_x = SCREEN_WIDTH/2 - img_width/2;
	int img_y = SCREEN_HEIGHT/2 - img_height/2;

	SDL_Color textColor = {255, 255, 255};
	SDL_Texture *helloWorldTex = renderText("Hello world!", "../fonts/sample.ttf", textColor, 64, ren);

	if(helloWorldTex == nullptr)
		sdl_bomb("Unable to render text to texture!");

	SDL_Event e;

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
						img_x -=3;
						break;
					case SDL_SCANCODE_RIGHT:
						img_x += 3;
						break;
					case SDL_SCANCODE_UP:
						img_y -=3;
						break;
					case SDL_SCANCODE_DOWN:
						img_y +=3;
						break;
				}
			}
		}

		SDL_RenderClear(ren);

		renderTexture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		renderTexture(image, ren, img_x, img_y);
		renderTexture(helloWorldTex, ren, 0, 0);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	IMG_Quit();
	SDL_Quit();
	return 0;
}
