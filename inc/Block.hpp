
#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "PhysicEntity.hpp"

class Block : public PhysicEntity
{
    public:

    Block(Core core, Scene_Play* play, const sf::Vector2f& position, char sides);
    ~Block();

    private:

    static b2EdgeShape edges[4];

};

#endif