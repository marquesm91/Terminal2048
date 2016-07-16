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

#define RESETCOLOR "echo -e \033[0m"
#define YES 89
#define NO 78
#define COLOR_DEFAULT -1

// Integer numbers for Arrow Keys
#define K_UP 3
#define K_DOWN 2
#define K_RIGHT 5		
#define K_LEFT 4

// Extra colors
#define COLOR_GRAY 8
#define COLOR_DARK_CYAN 9
#define COLOR_DARK_YELLOW 10
#define COLOR_DARK_GREEN 11
#define COLOR_DARK_MAGENTA 12
#define COLOR_DARK_RED 13

#define GAMEOVER 1
#define WIN 2
#define PLAY_AGAIN 3
#define EXIT_GAME 4
#define PLAYING 5

using namespace std;

static const char tabulation = 9;
static const int SIZEGRID = 4;
static const int max_tiles = SIZEGRID*SIZEGRID;

enum colors
{
	black = 49, //default background
	dark_blue = 34,
	dark_green = 32,
	dark_aqua, dark_cyan = 36,
	dark_red = 31,
	dark_purple = 35, dark_pink = 35, dark_magenta = 35,
	dark_yellow = 33,
	dark_white = 97,
	gray = 37,
	blue = 94,
	green = 92,
	aqua = 96, cyan = 96,
	red = 91,
	purple = 95, pink = 95, magenta = 95,
	yellow = 93,
	white = 97
};

class Terminal2048
{
public:

	Terminal2048();
	void update_best_score();
	bool start();

private:

	// Methods for basic instructions
	void print();
	void addtile();

	// Methods for score
	void makepoint(unsigned int value);

	// Methods for change colors
	void choose_color_tile(unsigned biggest_tile);

	// Methods for Moviments
	void move_up();
	void move_down();
	void move_right();
	void move_left();

	// Methods for possible looses
	//bool check_gameover();
	//bool call_gameover();

	// Methods for possible wins
	void check();
	//bool call_youwin();
	
	// Method for try again
	void call(int what);

	// Attributes
	unsigned biggest_tile;
	unsigned tiles_availables;
	bool try_again;
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