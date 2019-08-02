
#include "Duck.hpp"

Duck::Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    //Duck vars
    state = MovementState_STANDING;
    side = MovementSide_RIGHT;

    groundings = headings = 0;

    lc = rc = uc = dc = false;
    gr = false; //?

    //Image
    setSprite(core.resources->Texture("duck"), sf::IntRect(0, 0, 64, 128));

    animate(4, sf::seconds(0.1f));
    stopAnimation();

    //Bodies
    setBody(b2BodyType::b2_dynamicBody, true);

    //-->0
    addFixture(createRectangle(b2Vec2(62, 126)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, -53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    physicize(world);

    bodies[0] = body;

    //-->1
    resetPhysics();

    addFixture(createRectangle(b2Vec2(62, 96), b2Vec2(0, 15)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, -33)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    physicize(world);

    bodies[1] = body;
    body->SetActive(false);

    //-->2
    resetPhysics();

    addFixture(createRectangle(b2Vec2(126, 62), b2Vec2(0, 33)), CollisionCategory_DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(96, 20), b2Vec2(0, 0)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //2


    physicize(world);

    bodies[2] = body;
    body->SetActive(false);

    //---

    body = bodies[0];

    //create vectors with coefs and velocities to not HARD CODE!!
    //clean code and calls to body, swithes,...
}

Duck::~Duck()
{
    body = bodies[0];
    bodies[1]->GetWorld()->DestroyBody(bodies[1]);
    bodies[2]->GetWorld()->DestroyBody(bodies[2]);
}

void Duck::update(const sf::Time deltatime)
{
    if(gr and (state == MovementState_FLYING or state == MovementState_JUMPING))
    {
        if(dc) changeState(MovementState_DOWNING);
        /*else if(uc)
        {
            body->ApplyLinearImpulse(b2Vec2(0, -32), body->GetPosition(), true);
            changeState(MovementState_JUMPING); //? check no jump
        }
        */
        else
        {
            changeState(MovementState_STANDING);
        }
    }

    if(hr and state == MovementState_FLOORING)
    {
        changeState(MovementState_STANDING);
    }

    gr = false;
    hr = false;

    if(state != MovementState_FLOORING and (lc xor rc))
    {
        b2Vec2 currentVel = body->GetLinearVelocity();
        float idealVel = ((side == MovementSide_LEFT) ? -7.f : 7.f);
        if(state == MovementState_FLYING) idealVel *= 0.6;
        float impulse = body->GetMass() * (idealVel - currentVel.x);
        body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        playAnimation();
    }
    else
    {
        float coef = 0.88f;
        if(state == MovementState_FLOORING) coef = 0.98f;
        b2Vec2 currentVel = body->GetLinearVelocity();
        if(std::abs(currentVel.x) > 0.05f)
        {
            float idealVel = currentVel.x * 0.9f; //diferent coefs?
            float impulse = body->GetMass() * (idealVel - currentVel.x);
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
        }
        else
        {
            body->SetLinearVelocity(b2Vec2(0.f, currentVel.y));

            if(state == MovementState_FLOORING)
            {
                if(not headings)
                {
                    changeState(MovementState_STANDING);
                }
            }
        }
        stopAnimation();
    }

    if(state == MovementState_FLYING and body->GetLinearVelocity().y > 0.f)
    {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, std::min(body->GetLinearVelocity().y, 1.8f)));
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
                        body->ApplyLinearImpulse(b2Vec2(0, -32), body->GetPosition(), true);
                        changeState(MovementState_JUMPING);
                    }
                    else if(state == MovementState_JUMPING)
                    {
                        changeState(MovementState_FLYING);
                    }
                    uc = true;
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(state)
                    {
                        case MovementState_STANDING:
                        {
                            if(lc xor rc) changeState(MovementState_FLOORING); //add pulse?
                            else changeState(MovementState_DOWNING);
                        }
                            break;
                        case MovementState_JUMPING:
                        {
                            //changeState(MovementState_DOWNING);
                        }
                            break;
                        default:
                            break;
                    }
                    dc = true;
                }
                    break;
                case sf::Keyboard::Left:
                {
                    switch(state)
                    {
                        case MovementState_FLOORING:
                        {
                            if(side == MovementSide_LEFT) body->ApplyLinearImpulse(b2Vec2(-7.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            side = MovementSide_LEFT;
                        }
                            break;
                    }
                    lc = true;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    switch(state)
                    {
                        case MovementState_FLOORING:
                        {
                            if(side == MovementSide_RIGHT) body->ApplyLinearImpulse(b2Vec2(7.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            side = MovementSide_RIGHT;
                        }
                            break;
                    }
                    rc = true;
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
                    uc = false;
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
                                if(lc and not rc) side = MovementSide_LEFT;
                                else if(rc and not lc) side = MovementSide_RIGHT;
                                //check no stand up!
                            }
                        }
                            break;
                        default:
                            break;
                    }
                    dc = false;
                }
                    break;
                case sf::Keyboard::Left:
                {
                    if(state != MovementState_FLOORING)
                    {
                        if(rc) side = MovementSide_RIGHT;
                    }
                    lc = false;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(state != MovementState_FLOORING)
                    {
                        if(lc) side = MovementSide_LEFT;
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

    sprite.setTextureRect(sf::IntRect(0, (sprite.getTextureRect().top/256)*256+side*128, 64, 128));
}

void Duck::onCollision(int fixtureid, PhysicEntity* collided)
{
    printInfo("> DUCK COLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(collided->getCC() & FOREGROUND_MASK)
            {
                if(not groundings)
                {
                    gr = true;
                }
                groundings++;
            }
        }
            break;
        case 2:
        {
            if(collided->getCC() & FOREGROUND_MASK)
            {
                headings++;
            }
        }
            break;
        default:
            break;
    }
}

void Duck::onDecollision(int fixtureid, PhysicEntity* collided)
{
    printInfo("> DUCK DECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(collided->getCC() & FOREGROUND_MASK)
            {
                groundings--;
            }
        }
            break;
        case 2:
        {
            if(collided->getCC() & FOREGROUND_MASK)
            {
                if(headings == 1)
                {
                    hr = true;
                }

                headings--;
            }
        }
            break;
        default:
            break;
    }
}

void Duck::changeState(MovementState newstate)
{
    b2Vec2 pos = body->GetPosition();
    b2Vec2 vel = body->GetLinearVelocity();

    switch(newstate)
    {
        case MovementState_STANDING:
        case MovementState_JUMPING:
        case MovementState_FLYING:
        {
            if(state == MovementState_DOWNING or state == MovementState_FLOORING)
            {
                body->SetActive(false);
                body = bodies[0];
                body->SetActive(true);

                body->SetTransform(pos, 0.f);
                body->SetLinearVelocity(vel);

                sprite.setTextureRect(sf::IntRect(0, 0*256+side*128, 64, 128));
            }
        }
            break;
        case MovementState_DOWNING:
        {
            if(state != MovementState_DOWNING)
            {
                body->SetActive(false);
                body = bodies[1];
                body->SetActive(true);

                body->SetTransform(pos, 0.f);
                body->SetLinearVelocity(vel);

                sprite.setTextureRect(sf::IntRect(0, 1*256+side*128, 64, 128));
            }
        }
            break;
        case MovementState_FLOORING:
        {
            if(state != MovementState_FLOORING)
            {
                body->SetActive(false);
                body = bodies[2];
                body->SetActive(true);

                body->SetTransform(pos, 0.f);
                body->SetLinearVelocity(vel);

                sprite.setTextureRect(sf::IntRect(0, 2*256+side*128, 64, 128));
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