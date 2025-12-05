#include "Missile.h"
#include <Windows.h>

// Fires the missile when spacebar is pressed
void Missile::fireMissle(Player& p)
{
    // Only fire if missile is not already active
    if (!isActive)
    {
        // 32 is the spacebar key code
        if (GetKeyState(32) & 0x8000)
        {
            xPos = p.getXPos();
            yPos = p.getYPos() - 1;
            isActive = true;
        }
    }
}

// Moves the missile upward until it goes off screen
void Missile::update()
{
    if (isActive)
    {
        if (yPos > 0)
            yPos--;
        else
            isActive = false;
    }
}
