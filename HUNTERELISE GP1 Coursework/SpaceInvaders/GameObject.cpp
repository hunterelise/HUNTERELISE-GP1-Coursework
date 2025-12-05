#include "GameObject.h"

GameObject::GameObject() : xPos(0), yPos(0)
{
}

GameObject::GameObject(int x) : xPos(x), yPos(0)
{
}

GameObject::~GameObject()
{
}

// Base update does nothing
void GameObject::update()
{
}

// Base draw prints a message
void GameObject::draw()
{
    std::cout << "called from base\n";
}

// Returns the x position
int GameObject::getXPos() const
{
    return xPos;
}

// Returns the y position
int GameObject::getYPos() const
{
    return yPos;
}

// Sets the x position
void GameObject::setXPos(int x)
{
    xPos = x;
}

// Sets the y position
void GameObject::setYPos(int y)
{
    yPos = y;
}
