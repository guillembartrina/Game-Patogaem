#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#define NUM_OPTIONS 3

class Scene_Menu : public Scene
{
public:

    Scene_Menu(Core core);
    ~Scene_Menu();

    void init();

    void handleEvents(const sf::Event& event);

    void update(const sf::Time deltatime);

    void draw(sf::RenderWindow& window) const;

    void pause();

    void resume();

private:

    sf::Text title;
    sf::Text options[NUM_OPTIONS];

    static const std::string texts[NUM_OPTIONS];

    int currentOption;

    bool updateRequest;
};

#endif