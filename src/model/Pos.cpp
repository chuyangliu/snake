#include "model/Pos.h"
#include "util/util.h"

//Initialize to 0 sizeType type variable x_ and y_ after declaration as copy constructor
Pos::Pos(const SizeType x_, const SizeType y_) : x(x_), y(y_) {}

Pos::~Pos() {}

//SizeType function to get x coordinate value
Pos::SizeType Pos::getX() const {
    return x;
}

//SizeType function to get y coordinate value
Pos::SizeType Pos::getY() const {
    return y;
}

//X coordinate value receiving function
void Pos::setX(const SizeType x_) {
    x = x_;
}

//Y coordinate value receiving function
void Pos::setY(const SizeType y_) {
    y = y_;
}

//The function that takes a as a reference and adds the value of b
Pos operator+(const Pos &a, const Pos &b) {
    return Pos(a.x + b.x, a.y + b.y);
}

//Function that returns a result if a and b are equal if they are equal
bool operator==(const Pos &a, const Pos &b) {
    return a.x == b.x && a.y == b.y;
}

//Function that returns a different result if a and b are different
bool operator!=(const Pos &a, const Pos &b) {
    return !(a == b);
}

std::string Pos::toString() const {
    return "(" + util::toString(x) + "," + util::toString(y) + ")";
}

// Function that returns the orientation of the specified position for this position
Direction Pos::getDirectionTo(const Pos &p) const {
    if (x == p.x) { //When the current x coordinate position matches the specified x coordinate value
        if (p.y - y == 1) { 
            return RIGHT;
        } else if (y - p.y == 1) { 
            return LEFT;
        }
    } else if (y == p.y) { //When the current y coordinate position matches the specified y coordinate value
        if (p.x - x == 1) {
            return DOWN;
        } else if (x - p.x == 1) {
            return UP;
        }
    }
    return NONE; //Returns Direction :: NONE if the specified position is not adjacent to this position.
}

//Returns the contiguous position in the specified direction
Pos Pos::getAdj(const Direction d) const {
    switch (d) { //d:Direction selection variable
        case LEFT:
            if (y) {
                return Pos(x, y - 1);
            }
            break;
        case UP:
            if (x) {
                return Pos(x - 1, y);
            }
            break;
        case RIGHT:
            return Pos(x, y + 1);
        case DOWN:
            return Pos(x + 1, y);
        default:
            break;
    }
    return *this; //Returns the position if there is no valid position in the specified direction.
}

//Functions that return all adjacent positions
std::vector<Pos> Pos::getAllAdj() const {
    std::vector<Pos> adj;
    for (int i = 1; i <= 4; ++i) {
        Pos p = getAdj((Direction)i); //Initialize to the current position
        if (p != *this) { 
            adj.push_back(p); //Access to p's memory space
        }
    }
    return adj;
}
