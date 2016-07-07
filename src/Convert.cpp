#include "Convert.h"

std::string Convert::toString(int data) {
    char str[255];
    sprintf(str, "%d", data);
    return std::string(str);
}
