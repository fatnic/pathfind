#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "structs.hpp"
#include "pathfind.hpp"
#include "drawtools.hpp"
#include "tmx/MapLoader.h"

void getWallsFromTmx(tmx::MapLoader* ml, std::vector<Wall*>& walls)
{
    for(tmx::MapLayer& layer : ml->GetLayers())
    {
        if(layer.name == "walls")
        {
            for(tmx::MapObject& object : layer.objects)
            {
                walls.push_back(new Wall(object.GetAABB().left, object.GetAABB().top, object.GetAABB().width, object.GetAABB().height));
            }
        }
    }
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    tmx::MapLoader ml("assets");
    ml.Load("tilemap.tmx");

    std::vector<Wall*> walls;
    getWallsFromTmx(&ml, walls);

    int windowWidth  = ml.GetMapSize().x;
    int windowHeight = ml.GetMapSize().y;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "GameWindow", sf::Style::Default, settings);

    sf::CircleShape npc(8);
    npc.setOrigin(npc.getRadius(), npc.getRadius());
    npc.setPosition(3*16-8, 3*16-8);
    npc.setFillColor(sf::Color::Blue);

    bool goalSet = false;
    sf::CircleShape goal(8);
    goal.setOrigin(goal.getRadius(), goal.getRadius());
    goal.setFillColor(sf::Color::Red);

    Pathfind pathfind(&ml, &walls, &window);
    std::vector<Point*> waypoints;
    
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(ml);
        pathfind.buildPathGrid();

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = sf::Mouse::getPosition(window).x;
            int y = sf::Mouse::getPosition(window).y;
            goal.setPosition(x, y);
            goalSet = true;

            pathfind.setStart(npc.getPosition().x, npc.getPosition().y);
            pathfind.setGoal(x, y);
            waypoints.clear();
            waypoints = pathfind.run();
        }

        window.draw(npc);

        if(goalSet)
            window.draw(goal);

        Point wp1(npc.getPosition().x, npc.getPosition().y);

        for(Point* point : waypoints)
        {
            DrawTools::drawLine(wp1, *point, sf::Color::Green, &window);
            wp1 = *point;
        }

        window.display();
    }

}
