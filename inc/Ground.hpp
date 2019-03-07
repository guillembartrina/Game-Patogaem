
#ifndef GROUND_HPP
#define GROUND_HPP

#include "PhysicEntity.hpp"

class Ground : public PhysicEntity
{
    public:

    Ground(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Ground();

    private:

};

#endif