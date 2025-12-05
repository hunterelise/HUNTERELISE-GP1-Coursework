#include "Alien.h"
#include <iostream>

int Alien::m_speed = 1;

Alien::Alien()
    : GameObject(), m_isActive(true)
{
}

Alien::~Alien()
{
}

Alien::Alien(const Alien& other)
    : GameObject(other), m_isActive(other.m_isActive)
{
}

// Sets the alien's position
void Alien::setPosition(int x, int y)
{
    xPos = x;
    yPos = y;
}

// Sets whether the alien is active
void Alien::setActive(bool state)
{
    m_isActive = state;
}

// Returns whether the alien is active
bool Alien::isActive() const
{
    return m_isActive;
}

// Sets the shared alien speed
void Alien::setSpeed(int s)
{
    m_speed = s;
}

// Returns the shared alien speed
int Alien::getSpeed()
{
    return m_speed;
}
