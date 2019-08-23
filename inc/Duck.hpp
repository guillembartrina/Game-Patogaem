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

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void onPrecollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc, b2Contact* contact) override;
  void onCollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc) override;
  void onDecollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc) override;

  void changeState(MovementState newstate);

  MovementState getState() const;

  private:

  MovementState state;
  MovementSide side;

  Holdable* holdable;
  std::set<Holdable*> holdables;

  unsigned int groundings, headings;

  bool pL, pR, pU, pD; //pressed arrows
  bool go, gz, hz;

  sf::Sound quack;

  static const float dynamicsValues[5];
  static const float dynamicsCoefs[5];
  static const sf::Vector2f holdableOffset[5];
};

#endif