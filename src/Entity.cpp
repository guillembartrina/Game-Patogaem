
#include "Entity.hpp"

#include <iostream>

Entity::Entity()
{
    ID = 0;
    haveSprite = false;
    haveAnimation = false;
    setPosition(ZEROVECTOR_F);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position) : play(play)
{
    ID = play->getNextID();
    haveSprite = false;
    haveAnimation = false;
    setPosition(position);
}

Entity::Entity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : play(play)
{
    ID = play->getNextID();
    haveSprite = true;
    haveAnimation = false;
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
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

        //sf::IntRect rect = sprite.getTextureRect();
        //sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(rect.width, rect.height)));

        haveAnimation = true;
        playing = true;
    }
}

void Entity::setAnimationFrame(unsigned int frameNum)
{
    if(haveAnimation and frameNum < numFrames)
    {
        currentFrame = frameNum;

        sf::IntRect rect = sprite.getTextureRect();
        sprite.setTextureRect(sf::IntRect(sf::Vector2i(currentFrame * rect.width, 0), sf::Vector2i(rect.width, rect.height)));

        currentTime = sf::Time::Zero;
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

            sf::IntRect rect = sprite.getTextureRect();
            rect.left = rect.width * currentFrame;
            sprite.setTextureRect(rect);

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
    return (ID == e.ID);
}

bool Entity::operator<(const Entity& e) const
{
    return (ID < e.ID);
}

unsigned int Entity::getID() const
{
    return ID;
}

void Entity::setSprite(const sf::Texture& texture, const sf::IntRect& rect)
{
    sprite.setTexture(texture);
    sprite.setTextureRect(rect);
    sprite.setOrigin(sf::Vector2f(rect.width, rect.height) * 0.5f);
    setPosition(getPosition());
    haveSprite = true;
}