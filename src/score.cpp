#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../includes/score.hpp"

bool Score::load(SDL_Renderer* renderer) {
    font = TTF_OpenFont("./assets/font/tetris.ttf", fontSize);
    if (!font) {
        return false;
    }
    std::string text = std::to_string(score);
    textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (!textSurface) {
        return false;
    }
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        return false;
    }
    return true;
}

bool Score::render(SDL_Renderer* renderer, int changedScore) {
    if (changedScore != score) {
        score = changedScore;
        std::string text = std::to_string(score);
        textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
        if (!textSurface) {
            return false;
        }
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        if (!textTexture) {
            return false;
        }
    }
    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;

    if (score < 10) {
        textRect.w = 75 / 2;
    } else if (score >= 10 && score < 100) {
        textRect.w = 75;
    } else if (score >= 100) {
        textRect.w = 75 * 1.5;
    }
    textRect.h = height;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    return true;
}
