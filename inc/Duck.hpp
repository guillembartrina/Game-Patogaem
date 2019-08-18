#ifndef DUCK_HPP
#define DUCK_HPP

#include "PhysicEntity.hpp"
#include "Holdable.hpp"

#include "SFML/Audio/Sound.hpp"

enum MovementSide
{
  MovementSide_RIGHT = 0,
  MovementSide_LEFT
};

enum MovementState
{
  MovementState_STANDING = 0,
  MovementState_JUMPING,
  MovementState_FLYING,
  MovementState_DOWNING,
  MovementState_FLOORING
};

class Duck : public PhysicEntity
{
  public:

  Duck(Core core, Scene_Play* play, const sf::Vector2f& position);
  ~Duck();

  void update(sf::Time deltatime) override;

  void handleEvents(const sf::Event& event);

  void onCollision(unsigned int fixtureid, PhysicEntity* collided) override;
  void onDecollision(unsigned int fixtureid, PhysicEntity* collided) override;

  void changeState(MovementState newstate);

  MovementState getState() const;

  private:

  MovementState state;
  MovementSide side;

  Holdable* holdable;

  unsigned int groundings, headings;

  bool pL, pR, pU, pD; //pressed arrows
  bool go, gz, hz;

  sf::Sound quack;

  static const float stateValues[5];
  static const float stateCoefs[5]; 
};

#endif