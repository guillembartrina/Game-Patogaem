
#include "Scene_Play.hpp"

#include "PhysicEntity.hpp"
#include "Ground.hpp"

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

    //delete duck;
}

void Scene_Play::init()
{
    view.setSize(sf::Vector2f(core.window->getSize()));
    view.setCenter(sf::Vector2f(CELLSIZE.x * NUMCELLS.x, CELLSIZE.y * NUMCELLS.y) * 0.5f);
    core.window->setView(view);

    for(int i = 0; i < NUMCELLS.x; i++)
    {
        for(int j = 0; j < NUMCELLS.y; j++)
        {
            if(map[j][i] == 1) entities.push_back(new Ground(core, this, cellToPixels(sf::Vector2u(i, j))));
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

    //duck = new Duck(core, cellToPixels(sf::Vector2u(1, 1)));
    //duckBody = duck->physicize(world);
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
                    /*
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
                    */
                case sf::Keyboard::W:
                {
                    //if(duckBody->GetLinearVelocity().y <= 0.1f and duckBody->GetLinearVelocity().y >= -0.1f)
                    //{
                    //    duckBody->ApplyLinearImpulse(b2Vec2(0.f, -140.f), duckBody->GetPosition(), true);
                    ///}
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        //duckBody->ApplyForceToCenter(b2Vec2(100.f, 0.f), true);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        //duckBody->ApplyForceToCenter(b2Vec2(-100.f, 0.f), true);
    }

    //sf::Vector2f viewDuck = duck->getPosition() - view.getCenter();

    //if(viewDuck.x >= view.getSize().x * 0.3f) view.move(sf::Vector2f(viewDuck.x - view.getSize().x * 0.3f, 0.f));
    //else if(viewDuck.x <= view.getSize().x * -0.3f) view.move(sf::Vector2f(viewDuck.x - view.getSize().x * -0.3f, 0.f));
    //if(viewDuck.y >= view.getSize().y * 0.3f) view.move(sf::Vector2f(0.f, viewDuck.y - view.getSize().y * 0.3f));
    //else if(viewDuck.y <= view.getSize().y * -0.3f) view.move(sf::Vector2f(0.f, viewDuck.y - view.getSize().y * -0.3f));

    ImGui::Begin("DEBUG");
    //ImGui::Text("Vector view to duck: (%f, %f)", duck->getPosition().x - view.getCenter().x, duck->getPosition().y - view.getCenter().y);
    ImGui::PushItemWidth(70.f);
    ImGui::InputInt("X", &x, 1, 100);
    ImGui::InputInt("Y", &y, 1, 100);
    if(ImGui::Button("CREATE"))
    {
        if(x >= 0 and x < NUMCELLS.x and y >= 0 and y < NUMCELLS.y)
        {
            entities.push_back(new Ground(core, this, cellToPixels(sf::Vector2u(x, y))));
            PhysicEntity* pe = dynamic_cast<PhysicEntity*>(entities.back());
            if(pe != nullptr)
            {
                pe->physicize(world);
            }
        }
    }
    ImGui::End();

    world.Step(deltatime.asSeconds(), 4, 4);

    for(int i = 0; i < entities.size(); i++)
    {
        entities[i]->update();
    }

    //duck->update();
}

void Scene_Play::draw(sf::RenderWindow& window) const
{
    for(int i = 0; i < entities.size(); i++)
    {
        window.draw(*entities[i]);
    }

    //window.draw(*duck);
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
