#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>

using namespace std;

class Game
{
public:
	Game();
	~Game();
	bool start();
	bool check_gameover();
private:
	//Grid *g;
	//Score *s;
};


#endif /* 2048_H_INCLUDED */