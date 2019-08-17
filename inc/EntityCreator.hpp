
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP

#include <iostream>

//All includes...
#include "Entity.hpp"
#include "GenericEntity.hpp"

#include "PhysicEntity.hpp"
#include "GenericPhysicEntity.hpp"
#include "Block.hpp"

#include "Object.hpp"
#include "Door.hpp"

enum EntityCode
{
    ENTITY = 0x0000,
    GENERICENTITY = 0x0001,

    PHYSICENTITY = 0x8000,
    GENERICPHYSICENTITY = 0x8001,
    DUCK = 0xC001,
    BLOCK = 0xC008,
    BLOCK_BLUECASTLE = 0xC009,

    OBJECT = 0xE000,
    DOOR = 0xE081
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
        //DUCK IN SCENE_PLAY EXPLICIT
        case 0xC008:
            return new Block(core, play, position, BLOCK);
            break;
        case 0xC009:
            return new Block(core, play, position, BLOCK_BLUECASTLE);
            break;
        case 0xE000:
            return new Object();
            break;
        case 0xE081:
            return new Door(core, play, position);
            break;
        default:
            assert(false);
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

static const Codepair IS_DUCK = std::make_pair(DUCK, 0xFFFF); //1000 0000 0000 1001
static const Codepair IS_BLOCK = std::make_pair(BLOCK, 0xFFF8); //1000 0000 0000 1000
static const Codepair IS_BLOCK_BLUECASTLE = std::make_pair(BLOCK_BLUECASTLE, 0xFFFF); //1000 0000 0000 1001
//static const Codepair IS_OTHERPHYSICENTITIES = std::make_pair(0xC000, 0xC000); //11XX XXXX XXXX XXXX

static const Codepair IS_OBJECT = std::make_pair(OBJECT, 0xE000); //1000 0000 0000 1001
static const Codepair IS_DOOR = std::make_pair(DOOR, 0xFFFF);

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