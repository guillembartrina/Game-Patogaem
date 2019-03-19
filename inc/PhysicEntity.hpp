
#ifndef PHYSIC_ENTITY_HPP
#define PHYSIC_ENTITY_HPP

#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Dynamics/b2World.h"

#include "Box2DUtils.hpp"
#include "Entity.hpp"

class PhysicEntity : public Entity
{
public:

    PhysicEntity();
    PhysicEntity(Scene_Play* play, const sf::Vector2f& position);
    PhysicEntity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture);
    ~PhysicEntity();
    
    void setPosition(const sf::Vector2f& position);
    void setRotation(float angle);

    virtual void update(const sf::Time deltatime) override;

    virtual void onReduceDurability();
    virtual void onDestroy(); 

    b2Body* physicize(b2World& world);

protected:

    void setPhysics(b2BodyType type, b2Shape* shape, float friction, float density, float restitution);

    bool physics; //set as true when physics defined
    b2BodyDef bodyDef;
    b2Shape* shape;
    b2FixtureDef fixtureDef;

private:

    bool physicized;
    b2Body* body;

    int durability;
};

#endif