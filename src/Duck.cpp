
#include "Duck.hpp"

Duck::Duck(Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    setSprite(core.resources->Texture("duck_full"), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(64, 128)));
    setPhysics(b2BodyType::b2_dynamicBody, createRectangle(b2Vec2(62, 126)), CollisionCategory::DUCK, 0.2f, 0.2f, 0.f);
    bodyDef.fixedRotation = true;

    state = MovementState::IDLE;
    side = MovementSide::MV_RIGHT;
}

void Duck::update(const sf::Time deltatime)
{
    PhysicEntity::update(deltatime);
}

void Duck::handleEvents(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Up:
                {
                    body->ApplyLinearImpulse(b2Vec2(0, -30), body->GetPosition(), true);
                }
                    break;
                case sf::Keyboard::Down:
                {
                    printInfo("down");
                }
                    break;
                case sf::Keyboard::Left:
                {
                    //if(state != MovementState::FLOOR) side = MovementSide::MV_LEFT;
                    body->ApplyForce(b2Vec2(-400, 0), body->GetPosition(), true);
                }
                    break;
                case sf::Keyboard::Right:
                {
                    //if(state != MovementState::FLOOR) side = MovementSide::MV_RIGHT;
                    body->ApplyForce(b2Vec2(400, 0), body->GetPosition(), true);
                }
                    break;
                default:
                    break;
            }
        }
            break;
        case sf::Event::KeyReleased:
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Up:
                {
                }
                    break;
                case sf::Keyboard::Down:
                {
                }
                    break;
                case sf::Keyboard::Left:
                {
                }
                    break;
                case sf::Keyboard::Right:
                {
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

Duck::~Duck() {}