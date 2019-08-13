
#include "PhysicEntity.hpp"

#include "Utils.hpp"
#include "Utils_Box2D.hpp"

PhysicEntity::PhysicEntity() : Entity()
{
   physicized = false;
   numBodies = 0;

   setCODE(0x8000);
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position) : Entity(play, position)
{
    physicized = false;
    numBodies = 0;

    setCODE(0x8000);
}

PhysicEntity::PhysicEntity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : Entity(play, position, texture, rect)
{
    physicized = false;
    numBodies = 0;

    setCODE(0x8000);
}

PhysicEntity::~PhysicEntity()
{
   if(physicized)
   {
       b2World* world = bodies[0]->GetWorld();

       for(unsigned int i = 0; i < numBodies; i++)
       {
           world->DestroyBody(bodies[i]);
       }

       delete[] bodies;
   }
}

b2Body* PhysicEntity::physicize(b2World& world)
{
    assert(numBodies > 0);

    if(physicized)
    {
        printInfo("Already physicized!");
        return body;
    }

    bodies = new b2Body*[numBodies];

    for(unsigned int i = 0; i < numBodies; i++)
    {
        bodyDefs[i]->bodyDef.position = metrize(tob2Vec2(getPosition()));
        bodies[i] = world.CreateBody(&bodyDefs[i]->bodyDef);

        for(unsigned int j = 0; j < bodyDefs[i]->fixtureDef.size(); j++)
        {
            b2Fixture* tmp = bodies[i]->CreateFixture(&bodyDefs[i]->fixtureDef[j]);
            tmp->SetUserData((void*)j);
        }

        bodies[i]->SetUserData(this);
        bodies[i]->SetActive(false);

        delete bodyDefs[i];
    }

    delete[] bodyDefs;
    physicized = true;

    bodies[0]->SetActive(true);
    body = bodies[0];

    return body;
}

void PhysicEntity::setPosition(const sf::Vector2f& position)
{
    body->SetTransform(tob2Vec2(metrize(position)), body->GetAngle());

    Entity::setPosition(position);
}

void PhysicEntity::setRotation(float angle)
{
    body->SetTransform(body->GetPosition(), -1*radize(angle)); //!!

    Entity::setRotation(angle);
}

void PhysicEntity::update(const sf::Time deltatime)
{
    if(body->IsAwake())
    {
        Entity::setPosition(pixelize(toVector2f(body->GetPosition())));
        if(not body->IsFixedRotation()) Entity::setRotation(anglize(body->GetAngle()));
    }

    Entity::update(deltatime);
}

void PhysicEntity::onCollision(unsigned int fixtureid, PhysicEntity* collided) {}

void PhysicEntity::onDecollision(unsigned int fixtureid, PhysicEntity* collided) {}

//void PhysicEntity::onReduceDurability() {}

/*
CollisionCategory PhysicEntity::getCC() const
{
    return CollisionCategory_ALL_COLLISION;
}
*/

sf::RectangleShape PhysicEntity::getHB(unsigned int num) const
{
    sf::RectangleShape hb;

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

void PhysicEntity::addBody(b2BodyType type, bool fixedrotation)
{
    numBodies++;

    BodyDef** tmp = new BodyDef*[numBodies];

    if(numBodies > 1)
    {
        memcpy(tmp, bodyDefs, sizeof(BodyDef*) * (numBodies - 1));
        delete[] bodyDefs;
    }
    
    bodyDefs = tmp;

    bodyDefs[numBodies - 1] = new BodyDef();
    b2BodyDef& current = bodyDefs[numBodies - 1]->bodyDef;
    current.type = type;
    current.fixedRotation = fixedrotation;
}

void PhysicEntity::addFixture(const b2Shape* shape, CollisionCategory category, float friction, float restitution, float density, bool sensor)
{
    assert(numBodies > 0);

    bodyDefs[numBodies - 1]->fixtureDef.push_back(b2FixtureDef());

    b2FixtureDef& current = bodyDefs[numBodies - 1]->fixtureDef.back();

    current.shape = shape;
    current.friction = friction;
    current.restitution = restitution;
    current.density = density;
    current.filter = getCollisionFilter(category);
    current.isSensor = sensor;
}

void PhysicEntity::setBody(unsigned int num, bool sametransform)
{
    assert(num >= 0 and num < numBodies);

    b2Vec2 pos = body->GetPosition();
    b2Vec2 vel = body->GetLinearVelocity();

    body->SetActive(false);
    body = bodies[num];
    body->SetActive(true);

    if(sametransform)
    {
        body->SetTransform(pos, 0.f);
        body->SetLinearVelocity(vel);
    }
}