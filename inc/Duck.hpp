#ifndef DUCK_HPP
#define DUCK_HPP

#include "PhysicEntity.hpp"

enum MovementSide
{
  MV_RIGHT = 1,
  MV_LEFT
};

enum MovementState
{
  IDLE = 1,
  WALK,
  JUMP,
  FLY,
  DOWN,
  FLOOR
};

class Duck : public PhysicEntity
{
    public:

    Duck(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Duck();

    void update(sf::Time deltatime) override;

    void handleEvents(const sf::Event& event);

    private:

    MovementState state;
    MovementSide side;
};

#endif