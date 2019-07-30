
#include "Duck.hpp"

Duck::Duck(b2World& world, Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    //Duck vars
    state = MovementState::State_IDLE;
    side = MovementSide::Side_RIGHT;

    groundings = 0;
    vel = ZEROVECTOR_F;

    lc = rc = false;

    //Image
    setSprite(core.resources->Texture("duck"), sf::IntRect(0, 0, 64, 128));

    animate(4, sf::seconds(0.1f));
    stopAnimation();

    //Bodies
    setBody(b2BodyType::b2_dynamicBody, true);

    //-->0
    addFixture(createRectangle(b2Vec2(62, 126)), CollisionCategory::DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory::ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, -53)), CollisionCategory::ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    physicize(world);

    bodies[0] = body;

    //-->1
    resetPhysics();

    addFixture(createRectangle(b2Vec2(62, 106), b2Vec2(0, 10)), CollisionCategory::DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory::ALL_COLLISION, 0.f, 0.f, 0.f, true); //1
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, -33)), CollisionCategory::ALL_COLLISION, 0.f, 0.f, 0.f, true); //2

    physicize(world);

    bodies[1] = body;
    body->SetActive(false);

    //-->2
    resetPhysics();

    addFixture(createRectangle(b2Vec2(126, 62), b2Vec2(0, 32)), CollisionCategory::DUCK, 0.f, 0.f, 1.f); //0
    addFixture(createRectangle(b2Vec2(52, 20), b2Vec2(0, 53)), CollisionCategory::ALL_COLLISION, 0.f, 0.f, 0.f, true); //1

    physicize(world);

    bodies[2] = body;
    body->SetActive(false);

    //---

    body = bodies[0];
}

Duck::~Duck()
{
    body = bodies[0];
    bodies[1]->GetWorld()->DestroyBody(bodies[1]);
    bodies[2]->GetWorld()->DestroyBody(bodies[2]);
}

void Duck::update(const sf::Time deltatime)
{
    //If up pressed keep flying
   if((rc or lc) and state != MovementState::State_FLOOR)
   {
       playAnimation();

       b2Vec2 currVelocity = body->GetLinearVelocity();
       float tempVelocity;
       if(side == MovementSide::Side_RIGHT) tempVelocity = 7.f;
       else tempVelocity = -7.f;
       float finalVelocity = tempVelocity - currVelocity.x;
       float impulse = body->GetMass() * finalVelocity;
       body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
   }
   else
   {
       //change if flloor, fer el truple de llarg!
       b2Vec2 currVelocity = body->GetLinearVelocity();
       float tempVelocity = currVelocity.x * 0.90f;
       float finalVelocity = tempVelocity - currVelocity.x;
       float impulse = body->GetMass() * finalVelocity;
       body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);

       stopAnimation();
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
                    if(groundings) 
                    {
                        body->ApplyLinearImpulse(b2Vec2(0, -28), body->GetPosition(), true);
                        changeState(MovementState::State_JUMP);
                    }
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(state)
                    {
                        case MovementState::State_IDLE:
                        {
                            changeState(MovementState::State_DOWN);
                        }
                            break;
                        case MovementState::State_WALK:
                        {
                            if(groundings) changeState(MovementState::State_FLOOR);
                            else changeState(MovementState::State_DOWN);
                        }
                            break;
                        default:
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Left:
                {
                    switch(state)
                    {
                        case MovementState::State_DOWN:
                        {
                            side = MovementSide::Side_LEFT;
                        }
                            break;
                        case MovementState::State_FLOOR:
                        {
                            body->ApplyLinearImpulse(b2Vec2(-7.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            if(rc) changeState(MovementState::State_IDLE); 
                            else changeState(MovementState::State_WALK); 

                            side = MovementSide::Side_LEFT;
                            lc = true;
                        }
                            break;
                    }
                }
                    break;
                case sf::Keyboard::Right:
                {
                    switch(state)
                    {
                        case MovementState::State_DOWN:
                        {
                            side = MovementSide::Side_RIGHT;
                        }
                            break;
                        case MovementState::State_FLOOR:
                        {
                            body->ApplyLinearImpulse(b2Vec2(7.f * body->GetMass(), 0.f), body->GetWorldCenter(), true);
                        }
                            break;
                        default:
                        {
                            if(lc) changeState(MovementState::State_IDLE);
                            else changeState(MovementState::State_WALK);

                            side = MovementSide::Side_RIGHT;
                            rc = true;
                        }
                            break;
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
                    //Lower the jump?
                }
                    break;
                case sf::Keyboard::Down:
                {
                    switch(state)
                    {
                        case MovementState::State_DOWN:
                        case MovementState::State_FLOOR: //<---------------
                        {
                            if(lc or rc) changeState(MovementState::State_WALK);
                            else changeState(MovementState::State_IDLE);
                        }
                            break;
                        default:
                            break;
                        /*
                        case MovementState::State_FLOOR:
                        {
                            //Check posar-se dret
                        }
                            break;
                        */
                    };
                }
                    break;
                case sf::Keyboard::Left:
                {
                    if(lc and state != MovementState::State_FLOOR)
                    {
                        if(rc) { changeState(MovementState::State_WALK); side = MovementSide::Side_RIGHT; }
                        else changeState(MovementState::State_IDLE);
                    }
                    lc = false;
                }
                    break;
                case sf::Keyboard::Right:
                {
                    if(rc and state != MovementState::State_FLOOR)
                    {  
                        if(lc) { changeState(MovementState::State_WALK); side = MovementSide::Side_LEFT; }
                        else changeState(MovementState::State_IDLE);
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
    sprite.setTextureRect(sf::IntRect(0, 0*256+side*128, 64, 128));
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
                //if(rc or lc) state = MovementState::State_WALK;
                //else state = MovementState::State_IDLE;
                groundings++;
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
        default:
            break;
    }
}

void Duck::changeState(MovementState newstate) //simplify??
{
    b2Vec2 pos = body->GetPosition();

    switch(newstate)
    {
        case MovementState::State_IDLE:
        case MovementState::State_WALK:
        case MovementState::State_JUMP:
        case MovementState::State_FLY:
        {
            if(state == MovementState::State_DOWN or state == MovementState::State_FLOOR)
            {
                body->SetActive(false);
                body = bodies[0];
                body->SetActive(true);

                sprite.setColor(sf::Color::Green);
            }
        }
            break;
        case MovementState::State_DOWN:
        {
            body->SetActive(false);
            body = bodies[1];
            body->SetActive(true);

            sprite.setColor(sf::Color::Magenta);
        }
            break;
        case MovementState::State_FLOOR:
        {
            body->SetActive(false);
            body = bodies[2];
            body->SetActive(true);

            sprite.setColor(sf::Color::Blue);
        }
            break;
    };

    body->SetTransform(pos, 0.f);

    state = newstate;
}