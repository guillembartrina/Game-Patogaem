
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "PhysicEntity.hpp"

class Object : public PhysicEntity
{
    public:

    Object();
    Object(Scene_Play* play, const sf::Vector2f& position);
    Object(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect);
    ~Object();

    virtual void onCollision();
    virtual void onDuckCollision();

    protected:

    private:

};

#endif