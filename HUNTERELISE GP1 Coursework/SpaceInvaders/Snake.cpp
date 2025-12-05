#include "Snake.h"

Snake::Snake(int startX, int startY)
    : direction(SnakeDirection::RIGHT)
{
    if (startX != 0 || startY != 0)
        reset(startX, startY);
}

void Snake::reset(int startX, int startY)
{
    segments.clear();

    // Place tail on the left and head on the right
    segments.emplace_back(std::make_unique<SnakeSegment>(startX - 2, startY)); // tail
    segments.emplace_back(std::make_unique<SnakeSegment>(startX - 1, startY)); // middle
    segments.emplace_back(std::make_unique<SnakeSegment>(startX, startY));     // head

    // Move head to index 0
    std::rotate(segments.rbegin(), segments.rbegin() + 1, segments.rend());

    direction = SnakeDirection::RIGHT;
}

void Snake::setDirection(SnakeDirection d)
{
    if (segments.empty()) {
        direction = d;
        return;
    }

    // Prevent turning directly into the opposite direction
    switch (direction)
    {
    case SnakeDirection::UP:
        if (d != SnakeDirection::DOWN) direction = d;
        break;
    case SnakeDirection::DOWN:
        if (d != SnakeDirection::UP) direction = d;
        break;
    case SnakeDirection::LEFT:
        if (d != SnakeDirection::RIGHT) direction = d;
        break;
    case SnakeDirection::RIGHT:
        if (d != SnakeDirection::LEFT) direction = d;
        break;
    }
}

SnakeDirection Snake::getDirection() const
{
    return direction;
}

Point Snake::getNextHeadPosition() const
{
    if (segments.empty())
        return Point{ 0, 0 };

    Point newHead = segments.front()->pos;

    // Move head one tile in the current direction
    switch (direction)
    {
    case SnakeDirection::UP:    newHead.y -= 1; break;
    case SnakeDirection::DOWN:  newHead.y += 1; break;
    case SnakeDirection::LEFT:  newHead.x -= 1; break;
    case SnakeDirection::RIGHT: newHead.x += 1; break;
    }

    return newHead;
}

void Snake::move(bool grow)
{
    Point newHeadPos = getNextHeadPosition();

    // Insert new head at the front
    segments.insert(segments.begin(), std::make_unique<SnakeSegment>(newHeadPos.x, newHeadPos.y));

    // Remove tail unless the snake should grow
    if (!grow && !segments.empty())
        segments.pop_back();
}

bool Snake::isOccupied(const Point& p) const
{
    for (const auto& seg : segments)
        if (seg->pos == p)
            return true;
    return false;
}

bool Snake::collidesWithBody(const Point& p) const
{
    // Ignore the tail because it will move
    if (segments.size() <= 1)
        return false;

    // Check from head to the segment before tail
    for (size_t i = 0; i + 1 < segments.size(); ++i)
    {
        if (segments[i]->pos == p)
            return true;
    }
    return false;
}

const std::vector<std::unique_ptr<SnakeSegment>>& Snake::getSegments() const
{
    return segments;
}
