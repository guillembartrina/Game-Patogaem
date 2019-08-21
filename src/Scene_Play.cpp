
#include "Scene_Play.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "EntityCreator.hpp"
#include "PhysicEntity.hpp"
#include "TestPE.hpp"
#include "Duck.hpp"

bool EntityComp::operator()(const Entity* e1, const Entity* e2) const
{
    return (*e1 < *e2);
}

Scene_Play::Scene_Play(Core core, std::string levelname)
: Scene(core)
, world(b2Vec2(0.f, 40.f))
{
    ID = 0;
    world.SetContactListener(&collisionHandler);

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(core.window->getSize()));

    duck = new Duck(core, this, sf::Vector2f(300, 900));
    static_cast<PhysicEntity*>(duck)->physicize(world);

    //TEST
    level.setName("testlevel");
    level.setBackground("background1");

    //IMGUI
    duckHBs = false;
    sceneHBs = false;
    x = y = 0;
    m = 2;
}

Scene_Play::~Scene_Play()
{
    delete duck;

    for(EntityHolder::iterator it = entities.cbegin(); it != entities.cend(); it++)
    {
        delete *it;
    }
}

void Scene_Play::init()
{
    view.setSize(sf::Vector2f(core.window->getSize()));
    view.setCenter(sf::Vector2f(CELLSIZE.x * NUMCELLS.x, CELLSIZE.y * NUMCELLS.y) * 0.5f);
    core.window->setView(view);

    loadLevel(&level);
}

void Scene_Play::handleEvents(const sf::Event& event)
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
                case sf::Keyboard::W:
                {
                    view.move(sf::Vector2f(0.f, -32.f));
                }
                    break;
                case sf::Keyboard::S:
                {
                    view.move(sf::Vector2f(0.f, 32.f));
                }
                    break;
                case sf::Keyboard::A:
                {
                    view.move(sf::Vector2f(-32.f, 0.f));
                }
                    break;
                case sf::Keyboard::D:
                {
                    view.move(sf::Vector2f(32.f, 0.f));
                }
                    break;
                case sf::Keyboard::C:
                {
                    //addEntity(new TestPE(core, this, cellToPixels(sf::Vector2u(core.window->mapPixelToCoords(sf::Mouse::getPosition(*core.window)) * (1.f/64.f))), "crate", b2BodyType::b2_dynamicBody, CollisionCategory(m)));    
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseButtonPressed:
        {
        }
            break;
        default:
            break;
    }

    static_cast<Duck*>(duck)->handleEvents(event);
}

