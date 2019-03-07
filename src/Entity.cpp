
#include "Entity.hpp"

#include <iostream>

Entity::Entity()
{
    haveSprite = false;
    setPosition(ZEROVECTOR_F);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position) : play(play)
{
    haveSprite = false;
    setPosition(position);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : play(play)
{
    haveSprite = true;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    setOrigin(sf::Vector2f(rect.width, rect.height) * 0.5f);
    setPosition(position);
}

Entity::~Entity() {}

void Entity::setPosition(const sf::Vector2f& position)
{
    Transformable::setPosition(position);
    if(haveSprite) sprite.setPosition(position);
}

void Entity::setOrigin(const sf::Vector2f& origin)
{
    Transformable::setOrigin(origin);
    if(haveSprite) sprite.setOrigin(origin);
}

void Entity::setRotation(float angle)
{
    Transformable::setRotation(angle);
    if(haveSprite) sprite.setRotation(angle);
}

void Entity::update() {}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(haveSprite) target.draw(sprite, states);
}

bool Entity::operator==(const Entity& e) const
{
    return (e.ID == ID);
}

void Entity::setSprite(const sf::Texture& texture)
{
    sprite.setTexture(texture);
    haveSprite = true;
    setOrigin(sf::Vector2f(texture.getSize())*0.5f);
}