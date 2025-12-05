#pragma once
#include "Entity.h"

class SnakeSegment : public Entity {
public:
    SnakeSegment(int x, int y) : Entity(x, y) {}

    // Returns the character used for a snake segment
    char glyph() const override { return 'x'; }
};
