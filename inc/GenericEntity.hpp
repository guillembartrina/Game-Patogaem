
#ifndef GENERICENTITY_HPP
#define GENERICENTITY_HPP

#include "PhysicEntity.hpp"

class GenericEntity : public Entity
{
    public:

    GenericEntity(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name);
    ~GenericEntity();

    private:

};

#endif