
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "PhysicEntity.hpp"

class Object : public PhysicEntity
{
    public:

    Object();
    Object(Scene_Play* play, const sf::Vector2f& position);
    Object(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect);
    ~Object();

    virtual void update(const sf::Time deltatime) override;
    
    virtual void action();
    void startTimer(sf::Time time);
    virtual void onTimerTrigger();

    protected:

    private:

    bool active;
    sf::Time currentTime;
    sf::Time triggerTime;

};

#endif