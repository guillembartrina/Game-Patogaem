
#include "Crate.hpp"

#include "Utils.hpp"
#include "EntityCreator.hpp"

Crate::Crate(Core core, Scene_Play* play, const sf::Vector2f& position) : Holdable(play, position)
{
    setSprite(core.resources->Texture("crate"), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(CELLSIZE)));
    
    addBody(b2BodyType::b2_dynamicBody);
    addFixture(createRectangle(tob2Vec2(CELLSIZE)), CollisionCategory_DYNAMIC_BACKGROUND, 0.4f, 0.f, 1.f);

    setCODE(CRATE);
}

Crate::~Crate() {}