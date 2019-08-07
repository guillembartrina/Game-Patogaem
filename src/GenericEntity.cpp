
#include "GenericEntity.hpp"

#include "Utils.hpp"

GenericEntity::GenericEntity(Core core, Scene_Play* play, const sf::Vector2f& position, std::string name) : Entity(play, position)
{
    setSprite(core.resources->Texture(name), sf::IntRect(ZEROVECTOR_I, sf::Vector2i(CELLSIZE)));

    sf::Vector2u rect = core.resources->Texture(name).getSize();

    if(rect.x > int(CELLSIZE.x))
    {
        animate(rect.x / int(CELLSIZE.x), sf::seconds(0.1f));
    }

    CODE = 0x0001;
}

GenericEntity::~GenericEntity() {}