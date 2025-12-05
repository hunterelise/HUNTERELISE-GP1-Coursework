#pragma once
#include <memory>
#include <vector>

#include "Game.h"
#include "Alien.h"
#include "Barrier.h"
#include "Ground.h"
#include "Menu.h"
#include "Missile.h"
#include "Player.h"
#include "ScreenBuffer.h"
#include "Window.h"

#define PLAYER 28
#define BARRIER 22
#define SPEED 20

class GameSource : public Game {
public:
    GameSource();
    ~GameSource();

    // Sets up the game
    void init() override;

    // Runs the game loop
    void run() override;

    // Sets initial player position
    void setPlayerPosition();

    // Sets initial alien positions
    void setAlienPositions();

    // Sets initial barrier positions
    void setBarrierPositions();

    // Creates the screen buffers
    void createBuffers(int width, int height);

    // Handles player input
    void processInput();

    // Swaps front and back buffers
    void swapBuffers();

    // Updates all game objects
    void updateGame();

    // Sets positions for game elements
    void setGamePositions(int width, int height);

    // Draws the entire game
    void drawGame(int width, int height);

    // Checks if the missile hits anything
    void checkCollision(int width, int height) {
        if (!m_missle.getState())
            return;

        for (Barrier& element : m_barriers) {
            if (element.getState() &&
                element.getXPosition() == m_missle.getXPos() &&
                element.getYPosition() == m_missle.getYPos()) {
                element.setState(false);
                m_missle.setActive(false);
                return;
            }
        }

        for (Alien& alien : m_aliens) {
            if (alien.isActive() &&
                alien.getXPos() == m_missle.getXPos() &&
                alien.getYPos() == m_missle.getYPos()) {
                alien.setActive(false);
                m_missle.setActive(false);

                m_score += 10;
                if (m_score > m_highScore)
                    m_highScore = m_score;

                return;
            }
        }
    }

private:
    // Resets the level to its starting state
    void resetLevel();

    // Shows win or lose screen
    void showEndScreen(bool win);

    static const int MAX_LEVEL = 3;

    // Returns the speed value for this level
    int getLevelBaseSpeed() const;

    // Returns how many rows aliens drop at level change
    int getLevelDropRows() const;

    // Returns the row where aliens start
    int getLevelStartRow() const;

    Window m_gameWindow;
    Ground m_gameGround;

    std::unique_ptr<Menu> m_menu;
    std::unique_ptr<Player> m_player;
    Missile m_missle;

    Alien m_aliens[20];
    ScreenBuffer m_frontBuffer;
    ScreenBuffer m_backBuffer;
    ScreenBuffer m_resetBuffer;
    std::vector<Barrier> m_barriers;

    int m_lives = 3;
    int m_score = 0;
    int m_highScore = 0;
    int m_level = 1;

    int m_alienDir = 1;
    int m_tickCounter = 0;

    DWORD m_debugLastPress = 0;
    DWORD m_debugCooldown = 200;

    enum gameState { STARTSCREEN, LEVEL1, WIN, LOSE, EXIT };

    gameState gS;
};
