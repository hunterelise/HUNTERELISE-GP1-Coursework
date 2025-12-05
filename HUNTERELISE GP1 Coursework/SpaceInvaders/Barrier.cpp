#include "Barrier.h"

Barrier::Barrier()
    : GameObject(), isActive(true)
{
}

Barrier::Barrier(const Barrier& other)
    : GameObject(other), isActive(other.isActive)
{
}

Barrier::~Barrier()
{
}

// Sets the barrier position
void Barrier::setPosition(int x, int y)
{
    xPos = x;
    yPos = y;
}

// Returns the x position
int Barrier::getXPosition() const
{
    return xPos;
}

// Returns the y position
int Barrier::getYPosition() const
{
    return yPos;
}
