
#include "Entity.hpp"

#include <iostream>

Entity::Entity()
{
    haveSprite = false;
    haveAnimation = false;
    setPosition(ZEROVECTOR_F);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position) : play(play)
{
    haveSprite = false;
    haveAnimation = false;
    setPosition(position);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture) : play(play)
{
    haveSprite = true;
    haveAnimation = false;
    sprite.setTexture(texture);
    int textureSize = texture.getSize().y;
    sprite.setOrigin(sf::Vector2f(textureSize, textureSize) * 0.5f);
    setPosition(position);
}

Entity::~Entity() {}

void Entity::animate(unsigned int numFrames, const sf::Time& frameTime)
{
    if(haveSprite)
    {
        this->numFrames = numFrames;
        this->frameTime = frameTime;

        currentFrame = 0;
        currentTime = sf::Time::Zero;

        int frameSize = sprite.getTexture()->getSize().y;
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(frameSize, frameSize)));

        haveAnimation = true;
        playing = true;
    }
}

void Entity::setAnimationFrame(unsigned int frameNum)
{
    if(haveAnimation and frameNum < numFrames)
    {
        currentFrame = frameNum;
    }
}

void Entity::playAnimation()
{
    if(haveAnimation) playing = true;
}

void Entity::stopAnimation()
{
    if(haveAnimation) playing = false;
}

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

void Entity::update(const sf::Time deltatime)
{
    if(haveAnimation and playing)
    {
        currentTime += deltatime;

        if(currentTime >= frameTime)
        {
            currentFrame++;
            currentFrame %= numFrames;

            int frameSize = sprite.getTexture()->getSize().y;
            sprite.setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * frameSize, 0), sf::Vector2i(frameSize, frameSize)));

            currentTime -= frameTime;
        }
    }
}

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
    haveSprite = true;
    sprite.setTexture(texture);
    int textureSize = texture.getSize().y;
    sprite.setOrigin(sf::Vector2f(textureSize, textureSize) * 0.5f);
    setPosition(getPosition());
}