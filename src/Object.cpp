
#include "Object.hpp"

Object::Object() {}

Object::Object(Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position) {}

Object::Object(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : PhysicEntity(play, position, texture, rect) {}

Object::~Object() {}

void Object::onCollision() {}

void Object::onDuckCollision() {}