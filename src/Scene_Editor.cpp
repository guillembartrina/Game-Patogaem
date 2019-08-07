
#include "Scene_Editor.hpp"

#include <fstream>
#include <string>

#include "Utils.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "json.hpp"

Scene_Editor::Scene_Editor(Core core)
: Scene(core)
{
    phase = false;
    cellsize = sf::Vector2f(core.window->getSize().x / NUMCELLS.x, core.window->getSize().y / NUMCELLS.y);

    //ImGui init
    strcpy(nameBuffer, "name");
    strcpy(backgroundBuffer, "background");

    currentItem = 0;

    loadLevelNames();
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
            //temp
            if(phase and event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2u pos(event.mouseButton.x / cellsize.x, event.mouseButton.y / cellsize.y);
                level.add(Coord(pos.x, pos.y), 1);               
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
        if(ImGui::Button("DELETE")) {}
    }
    else
    {
        ImGui::InputText("Name", nameBuffer, 31);
        ImGui::InputText("Background", backgroundBuffer, 31);
        ImGui::Separator();
        ImGui::LabelText("DATA", "DATA");
        ImGui::Separator();
        if(ImGui::Button("SAVE"))
        {
            level.setName(std::string(nameBuffer));
            level.setBackground(std::string(backgroundBuffer));
            level.serialize();
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
        shape.setFillColor(sf::Color::Black);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(-0.8f);

        for(int i = 0; i < NUMCELLS.x; i++)
        {
            for(int j = 0; j < NUMCELLS.y; j++)
            {
                //temp
                if(not level.get(Coord(i, j)).empty()) shape.setFillColor(sf::Color::Blue);
                else shape.setFillColor(sf::Color::Black);

                shape.setPosition(sf::Vector2f(i*cellsize.x, j*cellsize.y));
                window.draw(shape);
            }
        }
    }
}

void Scene_Editor::pause() {}

void Scene_Editor::resume() {}

void Scene_Editor::loadLevelNames()
{
    std::fstream file("rsc/levels.json", std::ios_base::in);
    std::string page = "", tmp;
    while(getline(file, tmp)) page += tmp;
    file.close();

    json::JSON obj;
    obj = json::Load(page);

    levelNamesSize = obj["Levels"].length();

    if(levelNamesSize != 0)
    {
        levelNames = new char*[levelNamesSize];
 
        for(unsigned int i = 0; i < levelNamesSize; i++)
        {
            std::string tmp = obj["Levels"][i]["name"].ToString();
            levelNames[i] = new char[tmp.length()+1];
            strcpy(levelNames[i], tmp.c_str());
        }
    }
}

/*
//REMAKE
void Scene_Editor::deserializeLevel(int num)
{
    fstream file("rsc/levels.json");
    std::string page = "", tmp;
    while(getline(file, tmp)) page += tmp;
    file.close();

    jute::jValue data = jute::parser::parse(page)["Levels"][num];

    level.name = data["name"].as_string();
    strcpy(nameBuffer, level.name.c_str());
    level.background = data["background"].as_string();
    strcpy(backgroundBuffer, level.background.c_str());

    unsigned int tmpsize = data["items"].size();

    for(int i = 0; i < tmpsize; i++)
    {
        unsigned int rowSize = data["items"][i].size();
        auto it = level.items.insert(std::make_pair(std::make_pair(data["items"][i][0].as_int(), data["items"][i][1].as_int()), std::vector<short>(rowSize-2)));
        for(int j = 2; j < rowSize; j++)
        {
            it.first->second[j-2] = data["items"][i][j].as_int();
        }
    }
}

void Scene_Editor::serializeLevel()
{
    std::string out =
    "{\n\t\"name\": \"" + level.name + "\",\n" + "\t\"background\": \"" + level.background + "\",\n" + "\t\"items\": \n\t[\n";

    bool first = true;
    
    for(auto it = level.items.begin(); it != level.items.end(); it++)
    {
        if(first) first = false;
        else out += ",\n";
        out += "\t\t[" + std::to_string(it->first.first) + "," + std::to_string(it->first.second);
        for(short& s : it->second)
        {
            out += "," + std::to_string(s);
        }
        out += "]";
    }
    
    out += "\n\t]\n}\n";

    std::cerr << out << std::endl;
}
*/