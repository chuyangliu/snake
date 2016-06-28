#include "Console.h"
#include <cstdio>
#ifdef __linux__
#include <termios.h>
#include <unistd.h>
#elif _WIN32
#include <Windows.h>
#include <conio.h>
#endif

ConsoleColor::ConsoleColor(const ColorType foreColor_, const ColorType backColor_,
                           const bool &foreIntensified_, const bool &backIntensified_)
: foreColor(foreColor_), backColor(backColor_),
foreIntensified(foreIntensified_), backIntensified(backIntensified_) {
}

#ifdef WIN32

void Console::setColor(const ConsoleColor &consoleColor) {
    WORD color = 0;
    switch (consoleColor.foreColor) {
        case WHITE:
            color |= FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE; break;
        case RED:
            color |= FOREGROUND_RED; break;
        case GREEN:
            color |= FOREGROUND_GREEN; break;
        case BLUE:
            color |= FOREGROUND_BLUE; break;
        case YELLOW:
            color |= FOREGROUND_RED | FOREGROUND_GREEN; break;
        case PURPLE:
            color |= FOREGROUND_BLUE | FOREGROUND_RED; break;
        case CYAN:
            color |= FOREGROUND_BLUE | FOREGROUND_GREEN; break;
        case BLACK:
        default:
            break;
    }
    if (consoleColor.foreIntensified) {
        color |= FOREGROUND_INTENSITY;
    }

    switch (consoleColor.backColor) {
        case WHITE:
            color |= BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE; break;
        case RED:
            color |= BACKGROUND_RED; break;
        case GREEN:
            color |= BACKGROUND_GREEN; break;
        case BLUE:
            color |= BACKGROUND_BLUE; break;
        case YELLOW:
            color |= BACKGROUND_RED | BACKGROUND_GREEN; break;
        case PURPLE:
            color |= BACKGROUND_BLUE | BACKGROUND_RED; break;
        case CYAN:
            color |= BACKGROUND_BLUE | BACKGROUND_GREEN; break;
        case BLACK:
        default:
            break;
    }
    if (consoleColor.backIntensified) {
        color |= BACKGROUND_INTENSITY;
    }

    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hout, color);
}

#endif

void Console::setCursor(const int &x, const int &y) {
#ifdef __linux__ 
    printf("\033[%d;%dH", y + 1, x);  // Param: row and col
#elif _WIN32
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hout, coord);
#else
    // Other platform
#endif
}

void Console::clear() {
#ifdef __linux__ 
    system("clear");
#elif _WIN32
    system("cls");
#else
    // Other platform
#endif
}

void Console::writeWithColor(const std::string &str, const ConsoleColor &consoleColor) {
#ifdef __linux__
    int fore = -1, back = -1;
    switch (consoleColor.backColor) {
        case WHITE:
            back = 47; break;
        case RED:
            back = 41; break;
        case GREEN:
            back = 42; break;
        case BLUE:
            back = 44; break;
        case YELLOW:
            back = 43; break;
        case PURPLE:
            back = 45; break;
        case CYAN:
            back = 46; break;
        case BLACK:
            back = 40; break;
        default:
            break;
    }
    switch (consoleColor.foreColor) {
        case WHITE:
            fore = 37; break;
        case RED:
            fore = 31; break;
        case GREEN:
            fore = 32; break;
        case BLUE:
            fore = 34; break;
        case YELLOW:
            fore = 33; break;
        case PURPLE:
            fore = 35; break;
        case CYAN:
            fore = 36; break;
        case BLACK:
            fore = 30; break;
        default:
            break;
    }
    if (fore != -1 && back != -1) {
        printf("\033[%d;%dm", fore, back);
        printf("%s", str.c_str());
        printf("\033[0m");  // Close all attributes
    }
#elif _WIN32
    setColor(consoleColor);
    printf("%s", str.c_str());
#else
    // Other platform
#endif
}

char Console::getch() {
#ifdef __linux__
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
#elif _WIN32
    return _getch();
#else
    // Other platform
#endif
}
