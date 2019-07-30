#ifndef DUCK_HPP
#define DUCK_HPP

#include "PhysicEntity.hpp"

enum MovementSide
{
  Side_RIGHT = 0,
  Side_LEFT
};

enum MovementState
{
  State_IDLE = 0,
  State_WALK,
  State_JUMP,
  State_FLY,
  State_DOWN,
  State_FLOOR
};

class Duck : public PhysicEntity
{
    public:

    Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Duck();

    void update(sf::Time deltatime) override;

    void handleEvents(const sf::Event& event);

    void onCollision(int fixtureid, PhysicEntity* collided) override;
    void onDecollision(int fixtureid, PhysicEntity* collided) override;

    void changeState(MovementState newstate);

    private:

    MovementState state;
    MovementSide side;

    int groundings;
    sf::Vector2f vel;

    bool lc, rc;

    b2Body* bodies[3];
};

#endif