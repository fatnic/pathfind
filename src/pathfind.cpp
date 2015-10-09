#include "pathfind.hpp"

#include <iostream>
#include <algorithm>
#include <stdlib.h>

void drawCircle(float radius, Point position, sf::Color color, sf::RenderTarget* window)
{
    sf::CircleShape circle(radius);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(position.x, position.y);
    circle.setFillColor(color);
    window->draw(circle);
}

Pathfind::Pathfind(tmx::MapLoader* ml, std::vector<Wall*>* walls, sf::RenderWindow* window)
    : _window(window)
    , _walls(walls)
{
    _tileSizeX = ml->GetTileSize().x;
    _tileSizeY = ml->GetTileSize().y;
    _tilesX = ml->GetMapSize().x / _tileSizeX;
    _tilesY = ml->GetMapSize().y / _tileSizeY;
}

void Pathfind::setStart(const int x, const int y)
{
    _start.x = (x / _tileSizeX) + 1;
    _start.y = (y / _tileSizeY) + 1;
}

void Pathfind::setGoal(const int x, const int y)
{
    _goal.x = (x / _tileSizeX) + 1;
    _goal.y = (y / _tileSizeY) + 1;
}

std::vector<Point*> Pathfind::run()
{

    for(auto& node : _waypoints)
        delete node;
    _waypoints.clear();

    PathNode* current = new PathNode(_start, nullptr, 0.f, distance(_start, _goal));
    _openlist.push_back(current);

    while(!_openlist.empty())
    {
        std::sort(_openlist.begin(), _openlist.end());
        current = _openlist[0];

        if(current->cell == _goal)
        {
            std::cout << "found goal\n";
            while(current->parent != nullptr)
            {
                _waypoints.push_back(new Point((current->cell.x * _tileSizeX) - 8, (current->cell.y * _tileSizeY) - 8));
                current = current->parent;
            }
            
            std::cout << _waypoints.size() << " waypoints...\n";

            for(auto& node : _openlist)
                delete node;
            _openlist.clear();

            for(auto& node : _closedlist)
                delete node;
            _closedlist.clear();

            std::reverse(_waypoints.begin(), _waypoints.end());
            return _waypoints;
        }

        _openlist.erase(_openlist.begin());
        _closedlist.push_back(current);

        int x = current->cell.x;
        int y = current->cell.y;

        calcNeighbours(x, y);

        for(int i = 0; i < 9; i++)
        {
            if(neighbours[i] == false) continue;

            int xi = (i % 3) - 1;
            int yi = (i / 3) - 1;

            Point neighbour(x + xi, y + yi);
            float G = distance(current->cell, neighbour);
            float H = manhattenDistance(neighbour, _goal);

            if(inVector(_closedlist, neighbour) && G >= current->G) continue;

            if(!inVector(_openlist, neighbour))
            {
                PathNode* newNode = new PathNode(neighbour, current, G, H);
                _openlist.push_back(newNode);
            }
            else
            {
                for(PathNode* node : _openlist)
                {
                    if((node->cell == neighbour) && G + H < node->F)
                    {
                        node->G = G;
                        node->H = H;
                        node->recalcF();
                        node->parent = current;
                    }
                }
            }
			
        }
    }

    return _waypoints;
}

float Pathfind::distance(Point p1, Point p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

float Pathfind::manhattenDistance(Point p1, Point p2)
{     
	float x = (float)(std::fabs((float)(p1.x - p2.x)));
	float y = (float)(std::fabs((float)(p1.y - p2.y)));
	return x + y;
}

bool Pathfind::inVector(std::vector<PathNode*> list, Point cell)
{
    for(std::size_t i = 0; i < list.size(); i++)
    {
        if(list[i]->cell.x == cell.x && list[i]->cell.y == cell.y)
            return true;
    }
    return false;
}

void Pathfind::calcNeighbours(int x, int y)
{
    for(int i = 0; i < 9; i++)
        neighbours[i] = true;

    neighbours[4] = false;

    for(int i = 0; i < 9; i++)
    {
        int xi = (i % 3) - 1;
        int yi = (i % 3) - 1;

        Point neighbour(x + xi, y + yi);

        /* drawCircle(2, Point((x+xi) * 16 - 8, (y+yi) * 16 - 8), sf::Color::Red, _window); */
        /* _window->display(); */

        if(blocked(neighbour)) neighbours[i] = false;
    }

    if(neighbours[1] == false)
    {
        neighbours[0] = false;
        neighbours[2] = false;
    }

    if(neighbours[3] == false)
    {
        neighbours[0] = false;
        neighbours[6] = false;
    }

    if(neighbours[5] == false)
    {
        neighbours[2] = false;
        neighbours[8] = false;
    }

    if(neighbours[7] == false)
    {
        neighbours[6] = false;
        neighbours[8] = false;
    }
}

bool Pathfind::blocked(Point cell)
{
    if(cell.x < 1 || cell.x > _tilesX || cell.y < 1 || cell.y > _tilesY)
        return true;

    for(Wall* wall : *_walls)
    {
        if(wall->AABB.contains(cell.x * _tileSizeX - (_tileSizeX/2), cell.y * _tileSizeY - (_tileSizeY/2)))
            return true;
    }
    return false;
}
