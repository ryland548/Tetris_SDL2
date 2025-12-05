#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../includes/block.hpp"
#include "../includes/game.hpp"

bool Block::load(SDL_Renderer* renderer) {
    std::string spritePath = "./assets/images/block" + std::to_string(cellColorValue) + ".png";
    spriteSolidSurface = IMG_Load(spritePath.c_str());
    if (spriteSolidSurface == nullptr) {
        return false;
    }
    spriteSolidTexture = SDL_CreateTextureFromSurface(renderer, spriteSolidSurface);
    if (spriteSolidTexture == nullptr) {
        return false;
    }
    SDL_FreeSurface(spriteSolidSurface);
    spriteSolidSurface = nullptr;

    std::string spritePreviewPath = "./assets/images/block8" + std::to_string(cellPreviewColorValue) + ".png";
    spritePreviewSurface = IMG_Load(spritePreviewPath.c_str());
    if (spritePreviewSurface == nullptr) {
        return false;
    }
    spritePreviewTexture = SDL_CreateTextureFromSurface(renderer, spritePreviewSurface);
    if (spritePreviewTexture == nullptr) {
        return false;
    }
    SDL_FreeSurface(spritePreviewSurface);
    spritePreviewSurface = nullptr;

    spriteInvisibleSurface = IMG_Load("./assets/images/block0.png");
    if (spriteInvisibleSurface == nullptr) {
        return false;
    }
    spriteInvisibleTexture = SDL_CreateTextureFromSurface(renderer, spriteInvisibleSurface);
    if (spriteInvisibleTexture == nullptr) {
        return false;
    }
    SDL_FreeSurface(spriteInvisibleSurface);
    spriteInvisibleSurface = nullptr;
    return true;
}

void Block::render(SDL_Renderer* renderer) {
    spriteRect = {
        static_cast<int>(x),
        static_cast<int>(y),
        width,
        height
    };
    if (state == "i") {
        SDL_RenderCopy(renderer, spriteInvisibleTexture, NULL, &spriteRect);
    } else if (state == "p") {
        SDL_RenderCopy(renderer, spritePreviewTexture, NULL, &spriteRect);
    } else {
        SDL_RenderCopy(renderer, spriteSolidTexture, NULL, &spriteRect);
    }
}

bool Block::changeColor(SDL_Renderer* renderer, int newCellColor) {
    std::string spritePath = "./assets/images/block" + std::to_string(newCellColor) + ".png";
    spriteSolidSurface = IMG_Load(spritePath.c_str());
    if (spriteSolidSurface == nullptr) {
        return false;
    }
    spriteSolidTexture = SDL_CreateTextureFromSurface(renderer, spriteSolidSurface);
    if (spriteSolidTexture == nullptr) {
        return false;
    }
    SDL_FreeSurface(spriteSolidSurface);
    spriteSolidSurface = nullptr;
    cellColorValue = newCellColor;
    return true;
}

bool Block::changePreviewColor(SDL_Renderer* renderer, int newCellPreviewColor) {
    std::string spritePreviewPath = "./assets/images/block8" + std::to_string(newCellPreviewColor) + ".png";
    spritePreviewSurface = IMG_Load(spritePreviewPath.c_str());
    if (spritePreviewSurface == nullptr) {
        return false;
    }
    spritePreviewTexture = SDL_CreateTextureFromSurface(renderer, spritePreviewSurface);
    if (spritePreviewTexture == nullptr) {
        return false;
    }
    SDL_FreeSurface(spritePreviewSurface);
    cellPreviewColorValue = newCellPreviewColor;
    return true;
}
