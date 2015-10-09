#pragma once

#include "structs.hpp"

namespace Tools
{
    inline float distance(Point start, Point end)
    {
        float dx = start.x - end.x;
        float dy = end.y - end.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    inline float manhattenDistance(Point start, Point end)
    {
		float x = (float)(std::fabs((float)(start.x - end.x)));
		float y = (float)(std::fabs((float)(start.y - end.y)));
		return x + y;
    }
}

namespace DrawTools
{
    inline void drawCircle(float radius, Point position, sf::Color color, sf::RenderTarget* window)
    {
        sf::CircleShape circle(radius);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(position.x, position.y);
        circle.setFillColor(color);
        window->draw(circle);
    }

    inline void drawLine(Point start, Point end, sf::Color color, sf::RenderTarget* window)
    {
        sf::Vertex line[2];
        line[0].position = sf::Vector2f(start.x, start.y);
        line[1].position = sf::Vector2f(end.x, end.y);
        line[0].color = color;
        line[1].color = color;
        window->draw(line, 2, sf::Lines);
    }
}

