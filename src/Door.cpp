
#include "Door.hpp"

#include "Utils.hpp"

Door::Door(Core core, Scene_Play* play, const sf::Vector2f& position) : Object(play, position)
{
    setSprite(core.resources->Texture("duck_full"), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(64, 128)));

    addBody(b2BodyType::b2_staticBody, true);
    addFixture(createRectangle(b2Vec2(64, 128), b2Vec2(-32, 0)), CollisionCategory_STATIC_FOREGROUND, 0.f, 0.f, 1.f);
}

Door::~Door() {}