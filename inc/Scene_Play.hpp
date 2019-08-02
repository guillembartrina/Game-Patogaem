#ifndef SCENE__PLAY_HPP
#define SCENE__PLAY_HPP

#define DEBUG_MENU_ENABLE true

#include <queue>

#include "Scene.hpp"

#include "Box2D/Box2D.h"
#include "Utils_Box2D.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "Entity.hpp"
#include "CollisionHandler.hpp"
#include "Level.hpp"


class Entity;

typedef std::map<unsigned int, Entity*> EntityHolder;

class Scene_Play : public Scene
{
public:

    Scene_Play(Core core, std::string levelname);
    ~Scene_Play();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

    unsigned int getNextID();

    EntityHolder::iterator addEntity(Entity* entity);
    void deleteEntity(unsigned int id);

private:

    sf::View view;

    //Play vars
    b2World world;
    CollisionHandler collisionHandler;

    sf::RectangleShape background;

    Entity* duck;

    unsigned int ID;
    EntityHolder entities;

    std::queue<unsigned int> toDelete;

    //Functions
    sf::Vector2f cellToPixels(sf::Vector2u cell) const;
    void loadLevel(Level* level);

    //ImGui vars

    bool duckHBs;
    bool sceneHBs;

    int x, y;
    int m;

    void imgui();

    //Test

    Level level;
};

#endif