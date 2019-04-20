
#include "Duck.hpp"

Duck::Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    state = MovementState::IDLE;
    side = MovementSide::MV_RIGHT;

    rects.push_back(sf::IntRect(ZEROVECTOR_I, sf::Vector2i(64, 128)));
    rects.push_back(sf::IntRect(sf::Vector2i(64, 0), sf::Vector2i(64, 128)));

    grounded = true;

    setSprite(core.resources->Texture("duck"), rects[state*2+side]);

    setPhysics(b2BodyType::b2_dynamicBody, createRectangle(b2Vec2(62, 126)), CollisionCategory::DUCK, 0.2f, 1.f, 0.f);
    bodyDef.fixedRotation = true;

    //physicize(world);

    //isSensor and all collisionas already activated
    /*
    b2Shape* s = createRectangle(b2Vec2(62, 24), b2Vec2(0, 51));
    fixtureDef.shape = s;
    body->CreateFixture(&fixtureDef);

    s = createRectangle(b2Vec2(62, 24), b2Vec2(0, -51));
    fixtureDef.shape = s;
    body->CreateFixture(&fixtureDef);
    */
}

Duck::~Duck() {}

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
                    if(grounded) body->ApplyLinearImpulse(b2Vec2(0, -24), body->GetPosition(), true);
                }
                    break;
                case sf::Keyboard::Down:
                {
                    printInfo("down");
                }
                    break;
                case sf::Keyboard::Left:
                {
                    if(state != MovementState::FLOOR)
                    {
                        side = MovementSide::MV_LEFT;
                    }
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(state != MovementState::FLOOR)
                    {
                        side = MovementSide::MV_RIGHT;
                    }
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
    sprite.setTextureRect(rects[state*2+side]);
}

void Duck::onCollision(PhysicEntity* collided)
{
    printInfo("DUCK COLLISION --> " << collided->getID());
}