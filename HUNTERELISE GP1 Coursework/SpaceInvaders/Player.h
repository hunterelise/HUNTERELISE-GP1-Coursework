#pragma once
#include "GameObject.h"

class Player : public GameObject {
public:
    Player() : GameObject(10) {}

    // Updates the player's movement and actions
    void update() override;
};
