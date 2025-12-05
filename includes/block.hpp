#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

class Game;

class Block {
    public:
        float x = 0.f;
        float y = 0.f;
        int width = 35;
        int height = 35;
        int cellY = 0;
        int cellX = 0;
        int cellColorValue = 1;
        int cellPreviewColorValue = 1;
        std::string state = "i";
        SDL_Surface* spriteSolidSurface;
        SDL_Texture* spriteSolidTexture;
        SDL_Surface* spritePreviewSurface;
        SDL_Texture* spritePreviewTexture;
        SDL_Surface* spriteInvisibleSurface;
        SDL_Texture* spriteInvisibleTexture;
        SDL_Rect spriteRect;

    bool load(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    bool changeColor(SDL_Renderer* renderer, int newCellColor);
    bool changePreviewColor(SDL_Renderer* renderer, int newCellPreviewColor);
};
