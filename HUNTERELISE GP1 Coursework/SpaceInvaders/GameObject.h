#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

class GameObject {
public:
    GameObject();
    GameObject(int x);
    virtual ~GameObject();

    // Updates the object
    virtual void update();

    // Draws the object
    virtual void draw();

    // Returns the x position
    int getXPos() const;

    // Returns the y position
    int getYPos() const;

    // Sets the x position
    void setXPos(int x);

    // Sets the y position
    void setYPos(int y);

protected:
    int xPos; // X coordinate
    int yPos; // Y coordinate
};
