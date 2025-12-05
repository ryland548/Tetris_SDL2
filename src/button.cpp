#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/button.hpp"

bool Button::load(SDL_Renderer* renderer) {
    font = TTF_OpenFont("./assets/font/tetris.ttf", fontSize);
    if (!font) {
        return false;
    }
    textSurface = TTF_RenderText_Solid(font, buttonLabel.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return true;
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Rect rect = {
            x,
            y,
            width,
            height
    };
    SDL_SetRenderDrawColor(renderer, 245, 147, 66, 0);
    SDL_RenderFillRect(renderer, &rect);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    SDL_Rect textRect = {
        x + (width - textW) / 2,
        y + (height - textH) / 2,
        textW,
        textH
    };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

bool Button::isClicked(int mouseX, int mouseY) {
    return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
}
