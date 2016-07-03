//************************************//
// TERMINAL 2048 by Matheus Marques   //
// Console Application for 2048 Game  //
// Date: June'10'2014                 //
// Linux Version                      //
//************************************//

#include "t2048_Linux.h"

Terminal2048::Terminal2048()
{
	score = 0;
	made_something = 0;
	tiles_availables = max_tiles;
	tile_2048 = false;
	try_again = false;
	//best_score = get_best_score();

	for (unsigned int i = 0; i < max_tiles; i++)
	{
		tile[i].exist = false;
		tile[i].value = 0;
	}

	addtile();
	addtile();
	refresh();
}

void Terminal2048::clear()
{
	// command for clear the terminal
	// \033[2J means from top (J) to bottom (2)
	// \033[1;1H means back the cursor on 1,1 coordenate
	std::cout << "\033[2J\033[1;1H";
}

char Terminal2048::getkey(void) 
{
  struct termios old, new_;
  char ch;
  
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new_ = old; /* make new settings same as old settings */
  new_.c_lflag &= ~ICANON; /* disable buffered i/o */
  new_.c_lflag &= 0 ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new_); /* use these new terminal i/o settings now */
  
  ch = getchar();
  
  tcsetattr(0, TCSANOW, &old);
  
  return ch;
}

std::string Terminal2048::set_color(colors textcol, colors backcol)
{
	int text = int(textcol);
	int back = int(backcol);
	
	std::string color = "";

	color += "\e[";
	color += std::to_string(text);
	color += "m";

	return color;
}

void Terminal2048::choose_color_for_tile(unsigned tile_number)
{
	std::string color = "";

	switch (tile_number)
	{
	case 0:		color = set_color(white, black); break;
	case 2:		color = set_color(cyan, black); break;
	case 4:		color = set_color(yellow, black); break;
	case 8:		color = set_color(green, black); break;
	case 16:	color = set_color(purple, black); break;
	case 32:	color = set_color(red, black); break;
	case 64:	color = set_color(gray, black); break;
	case 128:	color = set_color(dark_cyan, black); break;
	case 256:	color = set_color(dark_yellow, black); break;
	case 512:	color = set_color(dark_green, black); break;
	case 1024:	color = set_color(dark_purple, black); break;
	case 2048:	color = set_color(dark_red, black); break;
	}

	color += std::to_string(tile_number);

	std::cout << color << tabulation;
}

void Terminal2048::refresh()
{
	clear();
	set_color(white, black);
	std::cout << "SCORE: " << score << std::endl;

	for (unsigned pos = 0; pos < max_tiles; pos++)
	{
		if (pos % 4 == 0) std::cout << std::endl<< std::endl;

		choose_color_for_tile(tile[pos].value);	
	}
}

void Terminal2048::makepoint(unsigned int value)
{
	score += value;
}

void Terminal2048::addtile()
{
	
	if (tiles_availables > 0)
	{
		std::default_random_engine generator((unsigned int)time(0));
		std::uniform_int_distribution<int> distribution_to_position(0, max_tiles - 1);
		std::uniform_int_distribution<int> distribution_to_number(0, 100);

		int pos;

		do{ 
			
			pos = distribution_to_position(generator);

		} while (tile[pos].exist != false);

		(distribution_to_number(generator) < 90) ? (tile[pos].value = 2) : (tile[pos].value = 4);

		tile[pos].exist = true;

		tiles_availables--;
	}
	
}

void Terminal2048::move_up()
{
	int ref;
	int searcher;

	for (int j = 0; j < SIZEGRID; j++)
	{
		ref = j;
		while (tile[ref].exist != true && ref < max_tiles) ref += 4; // search for available tile
		searcher = ref + 4;

		while (ref < max_tiles - SIZEGRID)
		{
			while (tile[searcher].exist != true && searcher < max_tiles) searcher += 4; // search for available tile to add to ref

			if (searcher < max_tiles && tile[searcher].value == tile[ref].value)
			{
				tile[ref].value *= 2;
				tile[searcher].exist = false;
				tile[searcher].value = 0;

				tiles_availables++;
				made_something = 1;

				makepoint(tile[ref].value);
				tile_2048 = check_youwin(tile[ref].value);
			}

			ref = searcher;
			searcher += 4;
		}

		do
		{
			ref = j;
			while (tile[ref].exist != false && ref < max_tiles) ref += 4; // Search for first zero to move up
			searcher = ref + 4;

			while (tile[searcher].exist != true && searcher < max_tiles) searcher += 4; // search for available tile to change with empty

			if (searcher < max_tiles)
			{
				tile[ref].value = tile[searcher].value;
				tile[ref].exist = true;
				tile[searcher].value = 0;
				tile[searcher].exist = false;

				made_something = 1;
			}
			
		}while (searcher < max_tiles);

	}

}

