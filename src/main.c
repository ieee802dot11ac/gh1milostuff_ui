#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rwops.h>
#include <hmxobj/hmxbitmap.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <hmxobj/hmx.h>
#include <string.h>

int main(int argc, char** argv) {
	int good = 0;
	uint32_t img_flags = 0b00111111; // everything
	good += SDL_Init(SDL_INIT_EVERYTHING);
	good += IMG_Init(img_flags);
	if (good != img_flags) {
		printf("bad init!\n");
		printf("%s\n", SDL_GetError());
	}
	SDL_Window *window = SDL_CreateWindow("Milo Bastard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect *rect = &(SDL_Rect){0, 500, 300, 300};
	SDL_Texture *texture;

	if (render == NULL) {
		printf("bad renderer!\n");
		printf("%s\n", SDL_GetError());
		return -1;
	}
	if (argv[1] == NULL) {
		printf("No Texture?\n");
		return -1;
	}
	if (strstr(argv[1], ".bmp_ps2")) {
		HX_BITMAP bitmap = hmx_bitmap_load(fopen(argv[1], "r"));
		SDL_Color *unpaletted_bmp = malloc(hmx_bitmap_len_tex_data(bitmap));
		SDL_PixelFormat *pixelbastard = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
		for (int y = 0; y < bitmap.height; ++y) {
			for (int x = 0; x < bitmap.width; ++x) {
				u8 pixel;
				if (bitmap.bpp == 8) {
					pixel = bitmap.texData[x + y * bitmap.width];
				} else if (bitmap.bpp == 4) {
					size_t addr = (x / 2) + y * (bitmap.width / 2);
					u8 shift = (x & 1) << 2;
					u8 mask = 0xF << shift;
					pixel = (bitmap.texData[addr] & mask) >> shift;
				} else {
					fprintf(stderr, "Unsupported number of bits per pixel (%u bpp) in texture file.\n",
							bitmap.bpp);
				}
				HX_COLOR_8888 color = bitmap.colorPalette[pixel];
				color = hmx_color_8888_fix_alpha(color);
				unpaletted_bmp[x + y * bitmap.height] = (SDL_Color){color.r, color.g, color.b, color.a};
			}
		}
		SDL_Surface *nuisance = &(SDL_Surface){0, pixelbastard, bitmap.width, bitmap.height, (bitmap.width*bitmap.bpp)/8, unpaletted_bmp};
		texture = SDL_CreateTextureFromSurface(render, nuisance);
	} else {
		texture = IMG_LoadTexture(render, argv[1]);
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
		//SDL_RenderDrawRect(render, rect);
		//SDL_RenderFillRect(render, rect);
		SDL_RenderPresent(render);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
