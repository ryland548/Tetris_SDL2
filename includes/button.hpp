#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Button {
    public:
        int x = 395;
        int y = 450;
        int width = 195;
        int height = 60;
        std::string buttonLabel = " ";
        TTF_Font* font = nullptr;
        SDL_Surface* textSurface = nullptr;
        SDL_Texture* textTexture = nullptr;
        SDL_Color textColor = {255, 255, 255, 0};
        int fontSize = 40;
        int textW = 0;
        int textH = 0;

    bool load(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    bool isClicked(int mouseX, int mouseY);
};
