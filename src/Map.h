#pragma once

#include "SearchableGrid.h"
#include <list>
#include <vector>
#include <queue>
#include <unordered_set>
#include <functional>

/*
A map of the game
*/
class Map {
public:
    typedef std::vector<std::vector<SearchableGrid>> content_type;
    typedef content_type::size_type size_type;
    typedef std::priority_queue<SearchableGrid, std::vector<SearchableGrid>, std::greater<SearchableGrid>> min_heap;
    typedef std::unordered_set<Point, decltype(Point::hash)*> hash_table;

    Map(const size_type &rowCnt_ = 20, const size_type &colCnt_ = 20);
    ~Map();

    /*
    Return the grid at the point.
    Two versions for const and non-const object.
    */
    SearchableGrid& getGrid(const Point &p);
    const SearchableGrid& getGrid(const Point &p) const;

    /*
    Check if the grid at the point is an unsafe grid.
    Unsafe grid type: snake body, snake head, snake tail, wall

    @param p the point to check
    */
    bool isUnsafe(const Point &p) const;

    /*
    Check if the point is inside the map
    (not include the boundary).

    @param p the point to check
    */
    bool isInside(const Point &p) const;

    /*
    Check whether the map is filled with snake body.
    */
    bool isFilledWithBody() const;

    /*
    Create a food on the map randomly.
    */
    void createFood();

    /*
    Remove food if exists
    */
    void removeFood();

    /*
    Check if there is food on the map.
    */
    bool hasFood() const;

    /*
    Return the food position.
    */
    const Point& getFood() const;

    /*
    Return the count of the rows.
    */
    size_type getRowCount() const;

    /*
    Return the count of the columns.
    */
    size_type getColCount() const;

    /*
    Set whether to show the details of the search algorithm.
    */
    void setShowSearchDetails(const bool &b);

    /*
    Calculate the manhatten distance between two points.

    @param from the start point
    @param to the end point
    @return the manhatten distance between two points
    */
    static unsigned getManhattenDist(const Point &from, const Point &to);

    /*
    Calculate the geometric distance between two points.

    @param from the start point
    @param to the end point
    @return the geometric distance between two points
    */
    static unsigned getGeometricDist(const Point &from, const Point &to);

    /*
    Find the shortest path from the start
    point to the end point.
    Algorithm reference:
    1. https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    2. https://en.wikipedia.org/wiki/A*_search_algorithm

    @param from the start point
    @param to the end point
    @param path the result will be stored as a list of Direction
                in this field. If there is no path between the two
                points, the size of this field will be zero.
    */
    void findMinPath(const Point &from, const Point &to, std::list<Direction> &path);

    /*
    Find the longest(approximately) path from the start
    point to the end point.
    Algorithm reference:
    1. https://en.wikipedia.org/wiki/Longest_path_problem
    2. https://en.wikipedia.org/wiki/Depth-first_search

    @param from the start point
    @param to the end point
    @param path the result will be stored as a list of Direction
                in this field. If there is no path between the two
                points, the size of this field will be zero.
    */
    void findMaxPath(const Point &from, const Point &to, std::list<Direction> &path);

private:
    Point food = Point::INVALID;

    content_type content;  // The elements on the map

    bool showSearchDetails = false;

    /*
    Return a random number in [min, max]
    */
    static int random(const int min, const int max);

    /*
    Randomly rearrage the elements in the vector.

    @param points the vector to rearrage
    */
    static void randomChange(std::vector<Point> &points);

    /*
    Initialize map content.
    */
    void init();

    /*
    Get a food position on the map
    WARNING: if there is no space for food,
    this will be an endless loop.

    @return the new food position
    */
    Point createFoodPos() const;

    /*
    Check if the grid at the point must be ignored by the search algorithm
    Unsearched grid type: snake body, snake head, snake tail, wall, food

    @param p the point to check
    */
    bool isUnsearch(const Point &p) const;

    /*
    DFS algorithm to find the longest(approximately) path.

    @param n current search point
    @param from the start point
    @param to the end point
    @param tot current path length
    @param max current maximum path length
    @param closeList stores the points that have been visited
    @param path the result will be stored in this field
    */
    void dfs(const Point &n,
             const Point &from,
             const Point &to,
             const long tot,
             long &max,
             Map::hash_table &closeList,
             std::list<Direction> &path);

    /*
    Construct the move path from the start
    point to the end point.

    @param from the start point
    @param to the end point
    @param path the result will be stored as a list of Direction
    in this field. If there is no path between the two
    points, the size of this field will be zero.
    */
    void constructPath(const Point &from, const Point &to, std::list<Direction> &path);

    /*
    Compute the H(Heuristic) value from the start point to the end point.
    This is the heuristic for shortest path search algorithm.

    @param from the start point
    @param to the end point
    @return the heuristic value
    */
    SearchableGrid::value_type estimateH1(const Point &from, const Point &to) const;

    /*
    Compute the H(Heuristic) value from the start point to the end point.
    This is the heuristic for longest path search algorithm.

    @param from the start point
    @param to the end point
    @return the heuristic value
    */
    SearchableGrid::value_type estimateH2(const Point &from, const Point &to) const;

    /*
    Sort the points by the value calculated by estimateH2()
    in descending order.

    @param points the points array to sort
    @param goal the goal point
    */
    void sortByH2(std::vector<Point> &points, const Point &goal);

    /*
    Show a visited node on the map. This method is designed for
    showing search details. It first checks if showSearchDetails
    field is true.

    @param n the point of the node
    @param type the new type of the grid at the point n
    */
    void showVisitedNodeIfNeeded(const Point &n, const Grid::GridType &type);

    /*
    Show the path on the map. This method is designed for
    showing search details. It first checks if showSearchDetails
    field is true.

    @param start the start point
    @param path the path to show
    */
    void showPathIfNeeded(const Point &start, const std::list<Direction> &path);
};
