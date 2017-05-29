#ifndef SNAKE_POINT_H_ //Prevent double inclusion and compilation of SNAKE_POINT_H_ when including header files.
#define SNAKE_POINT_H_

#include "model/Pos.h"
#include <cstdint> //Int8_t, int64_t, and macros for the minimum and maximum values for each type

/*
Point on the game map.
*/
class Point {
public:
    typedef uint32_t ValueType; //uint32_t type ValueType Declaration

    static const ValueType MAX_VALUE = UINT32_MAX; //0xffffffffui32
	/*
	Enum type containing information
	*/
    enum Type {
        EMPTY,
        WALL,
        FOOD,
        SNAKE_BODY,
        SNAKE_HEAD,
        SNAKE_TAIL,
        TEST_VISIT,
        TEST_PATH
    };

    Point(); //Constructor
    ~Point(); //Destructor

    void setType(Type type_); //Functions that take one of the types declared as enum
	void setParent(const Pos &p_); //Functions that take the value of the parent in a variable called p_
    void setVisit(const bool v); //Function to receive whether or not you visited
    void setDist(const ValueType dist_); //Functions that receive distance data
    void setIndex(const ValueType index_); //Function to receive reference data
    Type getType() const; //Function to get and return type value
    Pos getParent() const; //Functions that can not change the value to get the position of the parent declared as Pos
    bool isVisit() const; //Functions that can not change the value to check whether you visited or not
    ValueType getDist() const; //Function to get distance data of type ValueType
    ValueType getIndex() const; //Function to get reference data of type ValueType

private:
    Type type = EMPTY; //Type is initialized to empty
    Pos parent; //Return variable of position of parent of type pos
    bool visit; //Member variable to check whether you visited or not
    ValueType dist; //Distance data setting
    ValueType index; //Reference

};

#endif