void Scene_Play::update(const sf::Time deltatime)
{
    //TEMPORAL
    sf::Vector2f duckPosToView = duck->getPosition() - view.getCenter();
    sf::Vector2f movingBounds = sf::Vector2f(core.window->getSize()) * 0.2f;;

    if(duckPosToView.x > movingBounds.x) view.move(sf::Vector2f(duckPosToView.x - movingBounds.x, 0.f));
    else if(duckPosToView.x < -movingBounds.x) view.move(sf::Vector2f(duckPosToView.x + movingBounds.x, 0.f));
    if(duckPosToView.y > movingBounds.y) view.move(sf::Vector2f(0.f, duckPosToView.y - movingBounds.y));
    else if(duckPosToView.y < -movingBounds.y) view.move(sf::Vector2f(0.f, duckPosToView.y + movingBounds.y));

    //--------------

    core.window->setView(view);
    background.setPosition(view.getCenter() - view.getSize() / 2.f);

    world.Step(deltatime.asSeconds(), 4, 4);

    duck->update(deltatime);

    for(EntityHolder::iterator it = entities.begin(); it != entities.end(); it++)
    {
        (*it)->update(deltatime);
    }

    while(not toDestroy.empty())
    {
        Entity* e = toDestroy.front();
        delete e;
        entities.erase(e);
        toDestroy.pop();
    }

    //IMGUI
    imgui();
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(background);

    for(EntityHolder::const_iterator it = entities.cbegin(); it != entities.cend(); it++)
    {
        window.draw(**it);
        if(sceneHBs)
        {
            for(int i = 0; i < 4; i++) window.draw(static_cast<PhysicEntity*>(*it)->getHB(i));
        }
    }

    window.draw(*duck);

    if(duckHBs)
    {
        window.draw(static_cast<Duck*>(duck)->getHB(0));
        window.draw(static_cast<Duck*>(duck)->getHB(1));
        window.draw(static_cast<Duck*>(duck)->getHB(2));
    }
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

sf::Vector2f Scene_Play::cellToPixels(sf::Vector2u cell) const
{
    assert(cell.x <= NUMCELLS.x and cell.y <= NUMCELLS.y);
    return sf::Vector2f(cell.x * CELLSIZE.x, cell.y * CELLSIZE.y) + (sf::Vector2f(CELLSIZE) * 0.5f);
}

unsigned int Scene_Play::getNextID()
{
    ID++;
    return ID;
}

EntityHolder::iterator Scene_Play::createEntity(Entity* entity)
{
    std::pair<EntityHolder::iterator, bool> it = entities.insert(entity);
    if(isTarjet(*it.first, IS_PHYSICENTITY)) static_cast<PhysicEntity*>(*it.first)->physicize(world);

    return it.first;
}

void Scene_Play::destroyEntity(Entity* entity)
{
    toDestroy.push(entity);
}

void Scene_Play::addEntity(Entity* entity)
{
    entities.insert(entity);
}

void Scene_Play::deleteEntity(Entity* entity)
{
    entities.erase(entity);
}

void Scene_Play::loadLevel(Level* level)
{
    entities.clear();

    background.setTexture(&core.resources->Texture(level->getBackground()));

    for(int i = 0; i < NUMCELLS.x; i++)
    {
        for(int j = 0; j < NUMCELLS.y; j++)
        {
            if(testmap[j][i] != 0)
            {
                createEntity(getEntitybyCode(EntityCode(testmap[j][i]), core, this, cellToPixels(sf::Vector2u(i, j))));
            }
        }
    }
}

void Scene_Play::imgui()
{
    if(DEBUG_MENU_ENABLE)
    {
        bool open = true;
        ImGui::Begin("DEBUG", &open, ImVec2(200, 400));

        ImGui::Separator();
        ImGui::Text("State: ");
        ImGui::SameLine();
        ImGui::Text(MovementState_String[static_cast<Duck*>(duck)->getState()].c_str());
        ImGui::Separator();

        if(ImGui::CollapsingHeader("HBs"))
        {
            ImGui::Checkbox("Duck HBs", &duckHBs);
            ImGui::Checkbox("Scene HBs", &sceneHBs);
        }

        /*
        ImGui::PushItemWidth(70.f);
        ImGui::InputInt("X", &x, 0, 100);
        ImGui::InputInt("Y", &y, 0, 100);
        if(ImGui::Button("CREATE"))
        {
            if(x >= 0 and x < NUMCELLS.x and y >= 0 and y < NUMCELLS.y)
            {
                EntityHolder::iterator it = addEntity(new TestPE(core, this, cellToPixels(sf::Vector2u(x, y)), "expl", b2BodyType::b2_dynamicBody, CollisionCategory(m)));

                PhysicEntity* pe = dynamic_cast<PhysicEntity*>(it->second);
                if(pe != nullptr)
                {
                    pe->physicize(world);
                }
            }
        }
        ImGui::BeginChild("CollCat", ImVec2(240, 140), true);
        ImGui::Text("CollisionCategory");
        ImGui::RadioButton("S_F", &m, CollisionCategory_STATIC_FOREGROUND);
        ImGui::RadioButton("S_B", &m, CollisionCategory_STATIC_BACKGROUND);
        ImGui::RadioButton("D_F", &m, CollisionCategory_DYNAMIC_FOREGROUND);
        ImGui::RadioButton("D_B", &m, CollisionCategory_DYNAMIC_BACKGROUND);
        ImGui::EndChild();
        */

        ImGui::End();
    }
}
