
#include "Scene_Menu.hpp"

#include "Scene_Play.hpp"

Scene_Menu::Scene_Menu(Core core)
: Scene(core)
{}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    t_title.setFont(core.resources->Font("font"));
    t_title.setString("TITLE");
    t_title.setCharacterSize(90);
    t_title.setFillColor(sf::Color::Yellow);
    t_title.setPosition(86, 100);

    t_play.setFont(core.resources->Font("font"));
    t_play.setString("PLAY");
    t_play.setCharacterSize(60);
    t_play.setFillColor(sf::Color::White);
    t_play.setPosition(160, 300);

    t_exit.setFont(core.resources->Font("font"));
    t_exit.setString("EXIT");
    t_exit.setCharacterSize(60);
    t_exit.setFillColor(sf::Color::White);
    t_exit.setPosition(160, 400);

    currentOption = 0;
}

void Scene_Menu::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Down:
                {
                    if(currentOption == 0) ++currentOption;
                }
                    break;
                case sf::Keyboard::Up:
                {
                    if(currentOption == 1) --currentOption;
                }
                    break;
                case sf::Keyboard::Return:
                {
                    if(currentOption == 0)
                    {
                        core.sceneHandler->addScene(std::unique_ptr<Scene>(new Scene_Play(core)));
                    }
                    else if(currentOption == 1)
                    {
                        core.sceneHandler->popScene();
                    }
                }
                    break;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
}

void Scene_Menu::update(const sf::Time deltatime)
{
    t_play.setFillColor(sf::Color::White);
    t_exit.setFillColor(sf::Color::White);

    if(currentOption == 0)
    {
        t_play.setFillColor(sf::Color::Green);
    }
    else if(currentOption == 1)
    {
        t_exit.setFillColor(sf::Color::Green);
    }
}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    window.draw(t_title);
    window.draw(t_play);
    window.draw(t_exit);
}

void Scene_Menu::pause() {}

void Scene_Menu::resume() {}
