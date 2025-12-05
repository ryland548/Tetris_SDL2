#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Label {
    public:
        double x = 628.0;
        double y = 15.0;
        std::string labelText = " ";
        int width = 0;
        int height = 70;
        int fontSize = 96;
        TTF_Font* font = NULL;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Rect spriteRect;
        SDL_Color textColor = {255, 255, 255, 0};

    bool load(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
};
