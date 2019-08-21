
#include "Holdable.hpp"

Holdable::Holdable() {}

Holdable::Holdable(Scene_Play* play, const sf::Vector2f& position) : Object(play, position) {}

Holdable::Holdable(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : Object(play, position, texture, rect) {}

Holdable::~Holdable() {}

Holdable* Holdable::get()
{
    body->SetActive(false);
    return this;
}

Entity* Holdable::unget()
{
    body->SetActive(true);
    return this;
}