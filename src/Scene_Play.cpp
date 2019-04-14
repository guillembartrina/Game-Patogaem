
#include "Scene_Play.hpp"

#include "PhysicEntity.hpp"
#include "GenericEntity.hpp"
#include "GenericPhysicEntity.hpp"
#include "TestPE.hpp"
#include "Duck.hpp"

Scene_Play::Scene_Play(Core core)
: Scene(core)
, world(b2Vec2(0.f, 60.f))
{
    ID = 0;
    world.SetContactListener(&collisionHandler);

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(core.window->getSize()));

    duck = new Duck(core, this, sf::Vector2f(300, 700));
    static_cast<Duck*>(duck)->physicize(world);

    //TEST
    level.name = "testlevel";
    level.background = "background1";

    //IMGUI
    x = y = 0;
    m = 1;
}

Scene_Play::~Scene_Play()
{
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
                default:
                    break;
            }
        }
            break;
        case sf::Event::MouseButtonPressed:
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                EntityHolder::iterator it = addEntity(new TestPE(core, this, cellToPixels(sf::Vector2u(core.window->mapPixelToCoords(sf::Mouse::getPosition(*core.window)) * (1.f/64.f))), "crate", b2BodyType::b2_dynamicBody, CollisionCategory(m)));

                PhysicEntity* pe = dynamic_cast<PhysicEntity*>(it->second);
                if(pe != nullptr)
                {
                    pe->physicize(world);
                }
            } 
        }
            break;
        default:
            break;
    }

    static_cast<Duck*>(duck)->handleEvents(event);
}

void Scene_Play::update(const sf::Time deltatime)
{
    core.window->setView(view);
    background.setPosition(view.getCenter() - view.getSize() / 2.f);

    world.Step(deltatime.asSeconds(), 4, 4);

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
    if(DEBUG_ENABLE)
    {
        ImGui::Begin("DEBUG");
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
        ImGui::RadioButton("S_F", &m, CollisionCategory::STATIC_FOREGROUND);
        ImGui::RadioButton("S_B", &m, CollisionCategory::STATIC_BACKGROUND);
        ImGui::RadioButton("D_F", &m, CollisionCategory::DYNAMIC_FOREGROUND);
        ImGui::RadioButton("D_B", &m, CollisionCategory::DYNAMIC_BACKGROUND);
        ImGui::EndChild();
        ImGui::End();
    }
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    window.draw(background);

    window.draw(*duck);

    for(EntityHolder::const_iterator it = entities.begin(); it != entities.end(); it++)
    {
        window.draw(*it->second);
    }
}

void Scene_Play::pause() {}

void Scene_Play::resume() {}

sf::Vector2f Scene_Play::cellToPixels(sf::Vector2u cell) const
{
    if(cell.x >= NUMCELLS.x or cell.y >= NUMCELLS.y)
    {
        std::cerr << "WARNING: Outisde map request position" << std::endl;
        return sf::Vector2f(CELLSIZE) * 0.5f;
    }
    return sf::Vector2f(cell.x * CELLSIZE.x, cell.y * CELLSIZE.y) + (sf::Vector2f(CELLSIZE) * 0.5f);
}

void Scene_Play::loadLevel(Level* level)
{
    entities.clear();

    background.setTexture(&core.resources->Texture(level->background));

    for(int i = 0; i < NUMCELLS.x; i++)
    {
        for(int j = 0; j < NUMCELLS.y; j++)
        {
            if(map[j][i] == 1)
            {
                EntityHolder::iterator it = addEntity(new GenericPhysicEntity(core, this, cellToPixels(sf::Vector2u(i, j)), "ts_castle_w_c"));

                PhysicEntity* pe = dynamic_cast<PhysicEntity*>(it->second);
                if(pe != nullptr)
                {
                    pe->physicize(world);
                }
            }
        }
    }
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
