#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define FNT_ASCII_FIRST_VISIBLE_CHAR        (((int)' ') + 1)
#define FNT_ASCII_LAST_VISIBLE_CHAR         127
#define FNT_ASCII_TOTAL_VISIBLE_CHARS       (FNT_ASCII_LAST_VISIBLE_CHAR - FNT_ASCII_FIRST_VISIBLE_CHAR + 1)

#define FNT_DEFAULT_TAB_WIDTH       4
#define FNT_DEFAULT_TEXT_COLOR      (SDL_Color){ 0, 0, 0, 255 }
#define FNT_TEXT_BUFFER_SIZE        1024

typedef enum {
    FNT_CHARSET_ASCII,
    FNT_CHARSET_UTF8,
} fnt_Charset;

typedef struct {
    SDL_Rect srcRect;
    float offsetX, offsetY;
    float scaleX, scaleY;
} fnt_GlyphData;

typedef struct {
    /* Used internally, not modifiable */
    SDL_Texture *texture;

    fnt_GlyphData *glyphs;
    struct {
        unsigned int first;
        unsigned int last;
        unsigned int total;
    } visibleChars;
    fnt_Charset charset;

    /* User-modifiable at runtime */
    float lineHeight;
    float charW;
    unsigned int tabWidth;
} fnt_Font;

/* I suppose you can't make this kind of API much simpler */
fnt_Font *fnt_initFont(const char *filePath, SDL_Renderer *renderer, float charW, float charH, fnt_Charset charset);

void fnt_drawText(fnt_Font *fnt, SDL_Renderer *renderer, float baselineStartX, float baselineStartY, const char *fmt, ...);

void fnt_destroyFont(fnt_Font *fnt);

/* Sets the font's texture color- and blend mode */
void fnt_setTextColor(fnt_Font *fnt, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#endif
