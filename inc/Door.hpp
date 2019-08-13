
#ifndef DOOR_HPP
#define DOOR_HPP

#include "Object.hpp"

class Door : public Object
{
    public:

    Door(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Door();

    private:
    
};

#endif