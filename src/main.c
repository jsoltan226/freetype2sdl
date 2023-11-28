#include "fonts.h"
#include "freetype/freetype.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define SOURCE_CODE_PRO "assets/SourceCodePro-Regular.otf"
#define LIBERATION_MONO "assets/LiberationMono-Regular.ttf" 
#define FREE_SANS       "assets/FreeSansBold.otf"

#define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
char FONT_PATH[FNT_TEXT_BUFFER_SIZE];


int main(int argc, char **argv)
{
    strncpy(FONT_PATH, LIBERATION_MONO, FNT_TEXT_BUFFER_SIZE - 1);
    if(argc == 2){
        switch(argv[1][0]){
            default: case '1':
            	break;
            case '2':
                strncpy(FONT_PATH, SOURCE_CODE_PRO, FNT_TEXT_BUFFER_SIZE - 1);
            	break;
            case '3':
                strncpy(FONT_PATH, FREE_SANS, FNT_TEXT_BUFFER_SIZE - 1);
            	break;
        }
    }
    FONT_PATH[FNT_TEXT_BUFFER_SIZE - 1] = '\0';

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
            "font test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_OPENGL
        );
    SDL_Renderer *renderer = SDL_CreateRenderer(
            window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
        );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    fnt_Font *font = fnt_initFont(FONT_PATH, renderer, 50.f, 100.f, FNT_CHARSET_ASCII);
    assert(font != NULL);

    int time = 0;
    bool running = true;
    bool keyI = false, keyD = false;
    while(running){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
                goto breakout;
            } else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_i){
                    keyI = true;
                } else if(event.key.keysym.sym == SDLK_d) {
                    keyD = true;
                }
            } else if(event.type == SDL_KEYUP){
                if(event.key.keysym.sym == SDLK_i){
                    keyI = false;
                } else if(event.key.keysym.sym == SDLK_d) {
                    keyD = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        fnt_setTextColor(font, 128, 128, time/25 % 255, 255);
        if(time % 100){
            if(keyI)
                font->lineHeight++;
            if(keyD)
                font->lineHeight--;
        }

        fnt_drawText(font, renderer, 0, 0, "Test\n12\t4,+_");

        fnt_setTextColor(font, (int)((time + 100)*0.8) % 255, time % 100, time %255, time %10 + 200);
        fnt_drawText(font, renderer, 200, 330, "TEST");
        SDL_RenderPresent(renderer);
        time++;
        SDL_Delay(16);

breakout: continue;
    }

    fnt_destroyFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
