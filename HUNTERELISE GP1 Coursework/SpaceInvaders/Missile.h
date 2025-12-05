#pragma once
#include "GameObject.h"
#include "Player.h"

class Missile : public GameObject {
public:
    Missile() : isActive(false) {}

    // Fires the missile from the player position
    void fireMissle(Player& p);

    // Updates missile movement
    void update();

    // Sets whether the missile is active
    void setActive(bool state) { isActive = state; }

    // Returns whether the missile is active
    bool getState() const { return isActive; }

private:
    bool isActive; // Tracks if the missile is active
};
