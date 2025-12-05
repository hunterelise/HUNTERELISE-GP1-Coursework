#pragma once
#include "GameObject.h"

class Alien : public GameObject {
public:
    Alien();
    ~Alien();

    Alien(const Alien& other);

    // Sets the position of the alien
    void setPosition(int x, int y);

    // Sets whether the alien is active
    void setActive(bool state);

    // Returns true if the alien is active
    bool isActive() const;

    // Sets the movement speed shared by all aliens
    static void setSpeed(int s);

    // Gets the movement speed shared by all aliens
    static int getSpeed();

private:
    bool m_isActive;     // Tracks if the alien is active
    static int m_speed;  // Speed value shared by all aliens
};
