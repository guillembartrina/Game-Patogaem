
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>
#include <utility>
#include <map>

class Level 
{
  public:
  std::string name;
  std::string background;
  std::map<std::pair<char, char>,std::vector<short>> items;
};

#endif