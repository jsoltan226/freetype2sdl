#include "fonts.h"
#include <cgd/user-input/keyboard.h>
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
#define WINDOW_WIDTH    600
#define WINDOW_HEIGHT   600
#define WINDOW_FLAGS    SDL_WINDOW_OPENGL

#define RENDERER_FLAGS  (SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE)

#define KEYBOARD_N_KEYS 7

#define LINE_HEIGHT     100.f
#define CHAR_W          50.f

#define TEXT1           "Test\t*\n12\t4,+_"
#define TEXT1_X         0
#define TEXT1_Y         0
#define TEXT2           "TES\tT"
#define TEXT2_X         200
#define TEXT2_Y         330
#define TEXT3           "1234567890"
#define TEXT3_X         0
#define TEXT3_Y         (WINDOW_HEIGHT - LINE_HEIGHT - 1)

#define MOVEMENT_SPEED  3
#define RESIZE_SPEED    1

#define SOURCE_CODE_PRO "assets/SourceCodePro-Regular.otf"
#define LIBERATION_MONO "assets/LiberationMono-Regular.ttf" 
#define FREE_SANS       "assets/FreeSansBold.otf"

#define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
char FONT_PATH[FNT_TEXT_BUFFER_SIZE];

kb_Keyboard *keyboard;

int time = 0;
bool running = true;
bool displayTextRects = true, displayCharRects = true, displayGlyphRects = true;
fnt_Vector2D textOffset = { .x = 0, .y = 0 };

int max(int a, int b);

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

    fnt_Font *font = fnt_initFont(FONT_PATH, renderer, CHAR_W, LINE_HEIGHT, FNT_CHARSET_ASCII, 
            FNT_FLAG_DISPLAY_GLYPH_RECTS | FNT_FLAG_DISPLAY_CHAR_RECTS | FNT_FLAG_DISPLAT_TEXT_RECTS);
    assert(font != NULL);

    keyboard = kb_initKeyboard();
    assert(keyboard != NULL);

    while(running){

        SDL_Event event;

        kb_updateKeyboard(keyboard);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
            }
        }
        if(kb_getKey(keyboard, KB_KEYCODE_W)->pressed)
            textOffset.y -= MOVEMENT_SPEED;
        if(kb_getKey(keyboard, KB_KEYCODE_S)->pressed)
            textOffset.y += MOVEMENT_SPEED;
        if(kb_getKey(keyboard, KB_KEYCODE_A)->pressed)
            textOffset.x -= MOVEMENT_SPEED;
        if(kb_getKey(keyboard, KB_KEYCODE_D)->pressed)
            textOffset.x += MOVEMENT_SPEED;

        if(kb_getKey(keyboard, KB_KEYCODE_ARROWUP)->pressed)
            font->lineHeight -= RESIZE_SPEED * (LINE_HEIGHT / CHAR_W);
        if(kb_getKey(keyboard, KB_KEYCODE_ARROWDOWN)->pressed)
            font->lineHeight += RESIZE_SPEED * (LINE_HEIGHT / CHAR_W);
        if(kb_getKey(keyboard, KB_KEYCODE_ARROWLEFT)->pressed)
            font->charW -= RESIZE_SPEED;
        if(kb_getKey(keyboard, KB_KEYCODE_ARROWRIGHT)->pressed)
            font->charW += RESIZE_SPEED;

        if(kb_getKey(keyboard, KB_KEYCODE_DIGIT1)->down)
            font->flags ^= FNT_FLAG_DISPLAY_GLYPH_RECTS;

        if(kb_getKey(keyboard, KB_KEYCODE_DIGIT2)->down)
            font->flags ^= FNT_FLAG_DISPLAY_CHAR_RECTS;

        if(kb_getKey(keyboard, KB_KEYCODE_DIGIT3)->down)
            font->flags ^= FNT_FLAG_DISPLAT_TEXT_RECTS;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        fnt_Vector2D textPos;
        fnt_setTextColor(font, 128, 128, time/25 % 255, 255);
        textPos = (fnt_Vector2D){ .x = TEXT1_X + textOffset.x, .y = TEXT1_Y + textOffset.y };
        fnt_drawText(font, renderer, &textPos, TEXT1);

        fnt_setTextColor(font, (int)((time + 100)*0.8) % 255, time % 100, time %255, time %10 + 200);
        textPos = (fnt_Vector2D){ .x = TEXT2_X + textOffset.x, .y = TEXT2_Y + textOffset.y };
        fnt_drawText(font, renderer, &textPos, TEXT2);

        fnt_setTextColor(font, 255, 255, 255, 128);
        textPos = (fnt_Vector2D){ .x = TEXT3_X + textOffset.x, .y = TEXT3_Y + textOffset.y };
        fnt_drawText(font, renderer, &textPos, TEXT3);

        SDL_RenderPresent(renderer);
        time++;
        SDL_Delay(16);
    }

    fnt_destroyFont(font);
    kb_destroyKeyboard(keyboard);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
