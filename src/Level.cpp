
#include "Level.hpp"

#include "Utils.hpp"
#include "EntityCreator.hpp"

Level::Level() {}

Level::~Level() {}

std::string Level::getName() const
{
    return name;
}

void Level::setName(const std::string& name)
{
    this->name = name;
}

std::string Level::getBackground() const
{
    return background;
}

void Level::setBackground(const std::string& background)
{
    this->background = background;
}

void Level::add(Coord coord, short code)
{
    items[coord].insert(code);
}

const std::set<short>& Level::get(Coord coord) const
{
    std::map<Coord, std::set<short>>::const_iterator it = items.find(coord);
    
    if(it != items.end()) return it->second;
    else return ZEROSET_S;
}

bool Level::tryDel(Coord coord, short code)
{
    std::map<Coord, std::set<short>>::iterator it1 = items.find(coord);
    if(it1 != items.end())
    {
        for(std::set<short>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++)
        {
            if(*it2 == code)
            {
                it1->second.erase(it2);
                
                return true;
            }
        }
    }

    return false;
}