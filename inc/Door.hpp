
#ifndef DOOR_HPP
#define DOOR_HPP

#include "Object.hpp"

class Door : public Object
{
    public:

    Door(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Door();

    void onPrecollision(unsigned int fixtureid, PhysicEntity* collided, b2Contact* contact) override;
    void onCollision(unsigned int fixtureid, PhysicEntity* collided) override;
    void onDecollision(unsigned int fixtureid, PhysicEntity* collided) override;

    virtual void action() override;

    private:
    
    virtual void onTimerTrigger() override;

    static const sf::Time closeTime;

    int nl, nr;

};

#endif