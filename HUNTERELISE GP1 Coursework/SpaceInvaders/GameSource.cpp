#include "GameSource.h"
#include "SnakeGameSource.h"
#include "ConsoleUtils.h"

#include <iostream>
#include <Windows.h>

extern int menuChoice;

GameSource::GameSource()
    : m_gameGround()
    , m_menu(std::make_unique<Menu>())
    , m_player(std::make_unique<Player>())
    , m_barriers(20)
{
}

GameSource::~GameSource()
{
    // unique_ptr cleanup happens automatically
}

// Sets the starting position of the player
void GameSource::setPlayerPosition()
{
    m_player->setXPos(15);
    m_player->setYPos(PLAYER);
}

// Sets starting positions for all aliens
void GameSource::setAlienPositions()
{
    int startRow = getLevelStartRow();

    for (int i = 0; i < 20; i++)
    {
        m_aliens[i].setPosition(i * 3, startRow);
        m_aliens[i].setActive(true);
    }

    Alien::setSpeed(getLevelBaseSpeed());
}

// Sets up barrier positions and activates them
void GameSource::setBarrierPositions()
{
    for (int i = 0; i < 5; i++)
        m_barriers[i].setPosition(i + 10, BARRIER);

    for (int i = 5; i < 10; i++)
        m_barriers[i].setPosition(i + 25, BARRIER);

    for (int i = 10; i < 15; i++)
        m_barriers[i].setPosition(i + 40, BARRIER);

    for (int i = 15; i < 20; i++)
        m_barriers[i].setPosition(i + 55, BARRIER);

    for (auto& b : m_barriers)
        b.setState(true);
}

// Creates and clears the screen buffers
void GameSource::createBuffers(int width, int height)
{
    m_frontBuffer = ScreenBuffer(width, height);
    m_backBuffer = ScreenBuffer(width, height);
    m_resetBuffer = ScreenBuffer(width, height);

    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
            m_resetBuffer.setChar(x, y, ' ');

    m_frontBuffer = m_resetBuffer;
    m_backBuffer = m_resetBuffer;
}

// Swaps front and back buffer
void GameSource::swapBuffers()
{
    std::swap(m_frontBuffer, m_backBuffer);
}

// Sets up window, ground, objects, and starting values
void GameSource::init()
{
    m_gameWindow.setWindow(80, 30);
    m_gameGround.setYPos(m_gameWindow.getHeight() - 1);

    setPlayerPosition();
    setAlienPositions();
    setBarrierPositions();
    createBuffers(80, 30);

    m_lives = 3;
    m_score = 0;
    m_level = 1;

    gS = STARTSCREEN;
}

// Handles keyboard input and debug keys
void GameSource::processInput()
{
    // Q returns to the start screen
    if (GetKeyState('Q') & 0x8000)
    {
        m_missle.setActive(false);
        gS = STARTSCREEN;
        return;
    }

    m_player->update();
    m_missle.fireMissle(*m_player);

    DWORD now = GetTickCount();

    // Debug keys with cooldown
    if (now - m_debugLastPress > m_debugCooldown)
    {
        // K kills all aliens
        if (GetKeyState('K') & 0x8000)
        {
            for (auto& a : m_aliens)
                a.setActive(false);

            m_debugLastPress = now;
        }

        // L removes a life and resets or ends game
        if (GetKeyState('L') & 0x8000)
        {
            m_lives--;
            if (m_lives <= 0)
                gS = LOSE;
            else
                resetLevel();

            m_debugLastPress = now;
        }
    }
}

// Returns base alien speed for current level
int GameSource::getLevelBaseSpeed() const
{
    switch (m_level)
    {
    case 1:  return 1;
    case 2:  return 2;
    case 3:  return 3;
    default: return 1;
    }
}

// Returns how many rows aliens drop when changing direction
int GameSource::getLevelDropRows() const
{
    switch (m_level)
    {
    case 1:  return 1;
    case 2:  return 2;
    case 3:  return 2;
    default: return 1;
    }
}

// Returns starting row for aliens for this level
int GameSource::getLevelStartRow() const
{
    switch (m_level)
    {
    case 1:  return 1;
    case 2:  return 2;
    case 3:  return 3;
    default: return 1;
    }
}

// Resets aliens, barriers, missile and movement values
void GameSource::resetLevel()
{
    setAlienPositions();
    setBarrierPositions();
    m_missle.setActive(false);
    m_alienDir = 1;
    m_tickCounter = 0;
    Alien::setSpeed(getLevelBaseSpeed());
}

