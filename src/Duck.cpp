
#include "Duck.hpp"

#include <math.h>

#include "Utils.hpp"
#include "EntityCreator.hpp"

const float Duck::stateValues[5] = {7.f, 7.f, 4.5f, 0.f, 10.f};
const float Duck::stateCoefs[5] = {0.75f, 0.85f, 0.85f, 0.f, 0.95f};

Duck::Duck(Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    //Duck vars
    state = MovementState_STANDING;
    side = MovementSide_RIGHT;

    holdable = nullptr;

    groundings = headings = 0;

    pL = pR = pU = pD = false;
    go = gz = hz = false;

    quack.setBuffer(core.resources->Sound("quack"));
    quack.setVolume(80.f);

    //Image
    setSprite(core.resources->Texture("duck"), sf::IntRect(0, 0, CELLSIZE.x, CELLSIZE.y*2));

    animate(4, sf::seconds(0.1f));
    stopAnimation();

    //Bodies
    addBody(b2BodyType::b2_dynamicBody, true);

    //-->0
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-2, (CELLSIZE.y*2)-2)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, -53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    //-->1
    addBody(b2BodyType::b2_dynamicBody, true);

    addFixture(createRectangle(b2Vec2(CELLSIZE.x-2, 86), b2Vec2(0, 20)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, -33)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    //-->2
    addBody(b2BodyType::b2_dynamicBody, true);

    addFixture(createRectangle(b2Vec2((CELLSIZE.x*2)-2, CELLSIZE.y-2), b2Vec2(0, 32)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(CELLSIZE.x-12, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2((CELLSIZE.x*2)-32, 20), b2Vec2(0, 0)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    //---

    //create vectors with coefs and velocities to not HARD CODE!!!!!!!!!!!!!!!
    //clean code and calls to body, swithes,...
    //restore ground/head on changebody

    setCODE(DUCK);
}

Duck::~Duck() {}

void Duck::update(const sf::Time deltatime)
{   
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
        if(state == MovementState_FLOORING) changeState(MovementState_STANDING);
    }
    hz = false;


    bool ableToMove = (state != MovementState_FLOORING and state != MovementState_DOWNING);
    if(ableToMove and (pL xor pR))
    {
        b2Vec2 currentVel = body->GetLinearVelocity();
        float idealVel = stateValues[state] * std::pow(-1, (int)side);
        float impulse = body->GetMass() * (idealVel - currentVel.x);
        body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        //playAnimation();
    }
    else
    {
        b2Vec2 currentVel = body->GetLinearVelocity();
        if(std::abs(currentVel.x) > 0.05f)
        {
            float idealVel = currentVel.x * stateCoefs[state]; //diferent coefs?
            float impulse = body->GetMass() * (idealVel - currentVel.x);
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        }
        else body->SetLinearVelocity(b2Vec2(0.f, currentVel.y));
        //stopAnimation();
    }

    if(state == MovementState_FLYING and body->GetLinearVelocity().y > 0.f)
    {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, std::min(body->GetLinearVelocity().y, 2.f)));
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
                    if(state == MovementState_STANDING and not headings) //groundings?
                    {
                        body->ApplyLinearImpulse(b2Vec2(0, -32), body->GetWorldCenter(), true);
                        changeState(MovementState_JUMPING);
                    }
                    else if(state == MovementState_JUMPING)
                    {
                        changeState(MovementState_FLYING);
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
                                changeState(MovementState_FLOORING);
                                body->ApplyLinearImpulse(b2Vec2(stateValues[state] * std::pow(-1, (int)side) * body->GetMass(), 0), body->GetWorldCenter(), true); //PULSE, change?
                            }
                            else changeState(MovementState_DOWNING);
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
                            if(side == MovementSide_LEFT) body->ApplyLinearImpulse(b2Vec2(-stateValues[state]/2.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
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
                            if(side == MovementSide_RIGHT) body->ApplyLinearImpulse(b2Vec2(stateValues[state]/2.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
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
                    quack.play();
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
                        changeState(MovementState_JUMPING);
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
                            changeState(MovementState_STANDING);
                        }
                            break;
                        case MovementState_FLOORING:
                        {
                            if(not headings)
                            {
                                changeState(MovementState_STANDING);
                                if(pL and not pR) side = MovementSide_LEFT;
                                else if(pR and not pL) side = MovementSide_RIGHT;
                                //check no stand up!
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

void Duck::onCollision(unsigned int fixtureid, PhysicEntity* collided)
{
    printInfo("> DUCK COLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(isTarjet(collided, IS_BLOCK))
            {
                if(groundings == 0) go = true;
                groundings++;
            }
        }
            break;
        case 2:
        {
            if(isTarjet(collided, IS_BLOCK)) headings++;
        }
            break;
        default:
            break;
    }
}

void Duck::onDecollision(unsigned int fixtureid, PhysicEntity* collided)
{
    printInfo("> DUCK DECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(isTarjet(collided, IS_BLOCK))
            {
                groundings--;
                if(groundings == 0) gz = true;
            }
        }
            break;
        case 2:
        {
            if(isTarjet(collided, IS_BLOCK))
            {
                headings--;
                if(headings == 0) hz = true;
            }
        }
            break;
        default:
            break;
    }
}

void Duck::changeState(MovementState newstate)
{
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
            }
        }
            break;
        case MovementState_DOWNING:
        {
            if(state != MovementState_DOWNING)
            {
                setBody(1);
                setSpriteRect(2+side);
            }
        }
            break;
        case MovementState_FLOORING:
        {
            if(state != MovementState_FLOORING)
            {
                setBody(2);
                setSpriteRect(4+side);
                hz = false;
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