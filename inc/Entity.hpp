
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"

#include "Scene_Play.hpp"
#include "Utils.hpp"

class Scene_Play;

class Entity : public sf::Transformable, public sf::Drawable
{
public:

    Entity();
    Entity(Scene_Play* play, const sf::Vector2f& position);
    Entity(Scene_Play* play,const sf::Vector2f& position, const sf::Texture& texture, const sf::IntRect& rect);
    ~Entity();

    void setPosition(const sf::Vector2f& position);
    void setOrigin(const sf::Vector2f& origin);
    void setRotation(float angle);

    virtual void update();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    bool operator==(const Entity& e) const;
    
protected:

    void setSprite(const sf::Texture& texture);

    bool haveSprite;
    sf::Sprite sprite;

private:

    Scene_Play* play;

    unsigned int ID;

};

#endif