// Updates alien movement and checks for game over
void GameSource::updateGame()
{
    // Count how many aliens are still alive
    int aliveCount = 0;
    for (const auto& a : m_aliens)
        if (a.isActive()) aliveCount++;

    // Level complete when all aliens are gone
    if (aliveCount == 0)
    {
        if (m_level < MAX_LEVEL)
        {
            m_level++;
            resetLevel();
        }
        else
        {
            gS = WIN;
        }
        return;
    }

    // Speed up as fewer aliens remain
    int baseSpeed = getLevelBaseSpeed();
    int bonus = (20 - aliveCount) / 5;
    int speed = baseSpeed + bonus;

    if (speed > 6)
        speed = 6;

    Alien::setSpeed(speed);

    m_tickCounter++;
    int moveEvery = 8 - speed;
    if (moveEvery < 1) moveEvery = 1;

    // Move aliens at set intervals
    if (m_tickCounter >= moveEvery)
    {
        m_tickCounter = 0;

        int minX = 1000;
        int maxX = -1000;
        bool anyAlive = false;

        // Find leftmost and rightmost aliens
        for (const auto& a : m_aliens)
        {
            if (!a.isActive()) continue;
            anyAlive = true;

            int ax = a.getXPos();
            if (ax < minX) minX = ax;
            if (ax > maxX) maxX = ax;
        }

        if (anyAlive)
        {
            bool hitEdge = false;

            // Detect if aliens reach screen edge
            if (m_alienDir > 0 && maxX + speed >= m_gameWindow.getWidth() - 1)
                hitEdge = true;
            else if (m_alienDir < 0 && minX - speed <= 0)
                hitEdge = true;

            // Drop down and change direction at edge
            if (hitEdge)
            {
                int drop = getLevelDropRows();

                for (auto& a : m_aliens)
                    if (a.isActive())
                        a.setPosition(a.getXPos(), a.getYPos() + drop);

                m_alienDir *= -1;
            }
            else
            {
                // Move aliens horizontally
                for (auto& a : m_aliens)
                {
                    if (!a.isActive()) continue;
                    a.setPosition(a.getXPos() + m_alienDir * speed, a.getYPos());
                }
            }
        }
    }

    // Check if any alien reaches the player row
    for (auto& a : m_aliens)
    {
        if (!a.isActive()) continue;

        if (a.getYPos() >= PLAYER)
        {
            m_lives--;
            if (m_lives <= 0)
                gS = LOSE;
            else
                resetLevel();
            break;
        }
    }
}

// Fills the back buffer with current game objects
void GameSource::setGamePositions(int width, int height)
{
    m_backBuffer = m_resetBuffer;

    // Aliens
    for (const auto& a : m_aliens)
    {
        if (!a.isActive()) continue;
        int ax = a.getXPos();
        int ay = a.getYPos();
        if (ax >= 0 && ax < width && ay >= 0 && ay < height)
            m_backBuffer.setChar(ax, ay, 'X');
    }

    // Barriers
    for (auto& b : m_barriers)
    {
        if (!b.getState()) continue;
        int bx = b.getXPosition();
        int by = b.getYPosition();
        if (bx >= 0 && bx < width && by >= 0 && by < height)
            m_backBuffer.setChar(bx, by, '=');
    }

    // Player
    int px = m_player->getXPos();
    int py = m_player->getYPos();
    if (px >= 0 && px < width && py >= 0 && py < height)
        m_backBuffer.setChar(px, py, '^');

    // Ground line
    int gy = m_gameGround.getYPos();
    if (gy >= 0 && gy < height)
    {
        for (int x = 0; x < width; ++x)
            m_backBuffer.setChar(x, gy, '-');
    }

    // Missile
    if (m_missle.getState())
    {
        int mx = m_missle.getXPos();
        int my = m_missle.getYPos();

        if (mx >= 0 && mx < width && my >= 0 && my < height)
            m_backBuffer.setChar(mx, my, '!');

        // Move missile after drawing it
        m_missle.update();
    }
}

// Draws the buffer to the screen and prints HUD
void GameSource::drawGame(int width, int height)
{
    for (int y = 1; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            setCursor(x, y);
            std::cout << m_frontBuffer.getChar(x, y);
        }
    }

    // Draw score and other info on the top line
    setCursor(0, 0);
    std::cout << "Score: " << m_score
        << "  Lives: " << m_lives
        << "  HighScore: " << m_highScore
        << "  Level: " << m_level << "/" << MAX_LEVEL << "   ";
}

// Shows win or lose animation and waits for Q
void GameSource::showEndScreen(bool win)
{
    system("cls");

    const char* text = win ? "YOU WIN!" : "YOU LOSE!";

    WORD bright = win
        ? (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
        : (FOREGROUND_RED | FOREGROUND_INTENSITY);

    WORD dim = win
        ? (FOREGROUND_GREEN)
        : (FOREGROUND_RED);

    int x = 30;
    int y = 10;

    // Flash the message a few times
    for (int i = 0; i < 20; ++i)
    {
        WORD color = (i % 2 == 0) ? bright : dim;
        setColor(color);

        setCursor(x, y);
        std::cout << text;

        Sleep(150);
    }

    // Reset color to white
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    setCursor(x, y + 2);
    std::cout << "Press Q to return to menu...";

    // Wait until Q is pressed
    while (true)
    {
        if (GetKeyState('Q') & 0x8000)
            break;
        Sleep(50);
    }

    // Clear any leftover key events
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hIn);
}

// Main game loop and state handling
void GameSource::run()
{
    while (gS != EXIT)
    {
        switch (gS)
        {
        case STARTSCREEN:
            m_menu->run();

            if (menuChoice == 1)
            {
                m_lives = 3;
                m_score = 0;
                m_level = 1;
                resetLevel();
                gS = LEVEL1;
            }
            else if (menuChoice == 2)
            {
                SnakeGameSource snake;
                snake.init();
                snake.run();
                gS = STARTSCREEN;
            }
            else
            {
                gS = EXIT;
            }
            break;

        case LEVEL1:
            processInput();

            if (gS != LEVEL1)
                break;

            updateGame();

            if (gS != LEVEL1)
                break;

            setGamePositions(m_gameWindow.getWidth(), m_gameWindow.getHeight());
            checkCollision(m_gameWindow.getWidth(), m_gameWindow.getHeight());
            swapBuffers();
            drawGame(m_gameWindow.getWidth(), m_gameWindow.getHeight());
            break;

        case WIN:
            showEndScreen(true);
            resetLevel();
            gS = STARTSCREEN;
            break;

        case LOSE:
            showEndScreen(false);
            resetLevel();
            gS = STARTSCREEN;
            break;

        case EXIT:
        default:
            break;
        }
    }
}
