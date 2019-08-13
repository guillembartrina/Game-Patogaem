
#include "GenericPhysicEntity.hpp"

#include "Utils.hpp"

GenericPhysicEntity::GenericPhysicEntity(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name) : PhysicEntity(play, position)
{
    setSprite(core.resources->Texture(name), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(CELLSIZE)));

    sf::Vector2u rect = core.resources->Texture(name).getSize();

    if(rect.x > int(CELLSIZE.x))
    {
        animate(rect.x / int(CELLSIZE.x), sf::seconds(0.1f));
    }

    setBody(b2BodyType::b2_staticBody);
    addFixture(createRectangle(tob2Vec2(CELLSIZE)), CollisionCategory_STATIC_FOREGROUND, 0.2f, 0.f, 1.f);

    setCODE(0x8001);
}

GenericPhysicEntity::~GenericPhysicEntity() {}