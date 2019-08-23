
#include "Crate.hpp"

#include "Utils.hpp"
#include "EntityCreator.hpp"

Crate::Crate(Core core, Scene_Play* play, const sf::Vector2f& position) : Holdable(play, position)
{
    setSprite(core.resources->Texture("crate"), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(CELLSIZE)));
    
    addBody(b2BodyType::b2_dynamicBody, true);
    addFixture(createRectangle(tob2Vec2(sf::Vector2f(CELLSIZE.x-4, CELLSIZE.y-4))), CollisionCategory_DYNAMIC_FOREGROUND, 0.5f, 0.f, 1.f);

    setCODE(CRATE);
}

Crate::~Crate() {}