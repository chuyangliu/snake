#include "Console.h"

#include <iostream>
using namespace std;

int main() {
    Console::writeWithColor("  \n", BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE| BACKGROUND_INTENSITY);
    return 0;
}
