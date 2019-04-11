#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#define DEBUG_ENABLE true

#include <set>

#include "Scene.hpp"

#include "Box2D/Box2D.h"
#include "Box2DUtils.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "Entity.hpp"
#include "CollisionHandler.hpp"

class Entity;

typedef std::map<unsigned int, Entity*> EntityHolder;

class Scene_Play : public Scene
{
public:

    Scene_Play(Core core);
    ~Scene_Play();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

    unsigned int getNextID();

private:

    sf::View view;

    //Play vars
    b2World world;
    CollisionHandler collisionHandler;

    unsigned int ID;
    EntityHolder entities;

    //Functions
    sf::Vector2f cellToPixels(sf::Vector2u cell) const;
    void loadLevel();

    EntityHolder::iterator addEntity(Entity* entity);

    //ImGui vars
    int x, y;
};

#endif