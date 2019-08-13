
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"

#include "Scene_Play.hpp"

class Scene_Play;

class Entity : public sf::Transformable, public sf::Drawable
{
public:

    Entity();
    Entity(Scene_Play* play, const sf::Vector2f& position);
    Entity(Scene_Play* play, const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect);
    ~Entity();

    void animate(unsigned int numFrames, const sf::Time& frameTime);
    void setAnimationFrame(unsigned int frameNum);
    void playAnimation();
    void stopAnimation();

    void setPosition(const sf::Vector2f& position);
    void setOrigin(const sf::Vector2f& origin);
    void setRotation(float angle);

    virtual void update(const sf::Time deltatime);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool operator==(const Entity& e) const;
    bool operator<(const Entity& e) const;

    unsigned int getID() const;
    void setCODE(unsigned short code);
    unsigned short getCODE() const;
    
protected:

    void setSprite(const sf::Texture& texture, const sf::IntRect& rect);

    bool haveSprite;
    sf::Sprite sprite;

private:

    Scene_Play* play;

    unsigned int ID;
    unsigned short CODE; //default: 0

    bool haveAnimation;
    bool playing;
    unsigned int numFrames;
    sf::Time frameTime;

    unsigned int currentFrame;
    sf::Time currentTime;

};

#endif