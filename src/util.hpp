#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
