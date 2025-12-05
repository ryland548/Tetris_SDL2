#pragma once
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "grid.hpp"
#include "label.hpp"
#include "score.hpp"
#include "button.hpp"

class Game {
    public:
        int screenWidth = 970;
        int screenHeight = 900;
        int delayTiming = 16;
        bool running = true;
        int score = 0;
        int lineTextCount = 0;
        bool start = false;
        bool calculatedScore = false;
        int mouseX, mouseY;
        std::string windowTitle = "Tetris";
        SDL_Event event;
        SDL_Color bgColor = {0, 0, 0, 0};
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Mix_Music* bgMusic = nullptr;
        Grid grid;
        Label nextBlockLabel;
        Label statsLabel;
        Label scoreLabel;
        Label lineLabel;
        Label gameOverScoreLabel;
        Label gameOverLineLabel;
        Score scoreText;
        Score lineText;
        Button startButton;
        Button restartButton;

    void error();
    void mixError();
    void ttfError();
    void load();
    void run();
    void input();
    void render();
    void gameOverScreen();
    void startScreen();
    void quit();
    void restart();
};
