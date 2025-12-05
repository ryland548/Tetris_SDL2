#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/label.hpp"

bool Label::load(SDL_Renderer* renderer) {
    font = TTF_OpenFont("./assets/font/tetris.ttf", fontSize);
    if (!font) {
        return false;
    }
    textSurface = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
    if (!textSurface) {
        return false;
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        return false;
    }
    width = labelText.length() * 24;
    return true;
}

void Label::render(SDL_Renderer* renderer) {
    spriteRect.x = x;
    spriteRect.y = y;
    spriteRect.w = width;
    spriteRect.h = height;
    SDL_RenderCopy(renderer, textTexture, NULL, &spriteRect);
}
