#ifndef PATHFIND_HPP
#define PATHFIND_HPP

#include <structs.hpp>

class Pathfind
{
private:
    int _tileSizeX;
    int _tileSizeY;
    Point _start;
    Point _goal;
    sf::RenderWindow* _window;
    std::vector<Point*> _waypoints;

public:
    Pathfind(const int tileSizeX, const int tileSizeY, sf::RenderWindow* window); 
    void setStart(const int x, const int y);
    void setGoal(const int x, const int y);
    std::vector<Point*> run();

private:

public:

};


#endif
