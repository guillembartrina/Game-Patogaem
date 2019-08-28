
#include "Duck.hpp"

#include <cmath>

#include "Utils.hpp"
#include "EntityCreator.hpp"

const float Duck::dynamicsValues[5] = {7.f, 7.f, 4.5f, 0.f, 10.f};
const float Duck::dynamicsCoefs[5] = {0.75f, 0.85f, 0.85f, 0.f, 0.95f};
const sf::Vector2f Duck::holdableOffset[5] = {sf::Vector2f(20, 0), sf::Vector2f(20, 0), sf::Vector2f(20, 0), sf::Vector2f(20, 20), sf::Vector2f(0, 20)};

Duck::Duck(Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    //Duck vars
    state = MovementState_JUMPING;
    side = MovementSide_RIGHT;

    holdable = nullptr;

    groundings = headings = 0;

    pL = pR = pU = pD = false;
    go = gz = hz = false;

    cp = false;

    //Image
    setSprite(core.resources->Texture("duck"), sf::IntRect(0, 0, CELLSIZE.x, CELLSIZE.y*2));

    animate(6, sf::seconds(0.1f));
    stopAnimation();

    //Bodies
    addBody(b2BodyType::b2_dynamicBody, true);

    //-->0
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-4, (CELLSIZE.y*2)-4)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 52))); //1
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, -52))); //2
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-4, (CELLSIZE.y*2)-4))); //3

    //-->1
    addBody(b2BodyType::b2_dynamicBody, true);

    addFixture(createRectangle(b2Vec2(CELLSIZE.x-4, 88-4), b2Vec2(0, 20)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 52))); //1
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, -12))); //2
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-4, 88-4), b2Vec2(0, 20))); //3

    //-->2
    addBody(b2BodyType::b2_dynamicBody, true);

    addFixture(createRectangle(b2Vec2((CELLSIZE.x*2)-4, CELLSIZE.y-4), b2Vec2(0, 32)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture_Sensor(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 52))); //1
    addFixture_Sensor(createRectangle(b2Vec2((CELLSIZE.x*2)-62, 20), b2Vec2(0, 12))); //2
    addFixture_Sensor(createRectangle(b2Vec2((CELLSIZE.x*2)-4, CELLSIZE.y-4), b2Vec2(0, 32))); //3

    //---

    //clean code and calls to body, swithes,...
    //restore ground/head on changebody

    addSound(core.resources->Sound("quack"));

    setCODE(DUCK);

    change = false;
}

