#include "Console.h"

#include <iostream>
using namespace std;

int main() {
    Console::setColor(BACKGROUND_RED | BACKGROUND_INTENSITY);
    cout << "  " << endl;
    return 0;
}
