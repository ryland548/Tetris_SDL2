#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <SDL2/SDL.h>
#include "../includes/grid.hpp"
#include "../includes/block.hpp"
#include "../includes/game.hpp"

bool Grid::init(SDL_Renderer* renderer) {
    std::random_device randomDevice;
    std::mt19937 g(randomDevice());
    std::uniform_int_distribution<> distrib(1, 7);
    int cellColor = distrib(g);
    colors.push_back(cellColor);
    std::shuffle(shuffablePieces.begin(), shuffablePieces.end(), g);
    for (int column = 0; column < gridHeight; column += 1) {
        std::vector<Block> gridRow;
        for (int row = 0; row < gridWidth; row += 1) {
            Block block = Block();
            block.x = (row * (block.width + cellGap)) + cellOffSetX;
            block.y = (column * (block.height + cellGap)) + cellOffsetY;

            if (!block.load(renderer)) {
                return false;
            }
            gridRow.emplace_back(block);
        }
        blockGrid.emplace_back(gridRow);
    }
    for (int column = 0; column < gridHeight; column += 1) {
        std::vector<int> gridRow;
        for (int row = 0; row < gridWidth; row += 1) {
            gridRow.emplace_back(0);
        }
        grid.emplace_back(gridRow);
    }
    return true;
}

void Grid::renderGrid(SDL_Renderer* renderer) {
    for (int column = 0; column < gridHeight; column += 1) {
        for (int row = 0; row < gridWidth; row += 1) {
            if (grid[column][row] == 0) {
                blockGrid[column][row].state = "i";
            } else if (grid[column][row] == 1) {
                blockGrid[column][row].state = "s";
            } else {
                blockGrid[column][row].state = "p";
            }
        }
    }
    for (std::vector<Block> &blockRow : blockGrid) {
        for (Block &block : blockRow) {
            block.render(renderer);
        }
    }
}

bool Grid::moveDown(SDL_Renderer* renderer) {
    if (!canMove) {
        return true;
    }
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsed = currentTime - lastFallTime;

    bool canMoveDown = true;

    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        if (blockY + 1 >= gridHeight) {
            canMoveDown = false;
            break;
        }
        std::vector<int> newBlockPosition = {blockY + 1, blockX};
        int count = 0;
        count = std::count(activePieceGrid.begin(), activePieceGrid.end(), newBlockPosition);
        if (grid[blockY + 1][blockX] == 1 && count == 0) {
            canMoveDown = false;
            break;
        }
    }
    if (canMoveDown) {
        if (elapsed >= frameSpeed) {
            for (auto &blockPosition : activePieceGrid) {
                int blockY = blockPosition[0];
                int blockX = blockPosition[1];
                grid[blockY][blockX] = 0;
            }
            for (auto &blockPosition : activePieceGrid) {
                blockPosition[0] += 1;
            }
            for (auto &blockPosition : activePieceGrid) {
                int blockY = blockPosition[0];
                int blockX = blockPosition[1];
                grid[blockY][blockX] = 1;
                if (!blockGrid[blockY][blockX].changeColor(renderer, blockGrid[blockY - 1][blockX].cellColorValue)) {
                    return false;
                }
            }
            lastFallTime = currentTime;
        }
    } else {
        pieceActive = false;
    }
    return true;
}

void Grid::showPiece() {
    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        grid[blockY][blockX] = 1;
    }
}

bool Grid::spawnPiece(SDL_Renderer* renderer) {
    if (!pieceActive && !gameOver) {
        pieceActive = true;
        std::random_device randomDevice;
        std::mt19937 g(randomDevice());
        if (shuffablePieces.empty()) {
            shuffablePieces = pieces;
            std::shuffle(shuffablePieces.begin(), shuffablePieces.end(), g);
        }
        activePieceGrid = shuffablePieces.back();
        if (colors.empty()) {
            for (int i = 0; i < 5; i += 1) {
                std::uniform_int_distribution<> distrib(1, 7);
                colors.push_back(distrib(g));
            }
        }
        int cellColor = colors.back();
        colors.erase(std::remove(colors.begin(), colors.end(), cellColor), colors.end());
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            if (grid[blockY][blockX] != 1) {
                if (!blockGrid[blockY][blockX].changeColor(renderer, cellColor)) {
                    return false;
                }
            } else {
                if (grid[blockY][blockX] == 1) {
                    gameOver = true;
                }
            }
        }
        shuffablePieces.erase(std::remove(shuffablePieces.begin(), shuffablePieces.end(), activePieceGrid), shuffablePieces.end());
    }
    return true;
}

