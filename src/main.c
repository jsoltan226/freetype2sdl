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

#define WINDOW_TITLE    "FreeType 2 SDL"
#define WINDOW_X        SDL_WINDOWPOS_CENTERED
#define WINDOW_Y        SDL_WINDOWPOS_CENTERED
#define WINDOW_WIDTH    500
#define WINDOW_HEIGHT   500
#define WINDOW_FLAGS    SDL_WINDOW_OPENGL

#define RENDERER_FLAGS  (SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE)

#define LINE_HEIGHT     100.f
#define CHAR_W          50.f

#define TEXT1           "Test\n12\t4,+_"
#define TEXT1_X         0
#define TEXT1_Y         0
#define TEXT2           "TEST"
#define TEXT2_X         200
#define TEXT2_Y         330

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
        WINDOW_TITLE, WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS
    );
    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, RENDERER_FLAGS);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    fnt_Font *font = fnt_initFont(FONT_PATH, renderer, CHAR_W, LINE_HEIGHT, FNT_CHARSET_ASCII);
    assert(font != NULL);

    int time = 0;
    bool running = true;
    bool keyI = false, keyD = false;

    int newlinesText1 = 0, newlinesText2 = 0, strWText1 = 0, strWText2 = 0;
    char *text1 = TEXT1;
    char *text2 = TEXT2;

    int currentTextW = 0;
    int i = 0;
    while(text1[i]){
        i++;
        currentTextW++;
        if(text1[i] == '\n'){
            newlinesText1++;
            strWText1 = currentTextW > strWText1 ? currentTextW : strWText1;
            currentTextW = 0;
        } else if(text1[i] == '\t'){
            currentTextW += i - (i % FNT_DEFAULT_TAB_WIDTH);
        }
    }
    strWText1 = currentTextW > strWText1 ? currentTextW : strWText1;

    currentTextW = 0;
    i = 0;
    while(text2[i]){
        i++;
        currentTextW++;
        if(text2[i] == '\n'){
            newlinesText2++;
            strWText2 = currentTextW > strWText2 ? currentTextW : strWText2;
            currentTextW = 0;
        } else if(text2[i] == '\t'){
            currentTextW += i - (i % FNT_DEFAULT_TAB_WIDTH);
        }
    }
    strWText2 = currentTextW > strWText2 ? currentTextW : strWText2;

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

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect destRect = {
            .x = TEXT1_X,
            .y = TEXT1_Y,
            .w = strWText1 * CHAR_W,
            .h = LINE_HEIGHT * (newlinesText1 + 1),
        };
        fnt_drawText(font, renderer, TEXT1_X, TEXT1_Y, TEXT1);
        SDL_RenderDrawRect(renderer, &destRect);

        destRect = (SDL_Rect){
            .x = TEXT2_X,
            .y = TEXT2_Y,
            .w = strWText2 * CHAR_W,
            .h = LINE_HEIGHT * (newlinesText2 + 1),
        };
        fnt_setTextColor(font, (int)((time + 100)*0.8) % 255, time % 100, time %255, time %10 + 200);
        fnt_drawText(font, renderer, TEXT2_X, TEXT2_Y, TEXT2);
        SDL_RenderDrawRect(renderer, &destRect);
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
