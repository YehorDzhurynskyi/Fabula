#include "pch.h"
#include "application.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

int main(int argc, char* argv[])
{
#if 0
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("Assets/Staatliches-Regular.ttf", 180);

    SDL_Color fg;
    fg.r = 0x0;
    fg.g = 0x0;
    fg.b = 0x0;
    fg.a = 0xff;

    SDL_Color bg;
    bg.r = 0x0;
    bg.g = 0x0;
    bg.b = 0x0;
    bg.a = 0x0;


    SDL_Surface* surface = TTF_RenderText_Blended(font, "0 1 2 3 4 5 6 7 8 9 m b e s t :", fg);
#if 0
    SDL_Surface* surface = TTF_RenderText(font,
                                          "0 1 2 3 4 5 6 7 8 9 m",
                                          fg,
                                          bg);
#endif

    IMG_SavePNG(surface, "fontatlas.png");

    TTF_CloseFont(font);
#endif

    run();
    return 0;
}
