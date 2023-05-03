#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char** argv) {
	int good;
	uint32_t img_flags = IMG_INIT_PNG;
	good += SDL_Init(SDL_INIT_EVERYTHING);
	good += IMG_Init(img_flags);
	if (good != img_flags) {
		printf("bad init!\n");
		printf("%s\n", SDL_GetError());
		printf("%s\n", IMG_GetError());
	}
	SDL_Window *window = SDL_CreateWindow("Milo Bastard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect *rect = &(SDL_Rect){20,20,400,400};
	if (render == NULL) {
		printf("bad renderer!\n");
		printf("%s\n", SDL_GetError());
		return -1;
	}
	SDL_Texture *texture = IMG_LoadTexture(render, argv[1]);
	if (texture == NULL) {
		printf("No Texture?\n");
		return -1;
	}
	bool running = true;
	while (running) {
		SDL_RenderClear(render);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				running = false;
		}
		SDL_RenderCopy(render, texture, NULL, NULL);
		SDL_RenderPresent(render);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
