#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Score {
    public:
        double x = 695.0;
        double y = 580.0;
        int width = 75;
        int height = 75;
        int fontSize = 96;
        int score = 0;
        TTF_Font* font = NULL;
        SDL_Surface* textSurface;
        SDL_Texture* textTexture;
        SDL_Color textColor = {140, 169, 255, 0};
    bool load(SDL_Renderer* renderer);
    bool render(SDL_Renderer* renderer, int changedScore);
};
