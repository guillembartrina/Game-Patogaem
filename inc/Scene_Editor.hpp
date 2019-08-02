#ifndef SCENE__EDITOR_HPP
#define SCENE__EDITOR_HPP

#include "Scene.hpp"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "jute.hpp"

#include "Utils.hpp"
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

    void loadLevelNames();
    
    void deserializeLevel(int num);
    void serializeLevel();

private:

    bool phase;

    //Data
    unsigned int size;
    char** levelNames;

    //Info
    Level level;

    //ImGui
    char nameBuffer[32];
    char backgroundBuffer[32];
    int currentItem;

    //Drawing
    sf::Vector2f cellsize;
};

#endif