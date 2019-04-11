
#include "CollisionHandler.hpp"

#include "Object.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::BeginContact(b2Contact* contact)
{
    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    assert(userDataA != nullptr and userDataB != nullptr and "Collision: at least one body has no USERDATA");

    static_cast<PhysicEntity*>(userDataA)->onCollision(static_cast<PhysicEntity*>(userDataB));
    static_cast<PhysicEntity*>(userDataB)->onCollision(static_cast<PhysicEntity*>(userDataA));
}

void CollisionHandler::EndContact(b2Contact* contact) {}