#include "pathfind.hpp"

Pathfind::Pathfind(int tileSizeX, int tileSizeY, sf::RenderWindow* window)
    : _tileSizeX(tileSizeX)
    , _tileSizeY(tileSizeY)
    , _window(window)
{

}

void Pathfind::setStart(const int x, const int y)
{
    _start.x = x;
    _start.y = y;
}

void Pathfind::setGoal(const int x, const int y)
{
    _goal.x = x;
    _goal.y = y;
}

std::vector<Point*> Pathfind::run()
{
    return _waypoints;
}
