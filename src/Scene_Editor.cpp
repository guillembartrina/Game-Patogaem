
#include "Scene_Editor.hpp"

#include <fstream>
#include <string>

#include "Utils.hpp"

#include "imgui.h"
#include "imguiSFML.h"

Scene_Editor::Scene_Editor(Core core)
: Scene(core)
{
    cellsize = sf::Vector2f(core.window->getSize().x / NUMCELLS.x, core.window->getSize().y / NUMCELLS.y);

    //ImGui init
    currentItem = -1;
    currentEntity = 0;

    levelNames = Level::getLevelNames(levelNamesSize);
    phase = false;
}

Scene_Editor::~Scene_Editor()
{
    for(int i = 0; i < levelNamesSize; i++)
    {
        delete levelNames[i];
    }
    delete levelNames;
}

void Scene_Editor::init() {}

void Scene_Editor::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Escape:
                {
                    core.sceneHandler->popScene();
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseButtonPressed:
        {
            if(phase and not ImGui::IsMouseHoveringAnyWindow())
            {
                Coord pos(event.mouseButton.x / cellsize.x, event.mouseButton.y / cellsize.y);

                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    level.add(pos, Entity_Code[currentEntity]);
                }
                else if(event.mouseButton.button == sf::Mouse::Right)
                {
                    if(not level.isEmpty(pos)) level.del(pos, *level.get(pos).begin());
                }
            }
            if(phase and event.mouseButton.button == sf::Mouse::Left and not ImGui::IsMouseHoveringAnyWindow())
            {
                sf::Vector2u pos(event.mouseButton.x / cellsize.x, event.mouseButton.y / cellsize.y);
                level.add(Coord(pos.x, pos.y), Entity_Code[currentEntity]);          
            }
        }
            break;
        default:
            break;
    }
}

void Scene_Editor::update(const sf::Time deltatime)
{
    bool open = true;
    ImGui::Begin("MENU", &open, ImVec2(200, 400));

    if(not phase)
    {
        if(ImGui::Button("NEW"))
        { 
            strcpy(nameBuffer, "name");
            strcpy(backgroundBuffer, "background");

            phase = true; 
        }
        ImGui::ListBox("Levels", &currentItem, levelNames, levelNamesSize, 10);
        if(ImGui::Button("LOAD"))
        {
            if(currentItem != -1)
            {
                level.deserialize(currentItem);

                strcpy(nameBuffer, level.getName().c_str());
                strcpy(backgroundBuffer, level.getBackground().c_str());

                phase = true;
            }
        }
        ImGui::SameLine(100.f);
        if(ImGui::Button("DELETE"))
        {
            if(currentItem != -1)
            {
                Level::deleteLevel(currentItem);
                reloadLevelNames();
            }
        }
        ImGui::Separator();
        if(ImGui::Button("BACK"))
        {
            core.sceneHandler->popScene();
        }
    }
    else
    {
        ImGui::InputText("Name", nameBuffer, 31);
        ImGui::InputText("Background", backgroundBuffer, 31);
        ImGui::Separator();
        ImGui::ListBox("Entities", &currentEntity, Entity_String, IM_ARRAYSIZE(Entity_String), 10);
        ImGui::Separator();
        if(ImGui::Button("SAVE"))
        {
            level.setName(std::string(nameBuffer));
            level.setBackground(std::string(backgroundBuffer));
            level.serialize();
        }

        ImGui::SameLine(60.f);
        if(ImGui::Button("BACK"))
        {
            level = Level();
            currentItem = 0;
            reloadLevelNames();

            phase = false;
        }
    }

    ImGui::End();
}

void Scene_Editor::draw(sf::RenderWindow& window) const
{
    if(phase)
    {
        sf::RectangleShape shape;
        shape.setSize(cellsize);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(-0.8f);

        for(int i = 0; i < NUMCELLS.x; i++)
        {
            for(int j = 0; j < NUMCELLS.y; j++)
            {
                if(not level.isEmpty(Coord(i, j)))
                {
                    shape.setFillColor(sf::Color::White);
                    Entity* e = getEntitybyCode(EntityCode(*level.get(Coord(i, j)).begin()), core, nullptr, ZEROVECTOR_F);
                    const sf::Sprite* s = e->getSprite();
                    if(s != nullptr)
                    {
                        shape.setTexture(s->getTexture());
                        shape.setTextureRect(s->getTextureRect());
                    }
                    else
                    {
                        shape.setTexture(&core.resources->Texture("default"), true);
                    }
                    delete e;
                }
                else
                {
                    shape.setFillColor(sf::Color::Black);
                    shape.setTexture(nullptr);
                }

                shape.setPosition(sf::Vector2f(i*cellsize.x, j*cellsize.y));
                window.draw(shape);
            }
        }
    }
}

void Scene_Editor::pause() {}

void Scene_Editor::resume() {}

void Scene_Editor::reloadLevelNames()
{
    for(int i = 0; i < levelNamesSize; i++)
    {
        delete levelNames[i];
    }
    delete levelNames;

    currentItem = -1;

    levelNames = Level::getLevelNames(levelNamesSize);
}