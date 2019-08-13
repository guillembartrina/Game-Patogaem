
#include "Object.hpp"

#include "Utils.hpp"

Object::Object() {}

Object::Object(Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position) {}

Object::Object(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect) : PhysicEntity(play, position, texture, rect) {}

Object::~Object() {}

void Object::update(const sf::Time deltatime)
{
    if(active)
    {
        currentTime += deltatime;
        if(currentTime >= triggerTime)
        {
            active = false;
            onTimerTrigger();
        }
    }

    PhysicEntity::update(deltatime);
}

void Object::action() {}

void Object::startTimer(sf::Time time)
{
    if(not active)
    {
        currentTime = sf::Time::Zero;
        triggerTime = time;
        active = true;
    }
    else
    {
        printInfo(getID() <<  " : TRYING TO ACTIVATE TIMER WHEN ALTERADY ACTIVE");
    }
}

void Object::onTimerTrigger() {}