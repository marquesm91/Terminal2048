#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

class Tile
{
public:
  Tile();
  Tile(size_t);
  Tile(const Tile &);

  size_t get() const;
  void set(size_t);

  Tile operator+ (const Tile &);
  Tile operator- (const Tile &);
  void operator= (const Tile &);
  void operator+= (const Tile &);
  bool operator== (const Tile &);
  bool operator== (size_t);

private:
  size_t value;
};


class Grid
{
public:
  Grid(size_t, size_t);

  bool add(const Tile &, size_t, size_t);
  size_t operator()(size_t, size_t);

private:
  size_t grid_rows;
  size_t grid_columns;
  vector<Tile> grid;
};


#endif /* 2048_H_INCLUDED */