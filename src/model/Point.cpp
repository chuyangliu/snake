#include "model/Point.h"

Point::Point() {} //Constructor

Point::~Point() {} //Destructor

//Function to get and return type value
Point::Type Point::getType() const {
    return type;
}

//Function that take one of the Types declared as enum
void Point::setType(Type type_) {
    type = type_;
}

//Function that take the value of the parent in a variable called p_
void Point::setParent(const Pos &p_) {
    parent = p_;
}

//Function to receive whether or not you visited
void Point::setVisit(const bool v) {
    visit = v;
}

//Function that receive distance data
void Point::setDist(const ValueType dist_) {
    dist = dist_;
}

//Function to receive reference data
void Point::setIndex(const ValueType index_) {
    index = index_;
}

//The value of getting the position of the parent declared as pos
Pos Point::getParent() const {
    return parent;
}

//Function to check whether you visited or not
bool Point::isVisit() const {
    return visit;
}

//Function to get distance data of type ValueType
Point::ValueType Point::getDist() const {
    return dist;
}

//Function to get reference data of type ValueType
Point::ValueType Point::getIndex() const {
    return index;
}
