
#include "PhysicEntity.hpp"

#include "Utils.hpp"
#include "Utils_Box2D.hpp"

PhysicEntity::PhysicEntity() : Entity()
{
    physics = false;
    physicized = false;
    cc = CollisionCategory_NO_COLLISION;
    durability = 0;
    bodyDef.type = b2BodyType::b2_staticBody;

    CODE = 0x8000;
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position) : Entity(play, position)
{
    physics = false;
    physicized = false;
    cc = CollisionCategory_NO_COLLISION;
    durability = 0;
    bodyDef.type = b2BodyType::b2_staticBody;

    CODE = 0x8000;
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : Entity(play, position, texture, rect)
{
    physics = false;
    physicized = false;
    cc = CollisionCategory_NO_COLLISION;
    durability = 0;
    bodyDef.type = b2BodyType::b2_staticBody;

    CODE = 0x8000;
}

PhysicEntity::~PhysicEntity()
{
    if(physicized)
    {
        body->GetWorld()->DestroyBody(body);
    }

    if(physics)
    {
        for(int i = 0; i < fixtureDef.size(); i++)
        {
            delete fixtureDef[i].shape;
        }
    }
}

b2Body* PhysicEntity::physicize(b2World& world)
{
    if(physics)
    {
        bodyDef.position = metrize(tob2Vec2(getPosition()));
        body = world.CreateBody(&bodyDef);

        for(int i = 0; i < fixtureDef.size(); i++)
        {
            b2Fixture* tmp = body->CreateFixture(&fixtureDef[i]);
            tmp->SetUserData((void*)i);
        }

        if(fixtureDef.size() == 1) //If only one fixture-> simple entity, needs a collision sensor
        {
            fixtureDef[0].filter = getCollisionFilter(CollisionCategory_ALL_COLLISION);
            fixtureDef[0].isSensor = true;
            body->CreateFixture(&fixtureDef[0]);
        }

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
    if(physicized and body->IsAwake())
    {
        Entity::setPosition(pixelize(toVector2f(body->GetPosition())));
        if(not body->IsFixedRotation()) Entity::setRotation(anglize(body->GetAngle()));
    }

    Entity::update(deltatime);
}

void PhysicEntity::onCollision(int fixtureid, PhysicEntity* collided) {}

void PhysicEntity::onDecollision(int fixtureid, PhysicEntity* collided) {}

//void PhysicEntity::onReduceDurability() {}

CollisionCategory PhysicEntity::getCC() const
{
    return cc;
}

sf::RectangleShape PhysicEntity::getHB(unsigned int num) const
{
    sf::RectangleShape hb = sf::RectangleShape();

    hb.setFillColor(sf::Color::Transparent);
    hb.setOutlineColor(sf::Color::Magenta);
    hb.setOutlineThickness(-2.f);

    if(physicized)
    {
        b2Fixture* f = body->GetFixtureList();
        while(num > 0 and f != nullptr) { f = f->GetNext(); num--; }

        if(f == nullptr) return hb;

        sf::Vector2f hs = toVector2f(pixelize(f->GetAABB(0).GetExtents()));
        
        hb.setSize(hs * 2.f);
        hb.setOrigin(hs);
        hb.setPosition(toVector2f(pixelize(f->GetAABB(0).GetCenter())));
    }

    return hb;
}

void PhysicEntity::setBody(b2BodyType type, bool rotation)
{
    bodyDef.type = type;
    bodyDef.fixedRotation = rotation;
}

void PhysicEntity::addFixture(const b2Shape* shape, CollisionCategory category, float friction, float restitution, float density, bool sensor)
{
    fixtureDef.push_back(b2FixtureDef());

    b2FixtureDef& current = fixtureDef.back();

    current.shape = shape;
    current.friction = friction;
    current.restitution = restitution;
    current.density = density;
    current.filter = getCollisionFilter(category);
    current.isSensor = sensor;

    cc = category;

    physics = true;
}

void PhysicEntity::resetPhysics()
{
    for(int i = 0; i < fixtureDef.size(); i++)
    {
        delete fixtureDef[i].shape;
    }

    fixtureDef.clear();

    physics = false;
    physicized = false;
}