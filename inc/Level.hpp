
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
  bool isEmpty(Coord coord) const;
  const std::set<unsigned short>& get(Coord coord) const;
  bool tryDel(Coord coord, short code);

  void serialize() const;
  void deserialize(unsigned int index);

  private:

  std::string name;
  std::string background;
  std::map<Coord, std::set<unsigned short>> items;
  std::vector<Coord> spawns;

};

#endif