#ifndef _TMXTOOLS_HPP
#define _TMXTOOLS_HPP

#include <tmx/MapLoader.h>
#include "structs.hpp"

class TmxTools
{
public:
    static inline void getWallsFromTmx(tmx::MapLoader* ml, std::vector<Wall*>& walls)
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

};

#endif
