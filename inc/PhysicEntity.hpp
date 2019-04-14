
#ifndef PHYSIC_ENTITY_HPP
#define PHYSIC_ENTITY_HPP

#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Dynamics/b2World.h"

#include "Box2DUtils.hpp"
#include "Entity.hpp"

#include "Utils.hpp"

enum CollisionCategory
{
    DUCK = 0b1 << 0,
    STATIC_FOREGROUND = 0b1 << 1,
    DYNAMIC_FOREGROUND = 0b1 << 2,
    STATIC_BACKGROUND = 0b1 << 3,
    DYNAMIC_BACKGROUND = 0b1 << 4,
    NO_COLLISION = 0b1 << 5
};

static b2Filter getCollisionFilter(CollisionCategory cc)
{
    b2Filter filter;
    filter.categoryBits = cc;

    switch(cc)
    {
        case DUCK:
        filter.maskBits = STATIC_FOREGROUND | DYNAMIC_FOREGROUND;
            break;
        case STATIC_FOREGROUND:
        filter.maskBits = DUCK | STATIC_FOREGROUND | DYNAMIC_FOREGROUND | STATIC_BACKGROUND | DYNAMIC_BACKGROUND;
            break;
        case DYNAMIC_FOREGROUND:
        filter.maskBits = DUCK | STATIC_FOREGROUND | DYNAMIC_FOREGROUND | STATIC_BACKGROUND;
            break;
        case STATIC_BACKGROUND:
        filter.maskBits = STATIC_FOREGROUND | DYNAMIC_FOREGROUND | STATIC_BACKGROUND | DYNAMIC_BACKGROUND;
            break;
        case DYNAMIC_BACKGROUND:
        filter.maskBits = STATIC_FOREGROUND | STATIC_BACKGROUND;
            break;
        case NO_COLLISION:
        filter.maskBits = 0x0000;
            break;
    }

    return filter;
}

class PhysicEntity : public Entity
{
public:

    PhysicEntity();
    PhysicEntity(Scene_Play* play, const sf::Vector2f& position);
    PhysicEntity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect);
    ~PhysicEntity();
    
    void setPosition(const sf::Vector2f& position);
    void setRotation(float angle);

    virtual void update(const sf::Time deltatime) override;

    b2Body* physicize(b2World& world);

    virtual void onCollision(PhysicEntity* collided);
    virtual void onReduceDurability();

protected:

    void setPhysics(b2BodyType type, b2Shape* shape, CollisionCategory category, float friction, float density, float restitution);

    bool physics; //must set as true when physics are defined
    b2BodyDef bodyDef;
    b2Shape* shape;
    CollisionCategory category;
    b2FixtureDef fixtureDef;

    b2Body* body;

private:

    bool physicized;
    
    unsigned int durability;
};

#endif