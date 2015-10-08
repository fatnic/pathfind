#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <SFML/Graphics.hpp> 

typedef sf::Vector2<double> Point;

struct Ray
{
    double angle;
    Point start, end;
};

struct Wall
{
    sf::IntRect AABB;

    Wall(const int x, const int y, const int width, const int height)
    {
        AABB.left = x;
        AABB.top = y;
        AABB.width = width;
        AABB.height = height;
    }

    void draw(sf::RenderTarget* window)
    {
        sf::RectangleShape wall;
        wall.setPosition(AABB.left, AABB.top);
        wall.setSize(sf::Vector2f(AABB.width, AABB.height));
        wall.setFillColor(sf::Color::Green);
        window->draw(wall);
    }
};

#endif
