
#include "Scene_Menu.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "Scene_Play.hpp"
#include "Scene_Editor.hpp"

const std::string Scene_Menu::texts[NUM_OPTIONS] = {"PLAY", "EDITOR", "EXIT"};

Scene_Menu::Scene_Menu(Core core)
: Scene(core)
{}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    title.setFont(core.resources->Font("font"));
    title.setString("TITLE");
    title.setCharacterSize(90);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(86, 100);

    for(int i = 0; i < NUM_OPTIONS; i++)
    {
        options[i].setFont(core.resources->Font("font"));
        options[i].setString(texts[i]);
        options[i].setCharacterSize(60);
        options[i].setFillColor(sf::Color::White);
        options[i].setPosition(160, 300+i*100);
    }

    currentOption = 0;

    updateRequest = true;
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
                    if(currentOption < NUM_OPTIONS-1) currentOption++;

                    updateRequest = true;
                }
                    break;
                case sf::Keyboard::Up:
                {
                    if(currentOption > 0) currentOption--;
                
                    updateRequest = true;
                }
                    break;
                case sf::Keyboard::Return:
                {
                    switch(currentOption)
                    {
                        case 0:
                            core.sceneHandler->addScene(std::unique_ptr<Scene>(new Scene_Play(core, "XXX")));
                            break;
                        case 1:
                            core.sceneHandler->addScene(std::unique_ptr<Scene>(new Scene_Editor(core)));
                            break;
                        case 2:
                            core.sceneHandler->popScene();
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
            break;
        default:
            break;
    }

    if(updateRequest)
    {
        for(int i = 0; i < NUM_OPTIONS; i++)
        {
            if(i == currentOption) options[i].setFillColor(sf::Color::Green);
            else options[i].setFillColor(sf::Color::White);
        }
        updateRequest = false;
    }
}

void Scene_Menu::update(const sf::Time deltatime) {}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    window.draw(title);
    
    for(int i = 0; i < NUM_OPTIONS; i++) window.draw(options[i]);
}

void Scene_Menu::pause() {}

void Scene_Menu::resume() {}
