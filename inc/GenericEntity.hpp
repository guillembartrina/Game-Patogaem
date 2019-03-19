
#ifndef GENERICENTITY_HPP
#define GENERICENTITY_HPP

#include "PhysicEntity.hpp"

class GenericEntity : public PhysicEntity
{
    public:

    GenericEntity(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name, std::pair<unsigned int, sf::Time> animationParams = std::make_pair(0, sf::Time::Zero));
    ~GenericEntity();

    private:

};

#endif