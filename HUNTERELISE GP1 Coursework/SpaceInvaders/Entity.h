#pragma once
#include "Point.h"

class Entity {
public:
    Point pos; // Position of the entity

    Entity() = default;
    Entity(int x, int y) : pos{ x, y } {}
    virtual ~Entity() {}

    // Updates the entity
    virtual void update() {}

    // Returns the character used to draw the entity
    virtual char glyph() const {
        return '?';
    }
};
