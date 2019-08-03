
#include "Scene_Play.hpp"

#include "imgui.h"
#include "imguiSFML.h"

#include "PhysicEntity.hpp"
#include "GenericEntity.hpp"
#include "GenericPhysicEntity.hpp"
#include "TestPE.hpp"
#include "Block.hpp"
#include "Duck.hpp"

Scene_Play::Scene_Play(Core core, std::string levelname)
: Scene(core)
, world(b2Vec2(0.f, 40.f))
{
    ID = 0;
    world.SetContactListener(&collisionHandler);

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(core.window->getSize()));

    duck = new Duck(core, this, sf::Vector2f(300, 700), world);

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

    for(EntityHolder::iterator it = entities.begin(); it != entities.end(); it++)
    {
        delete it->second;
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
                    EntityHolder::iterator it = addEntity(new TestPE(core, this, cellToPixels(sf::Vector2u(core.window->mapPixelToCoords(sf::Mouse::getPosition(*core.window)) * (1.f/64.f))), "crate", b2BodyType::b2_dynamicBody, CollisionCategory(m)));

                    static_cast<PhysicEntity*>(it->second)->physicize(world);       
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
    if(duckPosToView.x < -movingBounds.x) view.move(sf::Vector2f(duckPosToView.x + movingBounds.x, 0.f));
    if(duckPosToView.y > movingBounds.y) view.move(sf::Vector2f(0.f, duckPosToView.y - movingBounds.y));
    if(duckPosToView.y < -movingBounds.y) view.move(sf::Vector2f(0.f, duckPosToView.y + movingBounds.y));

    //--------------

    core.window->setView(view);
    background.setPosition(view.getCenter() - view.getSize() / 2.f);

    world.Step(deltatime.asSeconds(), 2, 2);

    duck->update(deltatime);

    for(EntityHolder::iterator it = entities.begin(); it != entities.end(); it++)
    {
        it->second->update(deltatime);
    }

    while(not toDelete.empty())
    {
        entities.erase(toDelete.front());
        toDelete.pop();
    }

    //IMGUI
    imgui();
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(background);

    window.draw(*duck);

    if(duckHBs)
    {
        window.draw(static_cast<Duck*>(duck)->getHB(0));
        window.draw(static_cast<Duck*>(duck)->getHB(1));
        window.draw(static_cast<Duck*>(duck)->getHB(2));
        window.draw(static_cast<Duck*>(duck)->getHB(3));
        window.draw(static_cast<Duck*>(duck)->getHB(4));
        window.draw(static_cast<Duck*>(duck)->getHB(5));
        window.draw(static_cast<Duck*>(duck)->getHB(6));
    }

    for(EntityHolder::const_iterator it = entities.begin(); it != entities.end(); it++)
    {
        window.draw(*it->second);
        if(sceneHBs)
        {
            for(int i = 0; i < 4; i++) window.draw(static_cast<PhysicEntity*>(it->second)->getHB(i));
        }
    }
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

sf::Vector2f Scene_Play::cellToPixels(sf::Vector2u cell) const
{
    if(cell.x >= NUMCELLS.x or cell.y >= NUMCELLS.y)
    {
        std::cerr << "WARNING: Outisde testmap request position" << std::endl;
        return sf::Vector2f(CELLSIZE) * 0.5f;
    }
    return sf::Vector2f(cell.x * CELLSIZE.x, cell.y * CELLSIZE.y) + (sf::Vector2f(CELLSIZE) * 0.5f);
}

unsigned int Scene_Play::getNextID()
{
    ID++;
    return ID;
}

EntityHolder::iterator Scene_Play::addEntity(Entity* entity)
{
    std::pair<EntityHolder::iterator, bool> it = entities.insert(std::make_pair(entity->getID(), entity));
    return it.first;
}

void Scene_Play::deleteEntity(unsigned int id)
{
    toDelete.push(id);
}

void Scene_Play::loadLevel(Level* level)
{
    entities.clear();

    sf::Vector2u sides[4] = { sf::Vector2u(0, -1), sf::Vector2u(1, 0), sf::Vector2u(0, 1), sf::Vector2u(-1, 0) };

    background.setTexture(&core.resources->Texture(level->getBackground()));

    for(int i = 0; i < NUMCELLS.x; i++)
    {
        for(int j = 0; j < NUMCELLS.y; j++)
        {
            if(testmap[j][i] > 0 and testmap[j][i] <= 200)
            {
                //EntityHolder::iterator it = addEntity(new GenericPhysicEntity(core, this, cellToPixels(sf::Vector2u(i, j)), "ts_castle_w_c"));
                char sides = 0x0F;
                if(i-1 >= 0 and testmap[j][i-1] > 0 and testmap[j][i-1] <= 200) sides = sides & 0x0E;
                if(i+1 < NUMCELLS.x and testmap[j][i+1] > 0 and testmap[j][i+1] <= 200) sides = sides & 0x0B;
                if(j-1 >= 0 and testmap[j-1][i] > 0 and testmap[j-1][i] <= 200) sides = sides & 0x07;
                if(j+1 < NUMCELLS.y and testmap[j+1][i] > 0 and testmap[j+1][i] <= 200) sides = sides & 0x0D;

                EntityHolder::iterator it = addEntity(new Block(core, this, cellToPixels(sf::Vector2u(i, j)), sides));

                static_cast<PhysicEntity*>(it->second)->physicize(world);
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

        if(ImGui::CollapsingHeader("HBs"))
        {
            ImGui::Checkbox("Duck HBs", &duckHBs);
            ImGui::Checkbox("Scene HBs", &sceneHBs);
        }

        ImGui::Text(MovementState_String[static_cast<Duck*>(duck)->getState()].c_str());

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
        //ImGui::ShowDemoWindow();
    }
}
