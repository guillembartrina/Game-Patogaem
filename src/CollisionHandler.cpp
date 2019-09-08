
#include "CollisionHandler.hpp"

#include "PhysicEntity.hpp"
#include "Utils.hpp"

CollisionHandler::CollisionHandler() {}

CollisionHandler::~CollisionHandler() {}

void CollisionHandler::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    unsigned int fixtureUserDataA = (uintptr_t)fixtureA->GetUserData();
    unsigned int fixtureUserDataB = (uintptr_t)fixtureB->GetUserData();

    PhysicEntity* userDataA = static_cast<PhysicEntity*>(fixtureA->GetBody()->GetUserData());
    PhysicEntity* userDataB = static_cast<PhysicEntity*>(fixtureB->GetBody()->GetUserData());

    b2WorldManifold wm;
    contact->GetWorldManifold(&wm);

    if((fixtureA->IsSensor() or isTarjet(userDataA, IS_DUCK)) and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << userDataA->getID() << "> colliding with <" << userDataB->getID() << ">");

        userDataA->onCollision(fixtureUserDataA & 0xFFFF, userDataB, fixtureUserDataB >> 16, contact);
    }

    if((fixtureB->IsSensor() or isTarjet(userDataB, IS_DUCK)) and not fixtureA->IsSensor())
    {
        printInfo("Sensor of <" << userDataB->getID() << "> colliding with <" << userDataA->getID() << ">");

        userDataB->onCollision(fixtureUserDataB & 0xFFFF, userDataA, fixtureUserDataA >> 16, contact);
    }
}

void CollisionHandler::PreSolve(b2Contact* contact, const b2Manifold *oldManifold)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    unsigned int fixtureUserDataA = (uintptr_t)fixtureA->GetUserData();
    unsigned int fixtureUserDataB = (uintptr_t)fixtureB->GetUserData();

    PhysicEntity* userDataA = static_cast<PhysicEntity*>(fixtureA->GetBody()->GetUserData());
    PhysicEntity* userDataB = static_cast<PhysicEntity*>(fixtureB->GetBody()->GetUserData());

    printInfo("Fixture of <" << userDataA->getID() << "> presolving with <" << userDataB->getID() << ">");
    
    userDataA->onPrecollision(fixtureUserDataA & 0xFFFF, userDataB, fixtureUserDataB >> 16, contact);
    userDataB->onPrecollision(fixtureUserDataB & 0xFFFF, userDataA, fixtureUserDataA >> 16, contact);

}

void CollisionHandler::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    unsigned int fixtureUserDataA = (uintptr_t)fixtureA->GetUserData();
    unsigned int fixtureUserDataB = (uintptr_t)fixtureB->GetUserData();

    PhysicEntity* userDataA = static_cast<PhysicEntity*>(fixtureA->GetBody()->GetUserData());
    PhysicEntity* userDataB = static_cast<PhysicEntity*>(fixtureB->GetBody()->GetUserData());

    if(fixtureA->IsSensor() and not fixtureB->IsSensor())
    {
        printInfo("Sensor of <" << userDataA->getID() << "> decolliding with <" << userDataB->getID() << ">");

        userDataA->onDecollision(fixtureUserDataA & 0xFFFF, userDataB, fixtureUserDataB >> 16);
    }

    if(fixtureB->IsSensor() and not fixtureA->IsSensor())
    {
        printInfo("Sensor of <" << userDataB->getID() << "> decolliding with <" << userDataA->getID() << ">");

        userDataB->onDecollision(fixtureUserDataB & 0xFFFF, userDataA, fixtureUserDataA >> 16);
    }
}