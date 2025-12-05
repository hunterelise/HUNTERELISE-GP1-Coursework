#include "Window.h"

void Window::setWindow(int w, int h)
{
    width = w;
    height = h;

    // Set the console window size and position
    SMALL_RECT windowSize = { 10, 10, static_cast<SHORT>(w + 20), static_cast<SHORT>(h + 20) };
    if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize))
    {
        std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
    }

    // Set the console window title
    if (!SetConsoleTitle(L"Space Invaders"))
    {
        std::cout << "SetConsoleTitle failed with error " << GetLastError() << std::endl;
    }

    // Hide the console cursor
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    // Change the console font size
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };

    HANDLE hcsb = CreateFileA(
        "CONOUT$", GENERIC_WRITE | GENERIC_READ,
        FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    GetCurrentConsoleFontEx(hcsb, FALSE, &cfi);

    cfi.dwFontSize.X = static_cast<SHORT>(cfi.dwFontSize.X * 2);
    cfi.dwFontSize.Y = static_cast<SHORT>(cfi.dwFontSize.Y * 2);

    SetCurrentConsoleFontEx(hcsb, FALSE, &cfi);
}
