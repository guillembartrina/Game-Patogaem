#ifndef TEST__PE_HPP
#define TEST__PE_HPP

#include "PhysicEntity.hpp"

#include "Utils.hpp"

class TestPE : public PhysicEntity
{
    public:

    TestPE(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name, b2BodyType type, CollisionCategory category, sf::Vector2f size = CELLSIZE);
    ~TestPE();

    virtual void onCollision(int fixtureid, PhysicEntity* collided) override;

    private:

};

#endif