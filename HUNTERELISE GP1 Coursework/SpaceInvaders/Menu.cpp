#include "Menu.h"

int menuChoice = 0;

void Menu::run()
{
    bool valid = false;

    // Loop until a valid menu choice is made
    do {
        system("cls");

        std::cout << "1. Play Space Invaders\n";
        std::cout << "2. Play Snake\n";
        std::cout << "3. Quit\n";
        std::cout << "Enter your choice: ";

        // Read a single key without waiting for Enter
        char ch = _getch();
        std::cout << ch << "\n";

        // Process the player's selection
        switch (ch)
        {
        case '1':
            menuChoice = 1;
            valid = true;
            break;

        case '2':
            menuChoice = 2;
            valid = true;
            break;

        case '3':
            menuChoice = 3;
            valid = true;
            break;

        default:
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        }

    } while (!valid);
}
