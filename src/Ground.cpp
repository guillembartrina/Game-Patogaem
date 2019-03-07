
#include "Ground.hpp"

Ground::Ground(Core core, Scene_Play* play, const sf::Vector2f& position) : PhysicEntity(play, position)
{
    setSprite(core.resources->Texture("box"));
    setPhysics(b2BodyType::b2_staticBody, createRectangle(b2Vec2(64.f, 64.f)), 0.2f, 1.f, 0.f);
}

Ground::~Ground() {}