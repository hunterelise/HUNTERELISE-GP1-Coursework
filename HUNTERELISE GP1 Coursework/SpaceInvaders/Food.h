#pragma once
#include "Entity.h"

class Food : public Entity {
public:
    Food() = default;
    Food(int x, int y) : Entity(x, y) {}

    // Returns the character used for food
    char glyph() const override {
        return 'O';
    }
};
