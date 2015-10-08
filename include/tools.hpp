#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "structs.hpp"

namespace DrawTools
{
    void drawCircle(float radius, Point position, sf::Color color, sf::RenderTarget* window)
    {
        sf::CircleShape circle(radius);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(position.x, position.y);
        circle.setFillColor(color);
        window->draw(circle);
    }

    void drawLine(Point start, Point end, sf::Color color, sf::RenderTarget* window)
    {
        sf::Vertex line[2];
        line[0].position = sf::Vector2f(start.x, start.y);
        line[1].position = sf::Vector2f(end.x, end.y);
        line[0].color = color;
        line[1].color = color;
        window->draw(line, 2, sf::Lines);
    }
}

#endif
