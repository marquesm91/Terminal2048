//************************************//
// TERMINAL 2048 by Matheus Marques   //
// Console Application for 2048 Game  //
// Date: June'10'2014                 //
// Linux Version                      //
//************************************//

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

// Use default color when init_curses
#define COLOR_DEFAULT -1

// Integer numbers for Arrow Keys
#define K_UP 3
#define K_DOWN 2
#define K_RIGHT 5		
#define K_LEFT 4

#define GAMEOVER 1
#define WIN 2
#define PLAY_AGAIN 3
#define EXIT_GAME 4
#define PLAYING 5

#define TARGET_TILE 2048 // if you have the TARGET_TILE you win the game

static const int SIZEGRID = 4;
static const int max_tiles = SIZEGRID*SIZEGRID;

using namespace std;

class Terminal2048
{
public:

	Terminal2048();
	void update_best_score();
	bool start();

private:

	// Methods for basic instructions
	void print();
	void add_tile();

	// Methods for score
	void score_point(unsigned int value);

	// Methods for change colors
	void choose_color_tile(unsigned biggest_tile);

	// Methods for Moviments
	void move_up();
	void move_down();
	void move_right();
	void move_left();

	// check if you win, lose or still playing
	void check();
	
	// Method for try again and print if you win or loose
	void call(int what);

	// Attributes
	unsigned biggest_tile;
	unsigned tiles_availables;
	unsigned score;
	unsigned best_score;
	unsigned made_something;
	char key_stroke;
	int status_game;
	int previous_random_position_tile;

	// Array of structs for tiles
	struct tile{
		bool exist;
		unsigned value;
	}tile[max_tiles];

};