#include "Tile.h"

Tile::Tile()
{
  this->value = 0;
}

Tile::Tile(size_t v)
{
  this->value = v;
}

Tile::Tile(const Tile &t)
{
  this->value = t.value;
}

Tile Tile::operator+ (const Tile &t)
{
  return Tile(this->value + t.value);
}

Tile Tile::operator- (const Tile &t)
{
  return Tile(this->value - t.value);
}

void Tile::operator= (const Tile &t)
{
  this->value = t.value;
}

void Tile::operator+= (const Tile &t)
{
  this->value = this->value + t.value;
}

bool Tile::operator== (const Tile &t)
{
  return this->value == t.value;
}

bool Tile::operator== (size_t v)
{
  return this->value == v;
}

size_t Tile::get() const
{
  return this->value;
}

void Tile::set(size_t v)
{
  this->value = v;
}

Grid::Grid(size_t rows, size_t cols)
  : grid_rows(rows),
    grid_columns(cols),
    grid(rows *cols)
{
  Tile t;
  for (int i = 0; i < rows * cols; i++)
  {
    cout << t.get() << endl;
    grid.push_back(t);
    cout << grid[i].get();
  }
}

size_t Grid::operator()(size_t i, size_t j)
{
  return grid[i * grid_columns + j].get();
}

bool Grid::add(const Tile &t , size_t i, size_t j)
{
  for (int i = 0; i < grid_rows; i++)
  {
    for (int j = 0; j < grid_columns; j++)
      cout << (i, j) << " ";
    cout << "\n";
  }
  //cout << "dentro1 = " << (i, j) << endl;
  //cout << "dentro2 = " << (2, 2) << endl;
  //cout << "dentro3 = " << (3, 2) << endl;

  if ((i, j))
    return false;
  else
  {
    grid[i * grid_columns + j].set(t.get());
    return true;
  }
}