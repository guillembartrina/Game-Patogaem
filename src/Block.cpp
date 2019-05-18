
#include "Block.hpp"

b2EdgeShape Block::edges[4] = {    createEdge(b2Vec2(-CELLSIZE.x / 2.f, -CELLSIZE.y / 2.f), b2Vec2(CELLSIZE.x / 2.f, -CELLSIZE.y / 2.f)),
                                    createEdge(b2Vec2(CELLSIZE.x / 2.f, (-CELLSIZE.y / 2.f) + 0), b2Vec2(CELLSIZE.x / 2.f, (CELLSIZE.y / 2.f) - 0)),
                                    createEdge(b2Vec2(CELLSIZE.x / 2.f, CELLSIZE.y / 2.f), b2Vec2(-CELLSIZE.x / 2.f, CELLSIZE.y / 2.f)),
                                    createEdge(b2Vec2(-CELLSIZE.x / 2.f, (CELLSIZE.y / 2.f) - 0), b2Vec2(-CELLSIZE.x / 2.f, (-CELLSIZE.y / 2.f) + 0)) };

Block::Block(Core core, Scene_Play* play, const sf::Vector2f& position, char sides = 0x0F) : PhysicEntity(play, position)
{
    setSprite(core.resources->Texture("bluecastle"), sf::IntRect(64 + ((sides & 0x08) >> 3) * 64 * 3, 0, 64, 64)); // change?

    setBody(b2BodyType::b2_staticBody);

    for(int i = 0; i < 4; ++i)
    {
        if(sides & (0x08 >> i)) addFixture(&edges[i], CollisionCategory::STATIC_FOREGROUND, 0.2f, 0.f, 1.f);
    }
}

Block::~Block() {}