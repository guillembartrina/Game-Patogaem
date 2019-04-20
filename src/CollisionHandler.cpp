
#include "CollisionHandler.hpp"

#include "PhysicEntity.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::BeginContact(b2Contact* contact)
{
    if(contact->GetFixtureA()->IsSensor() and contact->GetFixtureB()->IsSensor())
    {
        void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
        void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

        printInfo("Colliding <" << static_cast<Entity*>(userDataA)->getID() << "> and <" << static_cast<Entity*>(userDataB)->getID() << ">");

        static_cast<PhysicEntity*>(userDataA)->onCollision(static_cast<PhysicEntity*>(userDataB));
        static_cast<PhysicEntity*>(userDataB)->onCollision(static_cast<PhysicEntity*>(userDataA));
    }
}

void CollisionHandler::EndContact(b2Contact* contact) {}