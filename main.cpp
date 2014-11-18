#include "Tile.h"
#include <vector>

int main()
{
  Tile t1(128);
  Tile t2(128);
  Tile t3(256);
  Tile t4(512);
  Tile t5(1024);
  Grid g(4,4);

  cout << g(2,2) << endl;
	g.add(t4,2,2);
  cout << g(1,2) << endl;

  if(t1 == t2)
  	t1 += t2;

  cout << t1.get() << endl;
}