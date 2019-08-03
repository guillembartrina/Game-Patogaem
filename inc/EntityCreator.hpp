
#ifndef ENTITYCREATOR_HPP
#define ENTITYCREATOR_HPP

//All includes...
#include "Entity.hpp"
#include "GenericEntity.hpp"

#include "PhysicEntity.hpp"
#include "GenericPhysicEntity.hpp"

enum ItemCode
{
    ENTITY = 0x0000,
    GENERICENTITY = 0x0001,
    //ENTITIES = 0x6...
    PHYSICENTITY = 0x8000,
    GENERICPHYSICENTITY = 0x8001
    //PHYSICENTITIES = 0xC....
};

static Entity* getEntitybyCode(ItemCode ic, Core core, Scene_Play* play, const sf::Vector2f& position)
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
        default:
            printInfo("NO ENTITY OF CODE" << hex << ic)
            break;
    }
};


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