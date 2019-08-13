
#include "Block.hpp"

#include "Utils.hpp"

const b2EdgeShape Block::edges[4] = {   
                                        createEdge(b2Vec2(-CELLSIZE.x / 2.f, -CELLSIZE.y / 2.f), b2Vec2(CELLSIZE.x / 2.f, -CELLSIZE.y / 2.f)),
                                        createEdge(b2Vec2(CELLSIZE.x / 2.f, (-CELLSIZE.y / 2.f) + 0), b2Vec2(CELLSIZE.x / 2.f, (CELLSIZE.y / 2.f) - 0)),
                                        createEdge(b2Vec2(CELLSIZE.x / 2.f, CELLSIZE.y / 2.f), b2Vec2(-CELLSIZE.x / 2.f, CELLSIZE.y / 2.f)),
                                        createEdge(b2Vec2(-CELLSIZE.x / 2.f, (CELLSIZE.y / 2.f) - 0), b2Vec2(-CELLSIZE.x / 2.f, (-CELLSIZE.y / 2.f) + 0)) 
                                    };

const std::string Block::types[2] = { "default", "bluecastle" };

Block::Block(Core core, Scene_Play* play, const sf::Vector2f& position, unsigned short type) : PhysicEntity(play, position)
{
    int i = position.x / CELLSIZE.x;
    int j = position.y / CELLSIZE.y;

    char sides = 0x0F; //0000 1111
    if(i-1 >= 0 and testmap[j][i-1] == 49153) sides = sides & 0x0E; //0000 111X
    if(i+1 < NUMCELLS.x and testmap[j][i+1] == 49153) sides = sides & 0x0B; //0000 1X11
    if(j-1 >= 0 and testmap[j-1][i] == 49153) sides = sides & 0x07; //0000 X111
    if(j+1 < NUMCELLS.y and testmap[j+1][i] == 49153) sides = sides & 0x0D; //0000 11X1
    
    setSprite(core.resources->Texture(types[type - 0xC000]), sf::IntRect(64 + ((sides & 0x08) >> 3) * 64 * 3, 0, 64, 64)); // change?
    
    addBody(b2BodyType::b2_staticBody);

    for(int i = 0; i < 4; ++i)
    {
        if(sides & (0x08 >> i)) addFixture(&edges[i], CollisionCategory_STATIC_FOREGROUND, 0.f, 0.f, 1.f);
    }

    setCODE(type);
}

Block::~Block() {}