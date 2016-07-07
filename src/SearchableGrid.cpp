#include "SearchableGrid.h"

SearchableGrid::SearchableGrid() {
}

SearchableGrid::~SearchableGrid() {
}

void SearchableGrid::setG(const value_type g_) {
    g = g_;
}

void SearchableGrid::setH(const value_type h_) {
    h = h_;
}

void SearchableGrid::setParent(const Direction &p_) {
    parent = p_;
}

void SearchableGrid::setLocation(const Point &p) {
    loc = p;
}

SearchableGrid::value_type SearchableGrid::getG() const {
    return g;

}

SearchableGrid::value_type SearchableGrid::getH() const {
    return h;
}

SearchableGrid::value_type SearchableGrid::getF() const {
    return g + h;
}

Direction SearchableGrid::getParent() const {
    return parent;
}

Point SearchableGrid::getLocation() const {
    return loc;
}

bool operator<(const SearchableGrid &a, const SearchableGrid &b) {
    return a.getF() < b.getF();
}

bool operator>(const SearchableGrid &a, const SearchableGrid &b) {
    return a.getF() > b.getF();
}

bool operator<=(const SearchableGrid &a, const SearchableGrid &b) {
    return a.getF() <= b.getF();
}

bool operator>=(const SearchableGrid &a, const SearchableGrid &b) {
    return a.getF() >= b.getF();
}