Duck::~Duck() {}

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
                    if(state == MovementState_STANDING and not headings) //groundings?
                    {
                        body->ApplyLinearImpulse(b2Vec2(0, -32), body->GetWorldCenter(), true);
                        //changeState(MovementState_JUMPING);
                        change = true;
                        ns = MovementState_JUMPING;
                    }
                    else if(state == MovementState_JUMPING)
                    {
                        //changeState(MovementState_FLYING);
                        change = true;
                        ns = MovementState_FLYING;
                    }
                    pU = true;
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(state)
                    {
                        case MovementState_STANDING:
                        {
                            if(pL xor pR)
                            {
                                //changeState(MovementState_FLOORING);
                                change = true;
                                ns = MovementState_FLOORING;
                                body->ApplyLinearImpulse(b2Vec2(dynamicsValues[state] * std::pow(-1, (int)side) * body->GetMass(), 0), body->GetWorldCenter(), true); //PULSE, change?
                            }
                            else
                            {
                                //changeState(MovementState_DOWNING);
                                change = true;
                                ns = MovementState_DOWNING;
                            }
                        }
                            break;
                        default:
                            break;
                    }
                    pD = true;
                }
                    break;
                case sf::Keyboard::Left:
                {
                    switch(state)
                    {
                        case MovementState_FLOORING:
                        {
                            if(side == MovementSide_LEFT) body->ApplyLinearImpulse(b2Vec2(-dynamicsValues[state]/2.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            side = MovementSide_LEFT;
                        }
                            break;
                    }
                    pL = true;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    switch(state)
                    {
                        case MovementState_FLOORING:
                        {
                            if(side == MovementSide_RIGHT) body->ApplyLinearImpulse(b2Vec2(dynamicsValues[state]/2.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            side = MovementSide_RIGHT;
                        }
                            break;
                    }
                    pR = true;
                }
                    break;
                case sf::Keyboard::E:
                {
                    playSound(0);
                    std::cerr << groundings << " " << headings << std::endl;
                }
                    break;
                case sf::Keyboard::C:
                {
                    if(holdable == nullptr)
                    {
                        if(not holdables.empty())
                        {
                            holdable = (*holdables.begin())->get();
                            holdables.erase(holdable);

                            play->deleteEntity(static_cast<Entity*>(holdable));
                            holdable->setPosition(getPosition());
                        }
                    }
                    else
                    {
                        sf::Vector2f impulse = ZEROVECTOR_F;

                        if(pL xor pR)
                        {
                            impulse.x = (pL ? -25.f : 25.f);

                            if(pU) impulse.y = -20.f;
                        }
                        else if(pU) impulse.y = -15.f;


                        play->addEntity(holdable->unget(impulse));
                        holdable = nullptr;
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
                    if(not groundings and state == MovementState_FLYING)
                    {
                        //changeState(MovementState_JUMPING);
                        change = true;
                        ns = MovementState_JUMPING;
                    }
                    else if(state == MovementState_JUMPING)
                    {
                        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y * 0.6f));
                    }
                    pU = false;
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(state)
                    {
                        case MovementState_DOWNING:
                        {
                            //changeState(MovementState_STANDING);
                            change = true;
                            ns = MovementState_STANDING;
                        }
                            break;
                        case MovementState_FLOORING:
                        {
                            if(not headings)
                            {
                                //changeState(MovementState_STANDING);
                                change = true;
                                ns = MovementState_STANDING;
                                if(pL and not pR) side = MovementSide_LEFT;
                                else if(pR and not pL) side = MovementSide_RIGHT;
                            }
                        }
                            break;
                        default:
                            break;
                    }
                    pD = false;
                }
                    break;
                case sf::Keyboard::Left:
                {
                    if(state != MovementState_FLOORING)
                    {
                        if(pR) side = MovementSide_RIGHT;
                    }
                    pL = false;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(state != MovementState_FLOORING)
                    {
                        if(pL) side = MovementSide_LEFT;
                    }
                    pR = false;
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

    setSpriteRect((getSpriteRect().top/256)*2+side);
}

void Duck::update(const sf::Time deltatime)
{
    PhysicEntity::update(deltatime);

    cp = false;

    if(change)
    {
        changeState(ns);
    }
    change = false;

    if(go)
    {
        if(state == MovementState_FLYING or state == MovementState_JUMPING)
        {
            if(pD) changeState(MovementState_DOWNING);
            else changeState(MovementState_STANDING);
        }
    }
    go = false;

    if(gz)
    {
        if(state == MovementState_STANDING) changeState(MovementState_JUMPING);
    }
    gz = false;

    if(hz)
    {
        if(state == MovementState_FLOORING and not pD) changeState(MovementState_STANDING);
    }
    hz = false;

    bool ableToMove = (state != MovementState_FLOORING and state != MovementState_DOWNING);
    if(ableToMove and (pL xor pR))
    {
        b2Vec2 currentVel = body->GetLinearVelocity();
        float idealVel = dynamicsValues[state] * std::pow(-1, (int)side);
        float impulse = body->GetMass() * (idealVel - currentVel.x);
        body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        if(state == MovementState_STANDING) playAnimation();
        else stopAnimation();
    }
    else
    {
        b2Vec2 currentVel = body->GetLinearVelocity();
        if(std::abs(currentVel.x) > 0.05f)
        {
            float idealVel = currentVel.x * dynamicsCoefs[state]; //diferent coefs?
            float impulse = body->GetMass() * (idealVel - currentVel.x);
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        }
        else body->SetLinearVelocity(b2Vec2(0.f, currentVel.y));
        stopAnimation();
    }

    if(state == MovementState_FLYING and body->GetLinearVelocity().y > 0.f)
    {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, std::min(body->GetLinearVelocity().y, 2.f)));
    }

    if(holdable != nullptr) holdable->setPosition(getPosition() + sf::Vector2f(holdableOffset[state].x * std::pow(-1.f, (int)side), holdableOffset[state].y));
}

void Duck::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Entity::draw(target, states);
    if(holdable != nullptr) target.draw(*holdable, states);
}

