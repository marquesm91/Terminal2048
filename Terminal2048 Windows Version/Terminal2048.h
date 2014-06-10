//************************************//
// TERMINAL 2048 by Matheus Marques   //
// Console Application for 2048 Game  //
// Date: June'10'2014                 //
// Windows Version                    //
//************************************//

#include <iostream>
#include <thread>
#include <random>
#include <windows.h>

#define YES 89
#define NO 78

static const char tabulation = 9;
static const int SIZEGRID = 4;
static const int max_tiles = SIZEGRID*SIZEGRID;

enum colors
{
	black = 0,
	dark_blue = 1,
	dark_green = 2,
	dark_aqua, dark_cyan = 3,
	dark_red = 4,
	dark_purple = 5, dark_pink = 5, dark_magenta = 5,
	dark_yellow = 6,
	dark_white = 7,
	gray = 8,
	blue = 9,
	green = 10,
	aqua = 11, cyan = 11,
	red = 12,
	purple = 13, pink = 13, magenta = 13,
	yellow = 14,
	white = 15
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
	void makepoint(unsigned int value);
	DWORD getkey();

	// Methods for change colors
	void set_color(colors textcol, colors backcol);
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
	unsigned made_something;
	DWORD key_stroke;

	// Array of structs for tiles
	struct tile{
		bool exist;
		unsigned value;
	}tile[max_tiles];

};