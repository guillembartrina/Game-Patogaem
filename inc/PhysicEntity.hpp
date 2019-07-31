
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
    CollisionCategory_DUCK = 0b1 << 0,
    CollisionCategory_STATIC_FOREGROUND = 0b1 << 1,
    CollisionCategory_DYNAMIC_FOREGROUND = 0b1 << 2,
    CollisionCategory_STATIC_BACKGROUND = 0b1 << 3,
    CollisionCategory_DYNAMIC_BACKGROUND = 0b1 << 4,
    CollisionCategory_NO_COLLISION = 0b1 << 5,
    CollisionCategory_ALL_COLLISION = 0b1 << 6
};

#define FOREGROUND_MASK 0b00000110
#define BACKGROUND_MASK 0b00011000

static b2Filter getCollisionFilter(CollisionCategory cc)
{
    b2Filter filter;
    filter.categoryBits = cc;

    switch(cc)
    {
        case CollisionCategory_DUCK:
        filter.maskBits = CollisionCategory_STATIC_FOREGROUND | CollisionCategory_DYNAMIC_FOREGROUND | CollisionCategory_ALL_COLLISION;
            break;
        case CollisionCategory_STATIC_FOREGROUND:
        filter.maskBits = CollisionCategory_DUCK | CollisionCategory_STATIC_FOREGROUND | CollisionCategory_DYNAMIC_FOREGROUND | CollisionCategory_STATIC_BACKGROUND | CollisionCategory_DYNAMIC_BACKGROUND | CollisionCategory_ALL_COLLISION;
            break;
        case CollisionCategory_DYNAMIC_FOREGROUND:
        filter.maskBits = CollisionCategory_DUCK | CollisionCategory_STATIC_FOREGROUND | CollisionCategory_DYNAMIC_FOREGROUND | CollisionCategory_STATIC_BACKGROUND | CollisionCategory_ALL_COLLISION;
            break;
        case CollisionCategory_STATIC_BACKGROUND:
        filter.maskBits = CollisionCategory_STATIC_FOREGROUND | CollisionCategory_DYNAMIC_FOREGROUND | CollisionCategory_STATIC_BACKGROUND | CollisionCategory_DYNAMIC_BACKGROUND | CollisionCategory_ALL_COLLISION;
            break;
        case CollisionCategory_DYNAMIC_BACKGROUND:
        filter.maskBits = CollisionCategory_STATIC_FOREGROUND | CollisionCategory_STATIC_BACKGROUND | CollisionCategory_ALL_COLLISION;
            break;
        case CollisionCategory_NO_COLLISION:
        filter.maskBits = 0x0000;
            break;
        case CollisionCategory_ALL_COLLISION:
        filter.maskBits = 0xFFFF;
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

    //virtual void onPrecollision(int fixtureid, PhysicEntity* collided);
    virtual void onCollision(int fixtureid, PhysicEntity* collided);
    virtual void onDecollision(int fixtureid, PhysicEntity* collided);
    
    //virtual void onReduceDurability();

    CollisionCategory getCC() const;

    sf::RectangleShape getHB(unsigned int num = 0) const;

protected:

    b2Body* body;

    void setBody(b2BodyType type, bool rotation = false);
    void addFixture(b2Shape* shape, CollisionCategory category, float friction, float restitution, float density, bool sensor = false);

    void resetPhysics();

private:

    bool physicized;

    bool physics;
    b2BodyDef bodyDef;
    std::vector<b2FixtureDef> fixtureDef;

    CollisionCategory cc;
    
    unsigned int durability;
};

#endif