bool Grid::moveLeft(SDL_Renderer* renderer) {
    if (!canMove) {
        return true;
    }
    for (auto &collumn : grid) {
        for (auto &row : grid) {
            for (int &block: row) {
                if (block == 2) {
                    block = 0;
                }
            }
        }
    }
    bool canMoveLeft = true;
    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        if (blockX - 1 < 0) {
            canMoveLeft = false;
            break;
        }
        std::vector<int> newBlockPosition = {blockY, blockX - 1};
        int count = 0;
        count = std::count(activePieceGrid.begin(), activePieceGrid.end(), newBlockPosition);
        if (grid[blockY][blockX - 1] == 1 && count == 0) {
            canMoveLeft = false;
            break;
        }
    }
    if (canMoveLeft) {
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            grid[blockY][blockX] = 0;
        }
        for (auto &blockPosition : activePieceGrid) {
            blockPosition[1] -= 1;
        }
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            grid[blockY][blockX] = 1;
            if (!blockGrid[blockY][blockX].changeColor(renderer, blockGrid[blockY][blockX + 1].cellColorValue)) {
                return false;
            }
        }
    }
    return true;
}

bool Grid::moveRight(SDL_Renderer* renderer) {
    if (!canMove) {
        return true;
    }
    for (auto &collumn : grid) {
        for (auto &row : grid) {
            for (int &block: row) {
                if (block == 2) {
                    block = 0;
                }
            }
        }
    }
    bool canMoveRight = true;
    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        if (blockX + 1 >= gridWidth) {
            canMoveRight = false;
            break;
        }
        std::vector<int> newBlockPosition = {blockY, blockX + 1};
        int count = 0;
        count = std::count(activePieceGrid.begin(), activePieceGrid.end(), newBlockPosition);
        if (grid[blockY][blockX + 1] == 1 && count == 0) {
            canMoveRight = false;
            break;
        }
    }
    if (canMoveRight) {
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            grid[blockY][blockX] = 0;
        }
        for (auto &blockPosition : activePieceGrid) {
             blockPosition[1] += 1;
        }
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            grid[blockY][blockX] = 1;
            if (!blockGrid[blockY][blockX].changeColor(renderer, blockGrid[blockY][blockX - 1].cellColorValue)) {
                return false;
            }
        }
    }
    return true;
}

bool Grid::hardDrop(SDL_Renderer* renderer) {
    if (!canMove) {
        return true;
    }
    for (auto &collumn : grid) {
        for (auto &row : grid) {
            for (int &block: row) {
                if (block == 2) {
                    block = 0;
                }
            }
        }
    }
    bool canMoveDown = true;
    int fallCount = 0;
    while (canMoveDown) {
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            if (blockY + (1 + fallCount) >= gridHeight) {
                canMoveDown = false;
                break;
            }
            std::vector<int> newBlockPosition = {blockY + (1 + fallCount), blockX};
            int count = 0;
            count = std::count(activePieceGrid.begin(), activePieceGrid.end(), newBlockPosition);
            if (grid[blockY + (1 + fallCount)][blockX] == 1 && count == 0) {
                canMoveDown = false;
                break;
            }
        }
        if (canMoveDown) {
            fallCount += 1;
        }
    }
    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        grid[blockY][blockX] = 0;
    }
    for (auto &blockPosition : activePieceGrid) {
        blockPosition[0] += fallCount;
    }
    for (auto &blockPosition : activePieceGrid) {
        int blockY = blockPosition[0];
        int blockX = blockPosition[1];
        grid[blockY][blockX] = 1;
        if (!blockGrid[blockY][blockX].changeColor(renderer, blockGrid[blockY - fallCount][blockX].cellColorValue)) {
            return false;
        }
    }
    return true;
}

