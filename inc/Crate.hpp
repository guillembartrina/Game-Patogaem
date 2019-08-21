
#ifndef CRATE_HPP
#define CRATE_HPP

#include "Holdable.hpp"

class Crate : public Holdable
{
    public:

    Crate(Core core, Scene_Play* play, const sf::Vector2f& position);
    ~Crate();

    private:

};

#endif