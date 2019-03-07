#ifndef SCENE__MENU_HPP
#define SCENE__MENU_HPP

#include "Scene.hpp"

#include "imgui.h"
#include "imguiSFML.h"

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

    sf::Text t_title;

    sf::Text t_play;
    sf::Text t_exit;

    int currentOption;

};

#endif