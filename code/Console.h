#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

class Console {
public:

#ifdef WIN32
    /*
    Set console cursor position.

    @param console x coordinate
    @param console y coordinate
    */
    static void setCursor(const SHORT &x = 0, const SHORT &y = 0);

    /*
    Set console output color.

    @param color FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
                 BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY
                 default color is dark white
    */
    static void setColor(const WORD &color = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
#endif

    /*
    Clear the console.
    */
    static void clear();

    /*
    Write string to console with a given color.

    @param str the string to write
    @param color the string color
    */
    static void writeWithColor(const std::string &str, const WORD &color);

};
