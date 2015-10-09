#pragma once

#include <vector>
#include "structs.hpp"
#include "tmx/MapLoader.h"

struct PathNode
{
    PathNode(){}

    PathNode(Point cell, PathNode* parent, float gCost, float hCost)
        : cell(cell) 
        , G(gCost)
        , H(hCost)
        , F(gCost + hCost)
        , parent(parent)
    {}

    bool operator<(const PathNode& p2)
    {
        return F < p2.F;
    }

    void recalcF()
    {
        F = G + H;
    }

    Point cell;
    float G{0};
    float H{0};
    float F{0};
    PathNode* parent;
};

class Pathfind
{
private:
    int _tileSizeX;
    int _tileSizeY;
    int _tilesX;
    int _tilesY;
    Point _start;
    Point _goal;
    sf::RenderWindow* _window;
    std::vector<std::vector<bool>> pathgrid;
    std::vector<Point*> _waypoints;
    std::vector<Wall*>* _walls;
    std::vector<PathNode*> _openlist;
    std::vector<PathNode*> _closedlist;
    bool neighbours[9];

    bool inVector(std::vector<PathNode*> list, Point cell);
    void calcNeighbours(int x, int y);
    bool blocked(Point cell);
    static bool sortNodes(PathNode* n0, PathNode* n1);

public:
    Pathfind(tmx::MapLoader* ml, std::vector<Wall*>* walls, sf::RenderWindow* window); 
    void setStart(const int x, const int y);
    void setGoal(const int x, const int y);
    void buildPathGrid();

    static float distance(Point p1, Point p2);
	static float manhattenDistance(Point p1, Point p2);

    std::vector<Point*> run();

};
