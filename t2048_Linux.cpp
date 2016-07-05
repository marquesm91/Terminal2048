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
	rrefresh();
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

void Terminal2048::choose_color_tile(unsigned tile_number)
{

	switch (tile_number)
	{
	/*case 0:		color = set_color(white, black); break;
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
	case 2048:	color = set_color(dark_red, black); break;*/
	case 0:		attron(COLOR_PAIR(1)); break;
	case 2:		attron(COLOR_PAIR(2)); break;
	case 4:		attron(COLOR_PAIR(3)); break;
	case 8:		attron(COLOR_PAIR(4)); break;
	case 16:	attron(COLOR_PAIR(5)); break;
	case 32:	attron(COLOR_PAIR(6)); break;
	case 64:	attron(COLOR_PAIR(7)); break;
	case 128:	attron(COLOR_PAIR(8)); break;
	case 256:	attron(COLOR_PAIR(9)); break;
	case 512:	attron(COLOR_PAIR(10)); break;
	case 1024:	attron(COLOR_PAIR(11)); break;
	case 2048:	attron(COLOR_PAIR(12)); break;
	}
}

void Terminal2048::rrefresh()
{
	attron(COLOR_PAIR(1));
	mvprintw(1, 1, "SCORE: %d", score);

	unsigned v_space = 0; // vertical space
	unsigned h_space = 0; // horizontal space
	unsigned start_posx = (LINES/2)-(max_tiles/4); // start position x for drawing
	unsigned start_posy = (COLS/2)-(max_tiles/2); // start position y for drawing

	for(unsigned j = 0; j < SIZEGRID; j++, v_space += 2)
	{
		h_space = 0;

		for(unsigned i = 0; i < SIZEGRID; i++, h_space += 4)
		{
			unsigned pos = j*SIZEGRID + i;

			mvhline(j + start_posx + v_space,
						  i + start_posy + h_space,
						  ' ',
						  i + (h_space - 1) + start_posy + h_space);

			choose_color_tile(tile[pos].value);

			mvprintw(j + start_posx + v_space, // x position
							 i + start_posy + h_space, // y position
							 "%d",										 // what to draw
							 tile[pos].value);				 // what var to draw
		}
	}
	refresh();
}

void Terminal2048::makepoint(unsigned int value)
{
	score += value;
}

void Terminal2048::addtile()
{
	
	if (tiles_availables > 0)
	{
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution_for_position(0, max_tiles - 1);
		std::uniform_int_distribution<int> distribution_for_number(0, 100);

		int pos;

		do{ 
			
			pos = distribution_for_position(generator);

		} while (tile[pos].exist != false);

		(distribution_for_number(generator) < 90) ? (tile[pos].value = 2) : (tile[pos].value = 4);

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
			rrefresh();
		}
		
		if (!tiles_availables)
			if (check_gameover())
				return call_gameover();

		if (tile_2048)
			return call_youwin();

		key_stroke = getch();

		made_something = 0;
		
		switch ((int)key_stroke)
		{
			case K_UP: 		move_up(); 		break;
			case K_DOWN:  move_down();	break;
			case K_RIGHT: move_right();	break;
			case K_LEFT:  move_left();	break;
			//default: std::cout << (int)key_stroke << std::endl;
		}
	}
}

void setup_colors();

// Main Program
int main(void)
{
	bool keep_playing;
	char ch;

	initscr();								// Start curses mode
	start_color();							// Start the color functionality
	cbreak();								// Line buffering disabled
	use_default_colors();
	curs_set(0);							// hide cursor console
	keypad(stdscr, TRUE);					// For Arrow Keys
	noecho();								// disable echo() in getch()

	setup_colors(); 

	do{

		Terminal2048 *g(new Terminal2048);
		keep_playing = g->start();

	} while (keep_playing == true);

	/*attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
	attroff(COLOR_PAIR(3));
	attroff(COLOR_PAIR(4));
	attroff(COLOR_PAIR(5));
	attroff(COLOR_PAIR(6));
	attroff(COLOR_PAIR(7));
	attroff(COLOR_PAIR(8));
	attroff(COLOR_PAIR(9));
	attroff(COLOR_PAIR(10));
	attroff(COLOR_PAIR(11));	
	attroff(COLOR_PAIR(12));*/
	standend();
	refresh();
	endwin();			// End curses mode
	return 0;
}

void setup_colors()
{
	unsigned R, G, B;

	// 1 - WHITE
	R = 255; G = 255; B = 255;
	init_color(COLOR_WHITE, R*1000/255, G*1000/255, B*1000/255);

	// 2 - CYAN
	R = 51; G = 255; B = 255;
	init_color(COLOR_CYAN, R*1000/255, G*1000/255, B*1000/255);

	// 3 - YELLOW
	R = 255; G = 255; B = 51;
	init_color(COLOR_YELLOW, R*1000/255, G*1000/255, B*1000/255);

	// 4 - GREEN
	R = 51; G = 255; B = 51;
	init_color(COLOR_GREEN, R*1000/255, G*1000/255, B*1000/255);

	// 5 - MAGENTA
	R = 255; G = 51; B = 255;
	init_color(COLOR_MAGENTA, R*1000/255, G*1000/255, B*1000/255);

	// 6 - RED
	R = 255; G = 51; B = 51;
	init_color(COLOR_RED, R*1000/255, G*1000/255, B*1000/255);

	// 7 - GRAY
	R = 160; G = 160; B = 160;
	init_color(COLOR_GRAY, R*1000/255, G*1000/255, B*1000/255);

	// 8 - DARK_CYAN
	R = 0; G = 102; B = 102;
	init_color(COLOR_DARK_CYAN, R*1000/255, G*1000/255, B*1000/255);

	// 9 - DARK_YELLOW
	R = 102; G = 102; B = 0;
	init_color(COLOR_DARK_YELLOW, R*1000/255, G*1000/255, B*1000/255);

	// 10 - DARK_GREEN
	R = 0; G = 102; B = 0;
	init_color(COLOR_DARK_GREEN, R*1000/255, G*1000/255, B*1000/255);

	// 11 - DARK_MAGENTA
	R = 102; G = 0; B = 102;
	init_color(COLOR_DARK_MAGENTA, R*1000/255, G*1000/255, B*1000/255);

	// 12 - DARK_RED
	R = 102; G = 0; B = 0;
	init_color(COLOR_DARK_RED, R*1000/255, G*1000/255, B*1000/255);

	init_pair(1, COLOR_WHITE, COLOR_DEFAULT); // 0
	init_pair(2, COLOR_CYAN, COLOR_DEFAULT); // 2
	init_pair(3, COLOR_YELLOW, COLOR_DEFAULT); // 4
	init_pair(4, COLOR_GREEN, COLOR_DEFAULT); // 8
	init_pair(5, COLOR_MAGENTA, COLOR_DEFAULT); // 16
	init_pair(6, COLOR_RED, COLOR_DEFAULT); // 32
	init_pair(7, COLOR_GRAY, COLOR_DEFAULT); // 64
	init_pair(8, COLOR_DARK_CYAN, COLOR_DEFAULT); // 128
	init_pair(9, COLOR_DARK_YELLOW, COLOR_DEFAULT); // 256
	init_pair(10, COLOR_DARK_GREEN, COLOR_DEFAULT); // 512
	init_pair(11, COLOR_DARK_MAGENTA, COLOR_DEFAULT); // 1024
	init_pair(12, COLOR_DARK_RED, COLOR_DEFAULT); // 2048
}
// NCurses tests main
/*
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
	endwin();			// End curses mode		  
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

}*/