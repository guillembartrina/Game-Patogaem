
#include "Scene_Play.hpp"

#include "PhysicEntity.hpp"
#include "Fixed.hpp"

Scene_Play::Scene_Play(Core core)
: Scene(core)
, world(b2Vec2(0.f, 60.f))
{}

Scene_Play::~Scene_Play()
{
    for(int i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
}

void Scene_Play::init()
{
    view.setSize(sf::Vector2f(core.window->getSize()));
    view.setCenter(sf::Vector2f(CELLSIZE.x * NUMCELLS.x, CELLSIZE.y * NUMCELLS.y) * 0.5f);
    core.window->setView(view);

    loadLevel();
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
                case sf::Keyboard::Up:
                {
                    view.move(sf::Vector2f(0.f, -10.f));
                }
                    break;
                case sf::Keyboard::Down:
                {
                    view.move(sf::Vector2f(0.f, 10.f));
                }
                    break;
                case sf::Keyboard::Left:
                {
                    view.move(sf::Vector2f(-10.f, 0.f));
                }
                    break;
                case sf::Keyboard::Right:
                {
                    view.move(sf::Vector2f(10.f, 0.f));
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

void Scene_Play::update(const sf::Time deltatime)
{
    core.window->setView(view);

    world.Step(deltatime.asSeconds(), 4, 4);

    for(int i = 0; i < entities.size(); i++)
    {
        entities[i]->update();
    }

    if(DEBUG_ENABLE)
    {
        ImGui::Begin("DEBUG");
        ImGui::PushItemWidth(70.f);
        ImGui::InputInt("X", &x, 1, 100);
        ImGui::InputInt("Y", &y, 1, 100);
        if(ImGui::Button("CREATE"))
        {
            if(x >= 0 and x < NUMCELLS.x and y >= 0 and y < NUMCELLS.y)
            {
                entities.push_back(new Fixed(core, this, cellToPixels(sf::Vector2u(x, y))));
                PhysicEntity* pe = dynamic_cast<PhysicEntity*>(entities.back());
                if(pe != nullptr)
                {
                    pe->physicize(world);
                }
            }
        }
        ImGui::End();
    }
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    for(int i = 0; i < entities.size(); i++)
    {
        window.draw(*entities[i]);
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

void Scene_Play::loadLevel()
{
    entities.clear();

    for(int i = 0; i < NUMCELLS.x; i++)
    {
        for(int j = 0; j < NUMCELLS.y; j++)
        {
            if(map[j][i] == 1) entities.push_back(new Fixed(core, this, cellToPixels(sf::Vector2u(i, j))));
        }
    }

    for(int i = 0; i < entities.size(); i++)
    {
        PhysicEntity* pe = dynamic_cast<PhysicEntity*>(entities[i]);
        if(pe != nullptr)
        {
            pe->physicize(world);
        }
    }
}
