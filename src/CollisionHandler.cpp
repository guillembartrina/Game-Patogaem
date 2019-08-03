
#include "CollisionHandler.hpp"

#include "PhysicEntity.hpp"
#include "Utils.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

/*
void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold *oldManifold)
{
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    if(contact->GetFixtureA()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> colliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onPrecollision((int)(intptr_t)contact->GetFixtureA()->GetUserData(), static_cast<PhysicEntity*>(userDataB));
    }

    if(contact->GetFixtureB()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> colliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onPrecollision((int)(intptr_t)contact->GetFixtureB()->GetUserData(), static_cast<PhysicEntity*>(userDataA));
    }
}
*/

void CollisionHandler::BeginContact(b2Contact* contact)
{
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    if(contact->GetFixtureA()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> colliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onCollision((int)(intptr_t)contact->GetFixtureA()->GetUserData(), static_cast<PhysicEntity*>(userDataB));
    }

    if(contact->GetFixtureB()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> colliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onCollision((int)(intptr_t)contact->GetFixtureB()->GetUserData(), static_cast<PhysicEntity*>(userDataA));
    }
}

void CollisionHandler::EndContact(b2Contact* contact)
{
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    if(contact->GetFixtureA()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataA)->getID() << "> colliding with <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onDecollision((int)(intptr_t)contact->GetFixtureA()->GetUserData(), static_cast<PhysicEntity*>(userDataB));
    }

    if(contact->GetFixtureB()->IsSensor())
    {
        printInfo("Sensor of <" << static_cast<Entity*>(userDataB)->getID() << "> colliding with <" << static_cast<Entity*>(userDataA)->getID() << ">");

        static_cast<PhysicEntity*>(userDataB)->onDecollision((int)(intptr_t)contact->GetFixtureB()->GetUserData(), static_cast<PhysicEntity*>(userDataA));
    }
}