bool Grid::showPreview(SDL_Renderer* renderer) {
    bool canMoveDown = true;
    int fallCount = 0;
    while (canMoveDown) {
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            if (blockY + (1 + fallCount) >= gridHeight) {
                canMoveDown = false;
                break;
            }
            std::vector<int> newBlockPosition = {blockY + (1 + fallCount), blockX};
            int count = 0;
            count = std::count(activePieceGrid.begin(), activePieceGrid.end(), newBlockPosition);
            if (grid[blockY + (1 + fallCount)][blockX] == 1 && count == 0) {
                canMoveDown = false;
                break;
            }
        }
        if (canMoveDown) {
            fallCount += 1;
        }
    }
    if (fallCount > 4) {
        for (auto &blockPosition : activePieceGrid) {
            int blockY = blockPosition[0];
            int blockX = blockPosition[1];
            grid[blockY + fallCount][blockX] = 2;
            int blockColor = blockGrid[blockY][blockX].cellColorValue;
            if (!blockGrid[blockY + fallCount][blockX].changePreviewColor(renderer, blockColor)) {
                return false;
            }
        }
    } else {
        for (auto &row : grid) {
            for (int &block: row) {
                if (block == 2) {
                    block = 0;
                }
            }
        }
    }
    return true;
}

void Grid::stopBlock() {
    canMove = false;
}

void Grid::resumeBlock() {
    canMove = true;
}

bool Grid::initNextBlockGrid(SDL_Renderer* renderer) {
    for (int column = 0; column < nextBlockGridHeight; column += 1) {
        std::vector<Block> gridRow;
        for (int row = 0; row < nextBlockGridWidth; row += 1) {
            Block block = Block();
            block.x = (row * (block.width + cellGap)) + nextBlockCellOffsetX;
            block.y = (column * (block.height + cellGap)) + nextBlockCellOffsetY;

            if (!block.load(renderer)) {
                return false;
            }
            gridRow.emplace_back(block);
        }
        blockNextBlockGrid.emplace_back(gridRow);
    }
    for (int column = 0; column < nextBlockGridHeight; column += 1) {
        std::vector<int> gridRow;
        for (int row = 0; row < nextBlockGridWidth; row += 1) {
            gridRow.emplace_back(0);
        }
        nextBlockGrid.emplace_back(gridRow);
    }
    return true;
}

void Grid::renderNextBlockGrid(SDL_Renderer* renderer) {
    for (auto &row : nextBlockGrid) {
        for (int &block: row) {
            if (block == 1) {
                block = 0;
            }
        }
    }
    if (!shuffablePieces.empty()) {
        for (auto &block : shuffablePieces.back()) {
            int blockY = block[0] + 2;
            int blockX = block[1] + 2;
            if (blockY < nextBlockGridHeight && blockX < nextBlockGridWidth) {
                nextBlockGrid[blockY][blockX] = 1;
                if (!colors.empty() && blockNextBlockGrid[blockY][blockX].cellColorValue != colors.back()) {
                    blockNextBlockGrid[blockY][blockX].changeColor(renderer, colors.back());
                } else {
                    if (colors.empty()) {
                        for (int i = 0; i < 5; i += 1) {
                            std::random_device randomDevice;
                            std::mt19937 g(randomDevice());
                            std::uniform_int_distribution<> distrib(1, 7);
                            colors.push_back(distrib(g));
                        }
                    }
                }
            }
        }
    } else {
        std::random_device randomDevice;
        std::mt19937 g(randomDevice());
        shuffablePieces = pieces;
        std::shuffle(shuffablePieces.begin(), shuffablePieces.end(), g);
    }
    for (int column = 0; column < nextBlockGridHeight; column += 1) {
        for (int row = 0; row < nextBlockGridWidth; row += 1) {
            if (nextBlockGrid[column][row] == 0) {
                blockNextBlockGrid[column][row].state = "i";
            } else if (nextBlockGrid[column][row] == 1) {
                blockNextBlockGrid[column][row].state = "s";
            }
        }
    }
    for (std::vector<Block> &blockRow : blockNextBlockGrid) {
        for (Block &block : blockRow) {
            block.render(renderer);
        }
    }
}

