#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT 450

typedef struct
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

void show_position(SDL_Renderer *ren, int x, int y, Color *x_color, Color *y_color, TTF_Font *font,
                   SDL_Color font_color, bool show_text)
{
    Color default_color = {255, 0, 0, 255};

    if (x_color == NULL && y_color == NULL)
    {
        x_color = &default_color;
        y_color = &default_color;
    }

    if (x_color == NULL)
        x_color = y_color;
    else if (y_color == NULL)
        y_color = x_color;

    char *text = malloc(100 * sizeof(char));
    sprintf(text, "(%i,%i)", x, y);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, font_color);
    SDL_Texture *message = SDL_CreateTextureFromSurface(ren, surface);

    int message_rect_x = x + 5;
    int message_rect_y = y + 5;

    if (message_rect_x + surface->w > WIDTH)
    {
        message_rect_x = x - surface->w - 5;
    }

    if (message_rect_y + surface->h > HEIGHT)
    {
        message_rect_y = y - surface->h - 5;
    }

    SDL_Rect message_rect = {.x = message_rect_x, .y = message_rect_y, .w = surface->w, .h = surface->h};

    // Draw lines
    SDL_SetRenderDrawColor(ren, x_color->r, x_color->g, x_color->b, x_color->a);
    SDL_RenderDrawLine(ren, 0, y, x, y);
    SDL_RenderDrawLine(ren, x, y, WIDTH, y);

    SDL_SetRenderDrawColor(ren, y_color->r, y_color->g, y_color->b, y_color->a);
    SDL_RenderDrawLine(ren, x, 0, x, y);
    SDL_RenderDrawLine(ren, x, y, x, HEIGHT);

    if (show_text)
    {
        // Display text
        SDL_RenderCopy(ren, message, NULL, &message_rect);
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderDrawRect(ren, &message_rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(message);
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "ERROR: Could not init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Mouse position test", 0, 0, WIDTH, HEIGHT, 0);
    if (win == NULL)
        return -1;

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
        return -1;

    if (TTF_Init() == -1)
        return -1;

    TTF_Font *font = TTF_OpenFont("FiraCode-Regular.ttf", 13);
    SDL_Color font_color = {255, 255, 255};
    Color x_color = {250, 80, 80, 255};
    Color y_color = {250, 80, 80, 255};

    bool quit = false;

    while (!quit)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }

        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);

        int x, y;
        SDL_GetMouseState(&x, &y);

        show_position(ren, x, y, &x_color, &y_color, font, font_color, true);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();

    return 0;
}