void Terminal2048::move_down()
{
	int ref;
	int searcher;

	for (int j = (max_tiles - SIZEGRID); j < max_tiles; j++)
	{
		ref = j;
		while (tile[ref].exist != true && ref >= 0) ref -= 4; // search for available tile
		searcher = ref - 4;

		while (ref >= SIZEGRID)
		{
			while (tile[searcher].exist != true && searcher >= 0) searcher -= 4; // search for available tile to add to ref

			if (searcher >= 0 && tile[searcher].value == tile[ref].value)
			{
				tile[ref].value *= 2;
				tile[searcher].exist = false;
				tile[searcher].value = 0;

				tiles_availables++;
				made_something = 1;

				makepoint(tile[ref].value);
			}

			ref = searcher;
			searcher -= 4;
		}

		do
		{
			ref = j;
			while (tile[ref].exist != false && ref >= 0) ref -= 4; // Search for first zero to move up
			searcher = ref - 4;

			if(ref >= 0)
				while (tile[searcher].exist != true && searcher >= 0) searcher -= 4; // search for available tile to change with empty

			if (searcher >= 0)
			{
				tile[ref].value = tile[searcher].value;
				tile[ref].exist = true;
				tile[searcher].value = 0;
				tile[searcher].exist = false;

				made_something = 1;
			}

		}while (searcher >= 0);

	}

}

void Terminal2048::move_left()
{
	int ref;
	int searcher;

	for (int j = 0; j < max_tiles; j += 4)
	{
		ref = j;
		while (tile[ref].exist != true && ref < (SIZEGRID + j)) ref++; // search for available tile
		searcher = ref + 1;

		while (ref < (SIZEGRID + j) - 1)
		{
			while (tile[searcher].exist != true && searcher < (SIZEGRID + j)) searcher++; // search for available tile to add to ref

			if (searcher < (SIZEGRID + j) && tile[searcher].value == tile[ref].value)
			{
				tile[ref].value *= 2;
				tile[searcher].exist = false;
				tile[searcher].value = 0;

				tiles_availables++;
				made_something = 1;

				makepoint(tile[ref].value);
			}

			ref = searcher;
			searcher++;
		}

		do
		{
			ref = j;
			while (tile[ref].exist != false && ref < (SIZEGRID + j)) ref++; // Search for first zero to move up
			searcher = ref + 1;

			while (tile[searcher].exist != true && searcher < (SIZEGRID + j)) searcher++; // search for available tile to change with empty

			if (searcher < (SIZEGRID + j))
			{
				tile[ref].value = tile[searcher].value;
				tile[ref].exist = true;
				tile[searcher].value = 0;
				tile[searcher].exist = false;

				made_something = 1;
			}

			
		}while (searcher < (SIZEGRID + j));

	}

}

void Terminal2048::move_right()
{
	int ref;
	int searcher;

	for (int j = 3; j < max_tiles; j += 4)
	{
		ref = j;
		while (tile[ref].exist != true && ref > (j - SIZEGRID)) ref--; // search for available tile
		searcher = ref - 1;

		while (ref > (j - SIZEGRID) + 1)
		{
			while (tile[searcher].exist != true && searcher > (j - SIZEGRID)) searcher--; // search for available tile to add to ref

			if (searcher > (j - SIZEGRID) && tile[searcher].value == tile[ref].value)
			{
				tile[ref].value *= 2;
				tile[searcher].exist = false;
				tile[searcher].value = 0;

				tiles_availables++;
				made_something = 1;

				makepoint(tile[ref].value);
			}

			ref = searcher;
			searcher--;
		}

		do
		{
			ref = j;
			while (tile[ref].exist != false && ref >= ((j - SIZEGRID) + 1)) ref--; // Search for first zero to move up
			searcher = ref - 1;

			while (tile[searcher].exist != true && searcher >= ((j - SIZEGRID) + 1)) searcher--; // search for available tile to change with empty

			if (searcher >= ((j - SIZEGRID) + 1))
			{
				tile[ref].value = tile[searcher].value;
				tile[ref].exist = true;
				tile[searcher].value = 0;
				tile[searcher].exist = false;

				made_something = 1;
			}

		}while (searcher >= ((j - SIZEGRID) + 1));

	}

}

bool Terminal2048::check_gameover()
{
	int ref = 0;
	int comparator = 0;
	int col = 0;
	bool checker = false;

	// check horizontal
	for (ref = 0, comparator = 0; comparator < max_tiles; ref++)
	{
		comparator = ref + 1;
		if ( comparator % 4 == 0)
		{
			ref++;
			comparator++;
		}

		if (tile[ref].value == tile[comparator].value) 
		{
			checker = false;
			return checker;
		}
		else
		{
			checker = true;
		}

	}

	// check vertical
	for (ref = 0, col = 0, comparator = 0; col < SIZEGRID; ref += 4)
	{
		if ((comparator = ref + 4) > max_tiles)
		{	
			col++;
			ref = col;
			comparator = ref + 4;
		}

		
		if ((tile[ref].value == tile[comparator].value) && col < SIZEGRID)
		{
			checker = false;
			return checker;
		}
		else
			checker = true;
	}

	return checker;
}

