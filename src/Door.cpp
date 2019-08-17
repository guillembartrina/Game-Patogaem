
#include "Door.hpp"

#include "Utils.hpp"
#include "EntityCreator.hpp"

const sf::Time Door::closeTime = sf::seconds(2.f);

Door::Door(Core core, Scene_Play* play, const sf::Vector2f& position) : Object(play, position - sf::Vector2f(0.f, CELLSIZE.y / 2.f))
{
    nl = nr = 0;

    setSprite(core.resources->Texture("door"), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(64, 128)));

    animate(3, sf::seconds(0.05f), false);
    stopAnimation();

    addBody(b2BodyType::b2_staticBody, true);
    addFixture(createRectangle(b2Vec2(32, 128)), CollisionCategory_STATIC_FOREGROUND, 0.f, 0.f, 1.f); //2
    addFixture(createRectangle(b2Vec2(128, 64), b2Vec2(64, 32)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //0
    addFixture(createRectangle(b2Vec2(128, 64), b2Vec2(-64, 32)), CollisionCategory_ALL_COLLISION, 0.f, 0.f, 0.f, true); //1

    setCODE(DOOR);
}

Door::~Door() {}

void Door::onPrecollision(unsigned int fixtureid, PhysicEntity* collided, b2Contact* contact)
{
    printInfo("> DOOR PRECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 0:
        {
            if(isTarjet(collided, IS_DUCK) and (nr or nl))
            {
                contact->SetEnabled(false);
            }
        }
            break;
        default:
            break;
    }
}

void Door::onCollision(unsigned int fixtureid, PhysicEntity* collided)
{
    printInfo("> DOOR COLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(isTarjet(collided, IS_DUCK))
            {
                if(not nr and not nl and not timerActive())
                {
                    setSpriteRect(2);
                    playAnimation();
                }

                nr++;
            }
        }
            break;
        case 2:
        {
            if(isTarjet(collided, IS_DUCK))
            {
                if(not nl and not nr and not timerActive())
                {
                    setSpriteRect(1);
                    playAnimation();
                }

                nl++;
            }
        }
            break;
        default:
            break;
    }
}

void Door::onDecollision(unsigned int fixtureid, PhysicEntity* collided)
{
    printInfo("> DOOR DECOLLISION --> FIXTURE(" << fixtureid << ") || " << collided->getID());

    switch(fixtureid)
    {
        case 1:
        {
            if(isTarjet(collided, IS_DUCK))
            {
                nr--;

                if(nr == 0)
                {
                    startTimer(closeTime);
                }
            }
        }
            break;
        case 2:
        {
            if(isTarjet(collided, IS_DUCK))
            {
                nl--;

                if(nl == 0)
                {
                    startTimer(closeTime);
                }
            }
        }
            break;
        default:
            break;
    }
}

void Door::action() {}

void Door::onTimerTrigger()
{
    if(nr == 0 and nl == 0)
    {
        setSpriteRect(0);
        stopAnimation();
    }
}