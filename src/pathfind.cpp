#include "pathfind.hpp"

#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include "tools.hpp"

Pathfind::Pathfind(tmx::MapLoader* ml, std::vector<Wall*>* walls, sf::RenderWindow* window)
    : _window(window)
    , _walls(walls)
{
    _tileSizeX = ml->GetTileSize().x;
    _tileSizeY = ml->GetTileSize().y;
    _tilesX = ml->GetMapSize().x / _tileSizeX;
    _tilesY = ml->GetMapSize().y / _tileSizeY;

    buildPathGrid();
}

void Pathfind::buildPathGrid()
{
    pathgrid.clear();
    int y = 0;
    while(y < _tilesY)
    {
        std::vector<bool> xrow;
        for(int x = 1; x < (_tilesX + 1); x++)
        {
            bool blocked = false;
            Point grid(((x + 1) * _tileSizeX) - (_tilesX / 2), ((y + 1) * _tileSizeY) - (_tilesY / 2));
            for(Wall* wall : *_walls)
            {
                if(wall->AABB.contains(grid.x, grid.y))
                {
                    blocked = true;
                    break;
                }
            }
            xrow.push_back(blocked);
        }
        pathgrid.push_back(xrow);
        y++;
    }
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

    if(blocked(_goal))
        return _waypoints;

    PathNode* current = new PathNode(_start, nullptr, 0.f, Tools::distance(_start, _goal));
    _openlist.push_back(current);

    while(!_openlist.empty())
    {
        std::sort(_openlist.begin(), _openlist.end(), sortNodes);
        current = _openlist[0];

        if(current->cell == _goal)
        {
            while(current->parent != nullptr)
            {
                _waypoints.push_back(new Point((current->cell.x * _tileSizeX) - 8, (current->cell.y * _tileSizeY) - 8));
                current = current->parent;
            }
            
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
            float G = Tools::distance(current->cell, neighbour);
            float H = Tools::manhattenDistance(neighbour, _goal);

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
                        /* node->recalcF(); */
                        node->parent = current;
                    }
                }
            }
			
        }
    }

    return _waypoints;
}

bool Pathfind::sortNodes(PathNode* n0, PathNode* n1)
{
    return (n0->F < n1->F);
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
        int yi = (i / 3) - 1;

        Point neighbour(x + xi, y + yi);

        if(blocked(neighbour))
            neighbours[i] = false;
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

    return pathgrid[cell.y-1][cell.x-1];
}

Point Pathfind::c2p(Point cell)
{
    return Point((cell.x * _tilesX) - (_tilesX / 2), (cell.y * _tilesY) - (_tilesY / 2));
}
