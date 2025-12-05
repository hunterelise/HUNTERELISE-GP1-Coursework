#pragma once
#include "GameObject.h"

class Barrier : public GameObject {
public:
    Barrier();
    Barrier(const Barrier& other);
    ~Barrier();

    // Sets the barrier position
    void setPosition(int x, int y);

    // Returns the x position
    int getXPosition() const;

    // Returns the y position
    int getYPosition() const;

    // Sets whether the barrier is active
    void setState(bool in) {
        isActive = in;
    }

    // Returns whether the barrier is active
    bool getState() const {
        return isActive;
    }

private:
    bool isActive; // Tracks if the barrier is active
};
