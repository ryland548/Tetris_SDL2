#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include "block.hpp"

class Grid {
    public:
        int gridWidth = 16;
        int gridHeight = 23;
        int nextBlockGridWidth = 8;
        int nextBlockGridHeight = 8;
        int cellGap = 2;
        int cellOffSetX = 30;
        int cellOffsetY = 23;
        int nextBlockCellOffsetX = 650;
        int nextBlockCellOffsetY = 96;
        bool pieceActive = false;
        float frameSpeed = 800;
        bool canMove = true;
        bool gameOver = false;
        Uint32 lastFallTime = 0;
        std::vector<int> colors = {};
        std::vector<std::vector<std::vector<int>>> pieces = {
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // i
            {{0, 1}, {1, 1}, {2, 1}, {2, 0}}, // j
            {{0, 0}, {1, 0}, {2, 0}, {2, 1}}, // l
            {{1, 0}, {1, 1}, {0, 1}, {0, 2}}, // s
            {{0, 0}, {0, 1}, {0, 2}, {1, 1}}, // t
            {{0, 0}, {0, 1}, {1, 1}, {1, 2}}, // z
            {{0, 0}, {1, 0}, {0, 1}, {1, 1}} // 0
        };
        std::vector<std::vector<std::vector<int>>> shuffablePieces = pieces;
        std::vector<std::vector<int>> activePieceGrid;
        std::vector<std::vector<int>> grid;
        std::vector<std::vector<Block>> blockGrid;
        std::vector<std::vector<int>> nextBlockGrid;
        std::vector<std::vector<Block>> blockNextBlockGrid;

    bool init(SDL_Renderer* renderer);
    void renderGrid(SDL_Renderer* renderer);
    bool moveDown(SDL_Renderer* renderer);
    bool spawnPiece(SDL_Renderer* renderer);
    void showPiece();
    bool moveLeft(SDL_Renderer* renderer);
    bool moveRight(SDL_Renderer* renderer);
    bool hardDrop(SDL_Renderer* renderer);
    bool showPreview(SDL_Renderer* renderer);
    void stopBlock();
    void resumeBlock();
    bool initNextBlockGrid(SDL_Renderer* renderer);
    void renderNextBlockGrid(SDL_Renderer* renderer);
    bool checkRow(SDL_Renderer* renderer, int &lineScore, int &score);
    bool rotateShape(SDL_Renderer* renderer);
    void stopGame();
};
