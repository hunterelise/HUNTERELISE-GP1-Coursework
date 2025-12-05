#include "SnakeGameSource.h"
#include "ConsoleUtils.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace std::chrono;

SnakeGameSource::SnakeGameSource(int w, int h, int fps)
    : width(w), height(h), targetFps(fps), snake()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

SnakeGameSource::~SnakeGameSource()
{
    // Cleanup is handled by smart pointers
}

// Sets starting values and positions
void SnakeGameSource::init()
{
    score = 0;
    lives = 3;
    extraLife.reset();
    food.reset();

    int cx = width / 2;
    int cy = height / 2;

    snake.reset(cx, cy);

    spawnFood();
    hideCursor();
    renderFrame();

    sS = SnakeState::PLAY;
}

// Main game loop for the snake game
void SnakeGameSource::run()
{
    milliseconds frameDelay(static_cast<int>(1000.0 / targetFps));

    sS = SnakeState::PLAY;

    while (sS != SnakeState::EXIT)
    {
        switch (sS)
        {
        case SnakeState::PLAY:
        {
            auto start = high_resolution_clock::now();

            handleInput();
            update();
            renderFrame();

            auto elapsed = high_resolution_clock::now() - start;
            if (elapsed < frameDelay)
                this_thread::sleep_for(frameDelay - elapsed);

            break;
        }

        case SnakeState::GAME_OVER:
        {
            flashDeath();

            setCursor(0, height + 3);
            setColor(4);
            cout << "GAME OVER\n";
            setColor(7);
            cout << "Final Score: " << score << "\n";
            cout << "Press Q to return to menu...";

            // Wait for Q to go back to menu
            while (true)
            {
                int ch = _getch();
                if (ch == 'q' || ch == 'Q')
                    break;
            }

            setGameState(SnakeState::EXIT);
            break;
        }

        default:
            setGameState(SnakeState::EXIT);
            break;
        }
    }
}

// Creates food at a free random position
void SnakeGameSource::spawnFood()
{
    while (true)
    {
        int fx = rand() % width;
        int fy = rand() % height;
        Point fp(fx, fy);

        // Avoid edges
        if (fx <= 1 || fy <= 1 || fx >= width - 2 || fy >= height - 2)
            continue;

        // Avoid snake and extra life
        if (!snake.isOccupied(fp) && (!extraLife || !(fp == *extraLife)))
        {
            food = std::make_unique<Food>(fx, fy);

            // Chance to spawn extra life if not at max lives
            if (lives < 3 && (rand() % 5 == 0))
                spawnExtraLife();

            break;
        }
    }
}

// Creates an extra life at a free position
void SnakeGameSource::spawnExtraLife()
{
    while (true)
    {
        int x = rand() % width;
        int y = rand() % height;
        Point p(x, y);

        if (!snake.isOccupied(p) && (!food || !(food->pos == p)))
        {
            extraLife = std::make_unique<Point>(p);
            break;
        }
    }
}

// Reads keyboard input and changes direction or exits
void SnakeGameSource::handleInput()
{
    if (!_kbhit()) return;

    int ch = _getch();

    // Arrow keys
    if (ch == 0 || ch == 224)
    {
        int key = _getch();
        switch (key)
        {
        case 72: snake.setDirection(SnakeDirection::UP);    break;  // Up arrow
        case 80: snake.setDirection(SnakeDirection::DOWN);  break;  // Down arrow
        case 75: snake.setDirection(SnakeDirection::LEFT);  break;  // Left arrow
        case 77: snake.setDirection(SnakeDirection::RIGHT); break;  // Right arrow
        }
    }
    else
    {
        // Quit game
        if (ch == 'q' || ch == 'Q')
        {
            setGameState(SnakeState::EXIT);
        }
    }
}

// Updates snake position, collisions, food, and lives
void SnakeGameSource::update()
{
    // Next head position based on current direction
    Point nextHead = snake.getNextHeadPosition();

    // Check collision with walls or own body (tail is ignored)
    if (nextHead.x < 0 || nextHead.x >= width ||
        nextHead.y < 0 || nextHead.y >= height ||
        snake.collidesWithBody(nextHead))
    {
        lives--;

        if (lives > 0)
        {
            // Lose a life and restart the round
            flashDeath();

            int cx = width / 2;
            int cy = height / 2;
            snake.reset(cx, cy);

            food.reset();
            extraLife.reset();
            spawnFood();
        }
        else
        {
            // No lives left, game over
            flashDeath();
            setGameState(SnakeState::GAME_OVER);
        }
        return;
    }

    bool grow = false;

    // If the next tile has food, increase score and grow
    if (food && nextHead == food->pos)
    {
        score += 10;
        spawnFood();
        grow = true;
    }

    // Move snake, growing if needed
    snake.move(grow);

    // Check for extra life pickup
    if (extraLife && nextHead == *extraLife)
    {
        lives++;
        extraLife.reset();
        flashExtraLife();
    }
}

// Draws the entire snake game frame
void SnakeGameSource::renderFrame()
{
    std::vector<std::string> buffer(height, std::string(width, ' '));

    // Food
    if (food &&
        food->pos.x >= 0 && food->pos.x < width &&
        food->pos.y >= 0 && food->pos.y < height)
    {
        buffer[food->pos.y][food->pos.x] = food->glyph();
    }

    // Extra life
    if (extraLife &&
        extraLife->x >= 0 && extraLife->x < width &&
        extraLife->y >= 0 && extraLife->y < height)
    {
        buffer[extraLife->y][extraLife->x] = '+';
    }

    // Snake segments, head at index 0
    const auto& segments = snake.getSegments();
    for (size_t i = 0; i < segments.size(); ++i)
    {
        const auto& seg = segments[i];
        int sx = seg->pos.x;
        int sy = seg->pos.y;

        if (sx >= 0 && sx < width &&
            sy >= 0 && sy < height)
        {
            buffer[sy][sx] = (i == 0 ? '@' : 'x');
        }
    }

    // Draw border and contents
    setCursor(0, 0);
    cout << "+" << std::string(width, '-') << "+\n";
    for (int y = 0; y < height; ++y)
    {
        cout << "|" << buffer[y] << "|\n";
    }
    cout << "+" << std::string(width, '-') << "+\n";
    cout << "Score: " << score << "  Lives: " << lives
        << "  (Arrow Keys, Q to Quit)\n";
}

// Flashes red to show death
void SnakeGameSource::flashDeath()
{
    for (int i = 0; i < 3; ++i)
    {
        setColor(4);
        renderFrame();
        this_thread::sleep_for(milliseconds(150));

        setColor(7);
        renderFrame();
        this_thread::sleep_for(milliseconds(150));
    }
}

// Flashes green to show extra life pickup
void SnakeGameSource::flashExtraLife()
{
    for (int i = 0; i < 3; ++i)
    {
        setColor(2);
        renderFrame();
        this_thread::sleep_for(milliseconds(150));

        setColor(7);
        renderFrame();
        this_thread::sleep_for(milliseconds(150));
    }
}
