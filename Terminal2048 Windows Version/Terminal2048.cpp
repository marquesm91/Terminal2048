//************************************//
// TERMINAL 2048 by Matheus Marques	  //
// Console Application for 2048 Game  //
// Date: June'10'2014				  //
// Windows Version					  //
//************************************//

#include "Terminal2048.h"

Terminal2048::Terminal2048()
{
	score = 0;
	made_something = 0;
	tiles_availables = max_tiles;
	tile_2048 = false;
	try_again = false;

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
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

DWORD Terminal2048::getkey()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD dwEventsRead;
	DWORD ch;
	BOOL PressDown = true;

	while (ReadConsoleInputA(hStdin, &InputRecord, 1, &dwEventsRead)) /* wait for pressed key */
	if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown == PressDown)
	{
		ch = InputRecord.Event.KeyEvent.wVirtualKeyCode;
		return ch;
	}

	return EOF;
}

void Terminal2048::set_color(colors textcol, colors backcol)
{
	HANDLE hStdin = GetStdHandle(STD_OUTPUT_HANDLE);
	int text = int(textcol);
	int back = int(backcol);
	
	text %= 16; 
	back %= 16;

	unsigned short wAttributes = ((unsigned)back << 4) | (unsigned)text;

	SetConsoleTextAttribute(hStdin, wAttributes);
}

void Terminal2048::choose_color_for_tile(unsigned tile_number)
{
	switch (tile_number)
	{
	case 0:		set_color(white, black); break;
	case 2:		set_color(cyan, black); break;
	case 4:		set_color(yellow, black); break;
	case 8:		set_color(green, black); break;
	case 16:	set_color(purple, black); break;
	case 32:	set_color(red, black); break;
	case 64:	set_color(gray, black); break;
	case 128:	set_color(dark_cyan, black); break;
	case 256:	set_color(dark_yellow, black); break;
	case 512:	set_color(dark_green, black); break;
	case 1024:	set_color(dark_purple, black); break;
	case 2048:	set_color(dark_red, black); break;
	}
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

		std::cout <<tile[pos].value << tabulation;
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
		
		switch (key_stroke)
		{
			case VK_UP: move_up(); break;
			case VK_LEFT: move_left(); break;
			case VK_DOWN: move_down(); break;
			case VK_RIGHT: move_right(); break;
			case VK_ESCAPE: exit(-1);
			//default: std::cout << (int)key_stroke << std::endl;
		}
		
	}

}

int main(void)
{
	bool keep_playing;

	do{

		std::auto_ptr<Terminal2048> g(new Terminal2048);
		keep_playing = g->start();

	} while (keep_playing == true);

	return 0;
}