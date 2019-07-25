
#include "Duck.hpp"

#include "Block.hpp"

Duck::Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    state = MovementState::IDLE;
    side = MovementSide::MV_RIGHT;

    grounded = true;
    vel = ZEROVECTOR_F;

    lc = rc = false;

    rects.push_back(sf::IntRect(ZEROVECTOR_I, sf::Vector2i(64, 128)));
    rects.push_back(sf::IntRect(sf::Vector2i(64, 0), sf::Vector2i(64, 128)));

    setSprite(core.resources->Texture("duck"), rects[state*2+side]);

    setBody(b2BodyType::b2_dynamicBody, true);

    addFixture(createRectangle(b2Vec2(38, 20), b2Vec2(0, 53)), CollisionCategory::DUCK, 0.f, 0.f, 0.f, true); //0
    addFixture(createRectangle(b2Vec2(12, 20), b2Vec2(-25, 53)), CollisionCategory::DUCK, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(12, 20), b2Vec2(25, 53)), CollisionCategory::DUCK, 0.f, 0.f, 0.f, true); //2
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, -53)), CollisionCategory::DUCK, 0.f, 0.f, 0.f, true); //3

    addFixture(createRectangle(b2Vec2(12, 16), b2Vec2(-25, 51)), CollisionCategory::DUCK, 0.f, 0.f, 0.f);
    addFixture(createRectangle(b2Vec2(38, 20), b2Vec2(0, 53)), CollisionCategory::DUCK, 0.8f, 0.f, 1.f);
    addFixture(createRectangle(b2Vec2(12, 16), b2Vec2(25, 51)), CollisionCategory::DUCK, 0.f, 0.f, 0.f);
    addFixture(createRectangle(b2Vec2(62, 106), b2Vec2(0, -10)), CollisionCategory::DUCK, 0.f, 0.f, 1.f);

    physicize(world);

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
    /*
    b2Vec2 currVel = body->GetLinearVelocity();
    float coef = 0.92f;
    if(grounded) coef = 0.82f;
    float tmpVel = 0.f;
    if(side == MovementSide::MV_LEFT) tmpVel = std::min(vel.x, currVel.x * coef);
    else if(side == MovementSide::MV_RIGHT) tmpVel = std::max(vel.x, currVel.x * coef);

    float finalVel = tmpVel - currVel.x;
    float impulse = body->GetMass() * finalVel;
    body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
    */
    if(vel.x != 0)
    {
        body->SetLinearVelocity(b2Vec2(vel.x, body->GetLinearVelocity().y));
    }
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
                    if(grounded) 
                    {
                        body->ApplyLinearImpulse(b2Vec2(0, -30), body->GetPosition(), true);
                        grounded = false;
                    }
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
                        vel.x -= 8;
                        side = MovementSide::MV_LEFT;
                        lc = true;
                    }
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(state != MovementState::FLOOR)
                    {
                        vel.x += 8;
                        side = MovementSide::MV_RIGHT;
                        rc = true;
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
                    if(lc)
                    {
                        vel.x += 8;
                    }
                    lc = false;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(rc)
                    {  
                        vel.x -= 8;
                    }
                    rc = false;
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

void Duck::onCollision(int fixtureid, PhysicEntity* collided)
{
    printInfo("> DUCK COLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 0:
        {
            if(dynamic_cast<Block*>(collided) != nullptr)
            {
                grounded = true;
                printInfo(" --- GROUNDED --- ");
            }
        }
            break;
        case 3:
        {

        }
            break;
        default:
            break;
    }
}