#ifndef DUCK_HPP
#define DUCK_HPP

#include "PhysicEntity.hpp"

enum MovementSide
{
  MV_RIGHT = 0,
  MV_LEFT
};

enum MovementState
{
  IDLE = 0,
  WALK,
  JUMP,
  FLY,
  DOWN,
  FLOOR
};

class Duck : public PhysicEntity
{
    public:

    Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Duck();

    void update(sf::Time deltatime) override;

    void handleEvents(const sf::Event& event);

    void onCollision(int fixtureid, PhysicEntity* collided) override;

    private:

    std::vector<sf::IntRect> rects;

    MovementState state;
    MovementSide side;

    bool grounded;
    sf::Vector2f vel;

    bool lc, rc;
};

#endif