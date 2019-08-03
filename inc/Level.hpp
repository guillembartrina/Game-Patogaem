
#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <utility>
#include <vector>
#include <set>
#include <map>

typedef std::pair<char, char> Coord;

class Level 
{
  public:

  Level();
  ~Level();

  std::string getName() const;
  void setName(const std::string& name);

  std::string getBackground() const;
  void setBackground(const std::string& background);

  void add(Coord coord, short code);
  const std::set<short>& get(Coord coord) const;
  bool tryDel(Coord coord, short code);

  //static void serialize();
  //static void deserialize();

  private:

  std::string name;
  std::string background;
  std::map<Coord, std::set<short>> items;
  std::vector<Coord> spawns;

};

#endif