bool Terminal2048::call_gameover()
{
	set_color(white, black);
	std::cout << std::endl << std::endl << "GAME OVER!" << std::endl <<"Try again?(y/n) ";

	return call_tryagain();
}

bool Terminal2048::check_youwin(unsigned number_tile)
{
	if (number_tile == 2048)
		return true;
	else
		return false;
}

bool Terminal2048::call_youwin()
{
	set_color(white, black);
	std::cout << std::endl << std::endl << "YOU WIN!" << std::endl << "Try again?(y/n) ";

	return call_tryagain();
}

bool Terminal2048::call_tryagain()
{
	do{

		key_stroke = getkey();

	} while (key_stroke != NO && key_stroke != YES);

	if (key_stroke == YES)
		return true;
	else
		return false;
}

bool Terminal2048::start()
{

	while (true)
	{

		if (made_something)
		{
			addtile();
			refresh();
		}
		
		if (!tiles_availables)
			if (check_gameover())
				return call_gameover();

		if (tile_2048)
			return call_youwin();

		key_stroke = getkey();

		made_something = 0;

		if(key_stroke == '\033'){
			getkey();
			switch (key_stroke =getkey())
			{
				case 'A': move_up(); break;
				case 'B': move_down(); break;
				case 'C': move_right(); break;
				case 'D': move_left(); break;
				//default: std::cout << (int)key_stroke << std::endl;
			}
		}

		fflush ( stdin );
	}

}

// Main Program
/*int main(void)
{
	bool keep_playing;

	initscr(); // Initialize Ncurses

	start_color(); // Initialize colors in Ncurses

	do{

		Terminal2048 *g(new Terminal2048);
		keep_playing = g->start();

	} while (keep_playing == true);

	return 0;
}*/

// NCurses tests main
typedef struct _win_border_struct {
	chtype 	left, right, top, under, 
	 	topleft, topright, underleft, underright;
}WIN_BORDER;

typedef struct _WIN_struct {

	int startx, starty;
	int height, width;
	WIN_BORDER border;
}WIN;

void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);

int main(int argc, char *argv[])
{	WIN win;
	int ch;

	initscr();								// Start curses mode
	start_color();							// Start the color functionality
	cbreak();								// Line buffering disabled

	curs_set(0);							// hide cursor console
	keypad(stdscr, TRUE);					// For Arrow Keys
	noecho();								// disable echo() in getch()
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	// Initialize the window parameters
	init_win_params(&win);
	print_win_params(&win);

	attron(COLOR_PAIR(1));
	printw("Press E to exit");
	refresh();
	attroff(COLOR_PAIR(1));
	
	create_box(&win, TRUE);
	while((ch = getch()) != 'E')
	{	switch(ch)
		{	case KEY_LEFT:
				create_box(&win, FALSE);
				--win.startx;
				create_box(&win, TRUE);
				break;
			case KEY_RIGHT:
				create_box(&win, FALSE);
				++win.startx;
				create_box(&win, TRUE);
				break;
			case KEY_UP:
				create_box(&win, FALSE);
				--win.starty;
				create_box(&win, TRUE);
				break;
			case KEY_DOWN:
				create_box(&win, FALSE);
				++win.starty;
				create_box(&win, TRUE);
				break;	
		}
	}
	endwin();			/* End curses mode		  */
	return 0;
}
void init_win_params(WIN *p_win)
{
	p_win->height = 2;
	p_win->width  = 3;

	// LINES and COLS are set with the size screen when initscr() is called.
	//p_win->starty = (LINES - p_win->height)/2;	
	//p_win->startx = (COLS - p_win->width)/2;
	p_win->starty = 1;
	p_win->startx = 1;

	p_win->border.left       = '|';
	p_win->border.right      = '|';
	p_win->border.top        = '-';
	p_win->border.under      = '-';
	p_win->border.topleft    = '+';
	p_win->border.topright   = '+';
	p_win->border.underleft  = '+';
	p_win->border.underright = '+';

}
void print_win_params(WIN *p_win)
{
#ifdef _DEBUG
	mvprintw(25, 0, "%d %d %d %d", p_win->startx, p_win->starty, 
				p_win->width, p_win->height);
	refresh();
#endif
}
void create_box(WIN *p_win, bool flag)
{	int i, j;
	int x, y, w, h;

	x = p_win->startx;
	y = p_win->starty;
	w = p_win->width;
	h = p_win->height;

	if(flag == TRUE)
	{	mvaddch(y, x, p_win->border.topleft);
		mvaddch(y, x + w, p_win->border.topright);
		mvaddch(y + h, x, p_win->border.underleft);
		mvaddch(y + h, x + w, p_win->border.underright);
		mvhline(y, x + 1, p_win->border.top, w - 1);
		mvhline(y + h, x + 1, p_win->border.under, w - 1);
		mvvline(y + 1, x, p_win->border.left, h - 1);
		mvvline(y + 1, x + w, p_win->border.right, h - 1);

	}
	else
		for(j = y; j <= y + h; ++j)
			for(i = x; i <= x + w; ++i)
				mvaddch(j, i, ' ');
				
	refresh();

}