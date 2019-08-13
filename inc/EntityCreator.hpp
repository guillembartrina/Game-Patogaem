
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP

//All includes...
#include "Entity.hpp"
#include "GenericEntity.hpp"

#include "PhysicEntity.hpp"
#include "GenericPhysicEntity.hpp"
#include "Block.hpp"

enum EntityCode
{
    ENTITY = 0x0000,
    GENERICENTITY = 0x0001,
    //ENTITIES = 0x6...
    PHYSICENTITY = 0x8000,
    GENERICPHYSICENTITY = 0x8001,
    BLOCK = 0xC000,
    BLOCK_BLUECASTLE = 0xC001
    //PHYSICENTITIES = 0xC....
};

static Entity* getEntitybyCode(EntityCode ic, Core core, Scene_Play* play, const sf::Vector2f& position)
{
    switch(ic)
    {
        case 0x0000:
            return new Entity();
            break;
        case 0x0001:
            return new GenericEntity(core, play, position, "test");
            break;
        case 0x8000:
            return new PhysicEntity();
            break;
        case 0x8001:
            return new GenericPhysicEntity(core, play, position, "test");
            break;
        case 0xC000:
            return new Block(core, play, position, BLOCK);
            break;
        case 0xC001:
            return new Block(core, play, position, BLOCK_BLUECASTLE);
            break;
        default:
            return nullptr;
            break;
    }
};

typedef std::pair<unsigned short, unsigned short> Codepair; //code + mask

static bool isTarjet(const Entity* entity, const Codepair& codepair)
{
    return (((entity->getCODE() & codepair.second) ^ codepair.first) == 0x0000);
};

static const Codepair IS_ENTITY = std::make_pair(ENTITY, 0x0000); //XXXX XXXX XXXX XXXX
static const Codepair IS_GENERICENTITY = std::make_pair(GENERICENTITY, 0xFFFF); //0000 0000 0000 0001
//static const Codepair IS_OTHERENTITIES = std::make_pair(0x6000, 0xC000); //01XX XXXX XXXX XXXX

static const Codepair IS_PHYSICENTITY = std::make_pair(PHYSICENTITY, 0x8000); //1XXX XXXX XXXX XXXX
static const Codepair IS_GENERICPHYSICENTITY = std::make_pair(GENERICPHYSICENTITY, 0xFFFF); //1000 0000 0000 0001
static const Codepair IS_BLOCK = std::make_pair(BLOCK, 0xFFF0); //1000 0000 0000 0001
static const Codepair IS_BLOCK_BLUECASTLE = std::make_pair(BLOCK_BLUECASTLE, 0xFFFF); //1000 0000 0000 0001
//static const Codepair IS_OTHERPHYSICENTITIES = std::make_pair(0xC000, 0xC000); //11XX XXXX XXXX XXXX

/*
CRATE
BIDON
TROWABLES:
    GRENADE
    FLAMEBOMB
GUNS:
    PISTOL
    SHOTGUN
    MP5
    RAY
    REVOLVER
    TRAP

*/

#endif