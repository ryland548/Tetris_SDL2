#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../includes/game.hpp"
#include "../includes/grid.hpp"
#include "../includes/block.hpp"
#include "../includes/label.hpp"
#include "../includes/score.hpp"
#include "../includes/button.hpp"

void Game::error() {
    std::cerr << "SDL Error: " << SDL_GetError() << '\n';
}

void Game::mixError() {
    std::cerr << "SDL Error: " << Mix_GetError() << '\n';
}

void Game::ttfError() {
    std::cerr << "SDL Error: " << TTF_GetError() << '\n';
}

void Game::quit() {
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
    }
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void Game::load() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error();
        quit();
    }
    window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        error();
        quit();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        error();
        quit();
    }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        mixError();
        quit();
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        mixError();
        quit();
    }
    if (TTF_Init() != 0) {
        ttfError();
        quit();
    }
    grid = Grid();
    if (!grid.init(renderer)) {
        error();
        quit();
    }
    if (!grid.initNextBlockGrid(renderer)) {
        error();
        quit();
    }
    nextBlockLabel = Label();
    nextBlockLabel.labelText = "  NEXT BLOCK  ";
    if (!nextBlockLabel.load(renderer)) {
        error();
        quit();
    }
    statsLabel = Label();
    statsLabel.labelText = "  GAME STATS  ";
    statsLabel.y = 408;
    if (!statsLabel.load(renderer)) {
        error();
        quit();
    }
    scoreLabel = Label();
    scoreLabel.labelText = "     SCORE  ";
    scoreLabel.y = 508;
    if (!scoreLabel.load(renderer)) {
        error();
        quit();
    }
    lineLabel = Label();
    lineLabel.labelText = "     LINE  ";
    lineLabel.y = 668;
    if (!lineLabel.load(renderer)) {
        error();
        quit();
    }
    gameOverScoreLabel = Label();
    gameOverScoreLabel.y = 620;
    gameOverScoreLabel.x = 290;
    gameOverScoreLabel.labelText = "Score obtained: " + std::to_string(score);

    gameOverLineLabel = Label();
    gameOverLineLabel.y = 730;
    gameOverLineLabel.x = 290;
    gameOverLineLabel.labelText = "Lines cleared: " + std::to_string(lineTextCount);
    if (!gameOverScoreLabel.load(renderer)) {
        error();
        quit();
    }
    scoreText = Score();
    if (!scoreText.load(renderer)) {
        error();
        quit();
    }
    lineText = Score();
    lineText.y += 160;
    if (!lineText.load(renderer)) {
        error();
        quit();
    }
    bgMusic = Mix_LoadMUS("./assets/audio/tetrisTheme.mp3");
    if (bgMusic == nullptr) {
        mixError();
        quit();
    }
    Mix_VolumeMusic(64);
    Mix_PlayMusic(bgMusic, -1);
    startButton = Button();
    startButton.buttonLabel = "Start";
    if (!startButton.load(renderer)) {
        error();
        quit();
    }
    restartButton = Button();
    restartButton.buttonLabel = "Restart";
    if (!restartButton.load(renderer)) {
        error();
        quit();
    }

}

void Game::render() {
    if (!grid.spawnPiece(renderer)) {
        error();
        quit();
    }
    grid.renderGrid(renderer);
    grid.showPiece();
    grid.renderNextBlockGrid(renderer);
    if (!grid.moveDown(renderer)) {
        error();
        quit();
    }
    if (!grid.showPreview(renderer)) {
        error();
        quit();
    }
    if (!grid.checkRow(renderer, lineTextCount, score)) {
        error();
        quit();
    }
    nextBlockLabel.render(renderer);
    statsLabel.render(renderer);
    scoreLabel.render(renderer);
    scoreText.render(renderer, score);
    lineLabel.render(renderer);
    lineText.render(renderer, lineTextCount);
}

void Game::input() {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_a) {
            if (!grid.moveLeft(renderer)) {
                error();
                quit();
            }
        }
        if (event.key.keysym.sym == SDLK_d) {
            if (!grid.moveRight(renderer)) {
                error();
                quit();
            }
        }
        if (event.key.keysym.sym == SDLK_SPACE) {
            if (!grid.hardDrop(renderer)) {
                error();
                quit();
            }
        }
        if (event.key.keysym.sym == SDLK_LSHIFT) {
            grid.stopBlock();
        }
        if (event.key.keysym.sym == SDLK_r) {
            if (!grid.rotateShape(renderer)) {
                error();
                quit();
            }
        }
    }
    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LSHIFT) {
            grid.resumeBlock();
        }
    }
}

void Game::run() {
    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (start && !grid.gameOver) {
                input();
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && !start) {
                if (startButton.isClicked(mouseX, mouseY)) {
                    start = true;
                    grid.gameOver = false;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && start && grid.gameOver) {
                if (restartButton.isClicked(mouseX, mouseY)) {
                    restart();
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderClear(renderer);
        SDL_GetMouseState(&mouseX, &mouseY);
        if (start) {
            if (!grid.gameOver) {
                render();
            } else {
                gameOverScreen();
            }
        } else {
            startScreen();
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    quit();
}

void Game::gameOverScreen() {
    if (!calculatedScore) {
        gameOverScoreLabel.labelText = "Score obtained: " + std::to_string(score);
        gameOverScoreLabel.load(renderer);
        gameOverLineLabel.labelText = "Lines cleared: " + std::to_string(lineTextCount);
        gameOverLineLabel.load(renderer);
    }
    gameOverScoreLabel.render(renderer);
    gameOverLineLabel.render(renderer);
    restartButton.render(renderer);
}

void Game::startScreen() {
    startButton.render(renderer);
}

void Game::restart() {
    for (auto &row : grid.grid) {
        for (int &block : row) {
            block = 0;
        }
    }
    for (auto &row : grid.nextBlockGrid) {
        for (int &block : row) {
            block = 0;
        }
    }
    grid.colors.clear();

    grid.pieceActive = false;
    for (auto &blockPos : grid.activePieceGrid) {
        blockPos[0] = 0;
        blockPos[1] = 1;
    }
    score = 0;
    lineTextCount = 0;
    grid.gameOver = false;
    start = false;

    score = 0;
    lineTextCount = 0;
    scoreText.score = 0;
    lineText.score = 0;
    calculatedScore = false;
    if (!scoreText.load(renderer)) {
        error();
        quit();
    }
    if (!lineText.load(renderer)) {
        error();
        quit();
    }
}