void Duck::onPrecollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc, b2Contact* contact)
{
    printInfo("> DUCK PRECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    if(isTarjet(collided, IS_CRATE))
    {
        b2WorldManifold wm;
        contact->GetWorldManifold(&wm);
        if(wm.normal.y != -1 or ignoring.find(collided->getID()) != ignoring.end())
        {
            //if(ignoring.find(collided->getID()) == ignoring.end()) std::cerr << "(" << fixtureid << ") ADD IGNORING CRATE " << collided->getID() << std::endl;
            ignoring.insert(collided->getID());
            contact->SetEnabled(false);
        }
    }
    else if(isTarjet(collided, IS_DOOR))
    {
        ignoring.insert(collided->getID());
        //std::cerr << "ADD IGNORING DOOR " << collided->getID() << std::endl;
        contact->SetEnabled(false);
    }
}

void Duck::onCollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc)
{
    printInfo("> DUCK COLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if((cc & FOREGROUND_MASK) != 0x0000 and ignoring.find(collided->getID()) == ignoring.end())
            {
                if(groundings == 0) go = true;
                groundings++;
                //std::cerr << "BOT COLLIDED " << collided->getID() << " - GrC: " << groundings << std::endl;
            }
            /*
            else
            {
                if(ignoring.find(collided->getID()) != ignoring.end()) std::cerr << "BOT IGNORING " << collided->getID() << std::endl;
            }
            */
        }
            break;
        case 2:
        {
            if((cc & FOREGROUND_MASK) != 0x0000 and ignoring.find(collided->getID()) == ignoring.end())
            {
                headings++;
                //std::cerr << "TOP COLLIDED " << collided->getID() << " - HdC: " << headings << std::endl;
            }
            /*
            else
            {
                if(ignoring.find(collided->getID()) != ignoring.end()) std::cerr << "TOP IGNORING " << collided->getID() << std::endl;
            }
            */
        }
            break;
        case 3:
        {
            if(isTarjet(collided, IS_HOLDABLE))
            {
                holdables.insert(static_cast<Holdable*>(collided));
            }

            if(isTarjet(collided, IS_CRATE) and getPosition().y+64 >= collided->getPosition().y)
            {
                ignoring.insert(collided->getID());
            }
        }
            break;
        default:
            break;
    }
}

void Duck::onDecollision(unsigned short fixtureid, PhysicEntity* collided, unsigned short cc)
{
    printInfo("> DUCK DECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if((cc & FOREGROUND_MASK) != 0x0000 and ignoring.find(collided->getID()) == ignoring.end())
            {
                groundings--;
                if(groundings == 0) gz = true;
                //std::cerr << "BOT DECOLLIDED " << collided->getID() << " - GrC: " << groundings << std::endl;
            }
            /*
            else
            {
                if(ignoring.find(collided->getID()) != ignoring.end()) std::cerr << "BOT OUT IGNORING " << collided->getID() << std::endl;
            }
            */
        }
            break;
        case 2:
        {
            if((cc & FOREGROUND_MASK) != 0x0000 and ignoring.find(collided->getID()) == ignoring.end())
            {
                headings--;
                if(headings == 0) hz = true;
                //std::cerr << "TOP DECOLLIDED " << collided->getID() << " - HdC: " << headings << std::endl;
            }
            /*
            else
            {
                if(ignoring.find(collided->getID()) != ignoring.end()) std::cerr << "TOP OUT IGNORING " << collided->getID() << std::endl;
            }
            */
        }
            break;
        case 3:
        {
            if(isTarjet(collided, IS_HOLDABLE))
            {
                holdables.erase(static_cast<Holdable*>(collided));
            }

            if(ignoring.find(collided->getID()) != ignoring.end())
            {   
                if(cp == false)
                {
                    //std::cerr << "DELETE INGNORING -> " << collided->getID() << std::endl;
                    ignoring.erase(collided->getID());
                }
            }
        }
            break;
        default:
            break;
    }
}

void Duck::changeState(MovementState newstate)
{
    //std::cerr << " > Changed state to " << MovementState_String[newstate] << std::endl;
    switch(newstate)
    {
        case MovementState_STANDING:
        case MovementState_JUMPING:
        case MovementState_FLYING:
        {
            if(state == MovementState_DOWNING or state == MovementState_FLOORING)
            {
                setBody(0);
                setSpriteRect(0+side);
                cp = true;
            }
        }
            break;
        case MovementState_DOWNING:
        {
            if(state != MovementState_DOWNING)
            {
                setBody(1);
                setSpriteRect(2+side);
                cp = true;
            }
        }
            break;
        case MovementState_FLOORING:
        {
            if(state != MovementState_FLOORING)
            {
                setBody(2);
                setSpriteRect(4+side);
                cp = true;
            }
        }
            break;
    };

    state = newstate;
}

MovementState Duck::getState() const
{
    return state;
}