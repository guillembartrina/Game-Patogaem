
#include "PhysicEntity.hpp"

PhysicEntity::PhysicEntity() : Entity()
{
    physics = false;
    physicized = false;
    durability = 0;
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position) : Entity(play, position)
{
    physics = false;
    physicized = false;
    durability = 0;
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : Entity(play, position, texture, rect)
{
    physics = false;
    physicized = false;
    durability = 0;
}

PhysicEntity::~PhysicEntity()
{
    delete shape;
}

b2Body* PhysicEntity::physicize(b2World& world)
{
    if(physics)
    {
        bodyDef.position.Set(metrize(getPosition().x), metrize(getPosition().y));
        body = world.CreateBody(&bodyDef);
        fixtureDef.shape = shape;
        fixtureDef.filter = getCollisionFilter(category);
        body->CreateFixture(&fixtureDef);
        body->SetUserData(this);
        physicized = true;

        return body;
    }

    return nullptr;
}

void PhysicEntity::setPosition(const sf::Vector2f& position)
{
    if(physicized)
    {
        body->SetTransform(tob2Vec2(metrize(position)), body->GetAngle());
    }

    Entity::setPosition(position);
}

void PhysicEntity::setRotation(float angle)
{
    if(physicized)
    {
        body->SetTransform(body->GetPosition(), -1*radize(angle)); //!!
    }

    Entity::setRotation(angle);
}

void PhysicEntity::update(const sf::Time deltatime)
{
    if(physicized)
    {
        Entity::setPosition(pixelize(toVector2f(body->GetPosition())));
        if(not body->IsFixedRotation()) Entity::setRotation(anglize(body->GetAngle()));
    }

    Entity::update(deltatime);
}

void PhysicEntity::onCollision(PhysicEntity* collided) {}

void PhysicEntity::onReduceDurability() {}

void PhysicEntity::setPhysics(b2BodyType type, b2Shape* shape, CollisionCategory category, float friction, float density, float restitution)
{
    bodyDef.type = type;
    this->shape = shape;
    this->category = category;
    fixtureDef.friction = friction;
    fixtureDef.density = density;
    fixtureDef.restitution = restitution;
    physics = true;
}