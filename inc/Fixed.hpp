
#ifndef FIXED_HPP
#define FIXED_HPP

#include "PhysicEntity.hpp"

class Fixed : public PhysicEntity
{
    public:

    Fixed(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Fixed();

    private:

};

#endif