
#include "Object.hpp"

Object::Object()
{
    durability = 1;
}

Object::Object(Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    durability = 1;
}

Object::Object(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : PhysicEntity(play, position, texture, rect) 
{
    durability = 1;
}

Object::~Object() {}

void Object::onReduceDurability() {}