bool Grid::checkRow(SDL_Renderer* renderer, int &lineScore, int &score) {
    for (int i = 0; i < gridHeight; i += 1) {
        bool rowComplete = true;
        for (int j = 0; j < gridWidth; j += 1) {
            if (grid[i][j] == 0 || grid[i][j] == 2) {
                rowComplete = false;
                break;
            }
        }
        if (rowComplete) {
            for (int j = 0; j < gridWidth; j += 1) {
                grid[i][j] = 0;
            }
            for (int row = i - 1; row >= 0; row -= 1) {
                for (int col = 0; col < gridWidth; col += 1) {
                    grid[row + 1][col] = grid[row][col];
                    blockGrid[row + 1][col].changeColor(renderer, blockGrid[row][col].cellColorValue);
                }
            }
            for (int i = 0; i < gridWidth; i += 1) {
                grid[0][i] = 0;
                blockGrid[0][i].changeColor(renderer, 1);
            }
            for (auto &row : grid) {
                for (int &block : row) {
                    if (block == 2) {
                        block = 0;
                    }
                }
            }
            lineScore += 1;
            score += (gridHeight - i) * 10;
        }
    }
    return true;
}

bool Grid::rotateShape(SDL_Renderer* renderer) {
    if (pieceActive) {
        for (auto &row : grid) {
            for (int &block: row) {
                if (block == 2) {
                    block = 0;
                }
            }
        }
        int py = activePieceGrid[1][0];
        int px = activePieceGrid[1][1];
        std::vector<std::vector<int>> rotatedPiece;
        for (auto &pos : activePieceGrid) {
            int posY = pos[0];
            int posX = pos[1];

            int newRow = py + (posX - px);
            int newCol = px - (posY - py);

            rotatedPiece.push_back({newRow, newCol});
        }
        bool canRotate = true;
        for (auto &pos : activePieceGrid) {
            int posY = pos[0];
            int posX = pos[1];
            grid[posY][posX] = 0;
        }
        for (auto &pos : rotatedPiece) {
            int posY = pos[0];
            int posX = pos[1];
            if (posX < 0 || posX >= gridWidth) {
                canRotate = false;
                for (auto &pos : activePieceGrid) {
                    int posY = pos[0];
                    int posX = pos[1];
                    grid[posY][posX] = 1;
                }
            } else if (posY < 0 || posY >= gridHeight) {
                canRotate = false;
                for (auto &pos : activePieceGrid) {
                    int posY = pos[0];
                    int posX = pos[1];
                    grid[posY][posX] = 1;
                }
            }
        }
        for (auto &pos : rotatedPiece) {
            int posY = pos[0];
            int posX = pos[1];
            if (grid[posY][posX] == 1) {
                canRotate = false;
                for (auto &pos : activePieceGrid) {
                    int posY = pos[0];
                    int posX = pos[1];
                    grid[posY][posX] = 1;
                }
            }
        }
        if (canRotate) {
            int color = 1;
            for (auto &pos : activePieceGrid) {
                grid[pos[0]][pos[1]] = 0;
                color = blockGrid[pos[0]][pos[1]].cellColorValue;
            }
            activePieceGrid = rotatedPiece;
            for (auto &pos : activePieceGrid) {
                grid[pos[0]][pos[1]] = 1;
                if (!blockGrid[pos[0]][pos[1]].changeColor(renderer, color)) {
                    return false;
                }
            }
        }
    }
    return true;
}
