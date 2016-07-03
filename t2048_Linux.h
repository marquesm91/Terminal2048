//************************************//
// TERMINAL 2048 by Matheus Marques   //
// Console Application for 2048 Game  //
// Date: June'10'2014                 //
// Linux Version                      //
//************************************//

#include <iostream>
#include <thread>
#include <random>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <ncurses.h>

#define YES 89
#define NO 78

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
	bool start();

private:

	// Methods for basic instructions
	void clear();
	void refresh();
	void addtile();
	char getkey();

	// Methods for score
	void makepoint(unsigned int value);
	int get_best_score();

	// Methods for change colors
	std::string set_color(colors textcol, colors backcol);
	void choose_color_for_tile(unsigned number_tile);
	
	// Methods for Moviments
	void move_up();
	void move_down();
	void move_right();
	void move_left();

	// Methods for possible looses
	bool check_gameover();
	bool call_gameover();

	// Methods for possible wins
	bool check_youwin(unsigned number_tile);
	bool call_youwin();
	
	// Method for try again
	bool call_tryagain();

	// Attributes
	unsigned tile_2048;
	unsigned tiles_availables;
	bool try_again;
	unsigned score;
	unsigned best_score;
	unsigned made_something;
	char key_stroke;

	// Array of structs for tiles
	struct tile{
		bool exist;
		unsigned value;
	}tile[max_tiles];

};