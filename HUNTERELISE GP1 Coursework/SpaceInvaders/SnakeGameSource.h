#pragma once

#include <memory>
#include "Game.h"
#include "Point.h"
#include "Food.h"
#include "Snake.h"

class SnakeGameSource : public Game {
public:
    SnakeGameSource(int w = 40, int h = 20, int fps = 10);
    ~SnakeGameSource();

    // Sets up the game
    void init() override;

    // Runs the game loop
    void run() override;

private:
    int width;
    int height;
    int targetFps;

    int score = 0;
    int lives = 3;

    enum class SnakeState { PLAY, GAME_OVER, EXIT };
    SnakeState sS = SnakeState::PLAY;

    // Sets the game state
    void setGameState(SnakeState s) { sS = s; }

    Snake snake;
    std::unique_ptr<Food> food = nullptr;
    std::unique_ptr<Point> extraLife = nullptr;

    // Creates new food on the board
    void spawnFood();

    // Creates an extra life pickup
    void spawnExtraLife();

    // Reads player input
    void handleInput();

    // Updates the game objects
    void update();

    // Draws the frame
    void renderFrame();

    // Flashes the snake on death
    void flashDeath();

    // Flashes the extra life pickup
    void flashExtraLife();
};
