
#include "GenericEntity.hpp"

GenericEntity::GenericEntity(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name, std::pair<unsigned int, sf::Time> animationParams) : PhysicEntity(play, position)
{
    setSprite(core.resources->Texture(name));
    setPhysics(b2BodyType::b2_staticBody, createRectangle(b2Vec2(64.f, 64.f)), 0.2f, 1.f, 0.f);
    fixtureDef.filter = getFilter(CollisionCategory::STATIC_FOREGROUND);

    if(animationParams.first != 0)
    {
        animate(animationParams.first, animationParams.second);
    }
}

GenericEntity::~GenericEntity() {}