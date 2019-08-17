
#include "CollisionHandler.hpp"

#include "PhysicEntity.hpp"
#include "Utils.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold *oldManifold)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    void* userDataA = fixtureA->GetBody()->GetUserData();
    void* userDataB = fixtureB->GetBody()->GetUserData();

    if(not fixtureA->IsSensor() and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> precolliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onPrecollision((unsigned int)(intptr_t)fixtureA->GetUserData(), static_cast<PhysicEntity*>(userDataB), contact);
    }

    if(not fixtureA->IsSensor() and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> precolliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onPrecollision((unsigned int)(intptr_t)fixtureB->GetUserData(), static_cast<PhysicEntity*>(userDataA), contact);
    }
}

void CollisionHandler::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    void* userDataA = fixtureA->GetBody()->GetUserData();
    void* userDataB = fixtureB->GetBody()->GetUserData();

    if(fixtureA->IsSensor() and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> colliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onCollision((unsigned int)(intptr_t)fixtureA->GetUserData(), static_cast<PhysicEntity*>(userDataB));
    }

    if(fixtureB->IsSensor() and not fixtureA->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> colliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onCollision((unsigned int)(intptr_t)fixtureB->GetUserData(), static_cast<PhysicEntity*>(userDataA));
    }
}

void CollisionHandler::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    void* userDataA = fixtureA->GetBody()->GetUserData();
    void* userDataB = fixtureB->GetBody()->GetUserData();

    if(fixtureA->IsSensor() and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> decolliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onDecollision((unsigned int)(intptr_t)fixtureA->GetUserData(), static_cast<PhysicEntity*>(userDataB));
    }

    if(fixtureB->IsSensor() and not fixtureA->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> decolliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onDecollision((unsigned int)(intptr_t)fixtureB->GetUserData(), static_cast<PhysicEntity*>(userDataA));
    }
}