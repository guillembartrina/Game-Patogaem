
#include "CollisionHandler.hpp"

#include "PhysicEntity.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::BeginContact(b2Contact* contact)
{
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    static_cast<PhysicEntity*>(userDataA)->onCollision(static_cast<PhysicEntity*>(userDataB));
    static_cast<PhysicEntity*>(userDataB)->onCollision(static_cast<PhysicEntity*>(userDataA));
}

void CollisionHandler::EndContact(b2Contact* contact) {}