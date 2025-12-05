#pragma once
#include "GameObject.h"

class Ground : public GameObject {
public:
    Ground() = default;
    explicit Ground(int y) { xPos = 0; yPos = y; }

    // Draws the ground
    void draw() override;
};
