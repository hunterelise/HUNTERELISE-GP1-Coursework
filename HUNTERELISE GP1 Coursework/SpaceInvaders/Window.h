#pragma once
#include <iostream>
#include <Windows.h>

class Window {
public:
    Window() : width(0), height(0), hConsole(GetStdHandle(STD_OUTPUT_HANDLE)) {}

    // Sets the console window size
    void setWindow(int width, int height);

    // Returns the window width
    int getWidth() const { return width; }

    // Returns the window height
    int getHeight() const { return height; }

private:
    int width;
    int height;
    HANDLE hConsole;
};
