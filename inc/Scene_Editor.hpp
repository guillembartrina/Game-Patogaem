#ifndef SCENE__EDITOR_HPP
#define SCENE__EDITOR_HPP

#include "Scene.hpp"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "Level.hpp"

class Scene_Editor : public Scene
{
public:

    Scene_Editor(Core core);
    ~Scene_Editor();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

    void reloadLevelNames();

private:

    //Logic
    bool phase;
    sf::Vector2f cellsize;

    //Data
    unsigned int levelNamesSize;
    char** levelNames;
    
    Level level;

    //ImGui
    char nameBuffer[32];
    char backgroundBuffer[32];
    int currentItem;
    int currentEntity;
};

#endif