#pragma once
#include <vector>
#include <memory>
#include "Point.h"
#include "SnakeSegment.h"

enum class SnakeDirection { UP, DOWN, LEFT, RIGHT };

class Snake {
public:
    Snake(int startX = 0, int startY = 0);

    // Resets the snake to a starting position
    void reset(int startX, int startY);

    // Sets the direction the snake will move
    void setDirection(SnakeDirection d);

    // Returns the current direction
    SnakeDirection getDirection() const;

    // Calculates where the head will move next
    Point getNextHeadPosition() const;

    // Moves the snake, growing if needed
    void move(bool grow);

    // Returns true if a point is occupied by the snake
    bool isOccupied(const Point& p) const;

    // Returns the list of segments
    const std::vector<std::unique_ptr<SnakeSegment>>& getSegments() const;

    // Checks if a point collides with the snake's body (not including tail)
    bool collidesWithBody(const Point& p) const;

private:
    std::vector<std::unique_ptr<SnakeSegment>> segments;
    SnakeDirection direction;
};
