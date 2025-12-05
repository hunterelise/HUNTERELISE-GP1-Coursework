#pragma once

class Game {
public:
    virtual ~Game() {}

    // Sets up the game
    virtual void init() = 0;

    // Starts the game loop
    virtual void run() = 0;
};
