#include "Player.h"
#include <Windows.h>

// Updates player movement based on input
void Player::update()
{
    // Check for left movement keys
    bool leftPressed =
        (GetKeyState('A') & 0x8000) ||
        (GetKeyState(VK_LEFT) & 0x8000);

    // Check for right movement keys
    bool rightPressed =
        (GetKeyState('D') & 0x8000) ||
        (GetKeyState(VK_RIGHT) & 0x8000);

    // Move left if within screen bounds
    if (leftPressed)
    {
        if (xPos > 1)
            xPos--;
    }
    // Move right if within screen bounds
    else if (rightPressed)
    {
        if (xPos < 79)
            xPos++;
    }
}
