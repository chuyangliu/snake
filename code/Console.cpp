#include "Console.h"

#ifdef WIN32

void Console::setCursor(const SHORT &x, const SHORT &y) {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hout, coord);
}

void Console::setColor(const WORD &color) {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, color);
}

#endif
