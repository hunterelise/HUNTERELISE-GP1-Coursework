#pragma once
#include <Windows.h>
#include <iostream>

// Moves the console cursor to a position
inline void setCursor(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = static_cast<SHORT>(x);
    coord.Y = static_cast<SHORT>(y);
    SetConsoleCursorPosition(hOut, coord);
}

// Changes the console text color
inline void setColor(WORD attributes) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, attributes);
}

// Hides the console cursor
inline void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
