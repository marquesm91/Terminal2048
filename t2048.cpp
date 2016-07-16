//************************************//
// TERMINAL 2048 by Matheus Marques   //
// Console Application for 2048 Game  //
// Date: June'10'2014                 //
// Linux Version                      //
//************************************//

#include "t2048.h"

Terminal2048 *g(new Terminal2048());

Terminal2048::Terminal2048()
{
	score = 0; 													// Initial current score
	made_something = 1; 								// to add_tiles and print what change
	tiles_availables = max_tiles; 			// available tiles at the game start is always the size board (or max_tiles)
	biggest_tile = 0; 									// biggest tile at the game start is always 0
	status_game = PLAYING; 							// always start the game as PLAYING
	previous_random_position_tile = -1; // help to improve random algorithm

	update_best_score();

	// Initilize every tile with 0 and fale existance
	for (unsigned int i = 0; i < max_tiles; i++)
	{
		tile[i].exist = false;
		tile[i].value = 0;
	}

	add_tile();
}

void Terminal2048::update_best_score()
{

	std::ifstream infile("score.txt");

	if(infile.is_open())
	{
		infile >> best_score;
		infile.close();
	}
	else
		best_score = 0;

	if(score > best_score)
	{
		std::ofstream otfile("score.txt");
		otfile << score;
		otfile.close();
	}
}

void Terminal2048::choose_color_tile(unsigned tile_number)
{
	switch (tile_number)
	{
		case 0:		attron(A_BOLD); attron(COLOR_PAIR(1)); break;
		case 2:		attron(A_BOLD); attron(COLOR_PAIR(2)); break;
		case 4:		attron(A_BOLD); attron(COLOR_PAIR(3)); break;
		case 8:		attron(A_BOLD); attron(COLOR_PAIR(4)); break;
		case 16:	attron(A_BOLD); attron(COLOR_PAIR(5)); break;
		case 32:	attron(A_BOLD); attron(COLOR_PAIR(6)); break;
		case 64:	attron(A_BOLD); attron(COLOR_PAIR(7)); break;
		case 128:	attroff(A_BOLD); attron(COLOR_PAIR(2)); break;
		case 256:	attroff(A_BOLD); attron(COLOR_PAIR(3)); break;
		case 512:	attroff(A_BOLD); attron(COLOR_PAIR(4)); break;
		case 1024: 	attroff(A_BOLD); attron(COLOR_PAIR(5)); break;
		case 2048: 	attroff(A_BOLD); attron(COLOR_PAIR(6)); break;
	}
}

void Terminal2048::print()
{
	if (made_something)
	{
		attron(COLOR_PAIR(1));
		attron(A_BOLD);
		mvprintw(1, 1, "SCORE: %d", score);
		mvprintw(2, 1, "BEST:  %d", score > best_score ? score : best_score);

		unsigned v_space = 0; 												 // vertical space
		unsigned h_space = 0; 												 // horizontal space
		unsigned start_posx = (LINES/2)-(max_tiles/4); // start position x for drawing
		unsigned start_posy = (COLS/2)-(max_tiles/2);  // start position y for drawing

		for(unsigned j = 0; j < SIZEGRID; j++, v_space += 2)
		{
			h_space = 0;

			for(unsigned i = 0; i < SIZEGRID; i++, h_space += 4)
			{
				unsigned pos = j*SIZEGRID + i;

				// Clear spaces between numbers with ' '
				mvhline(j + start_posx + v_space,
							  i + start_posy + h_space,
							  ' ',
							  i + (h_space - 1) + start_posy + h_space);

				// Get the appropriated color based on tile's value
				choose_color_tile(tile[pos].value);

				// Put the number in the correct position
				mvprintw(j + start_posx + v_space, // x position
								 i + start_posy + h_space, // y position
								 "%d",										 // what to draw
								 tile[pos].value);				 // what var to draw
			}
		}

		refresh(); // print what change in relation to old screen
	}
}

void Terminal2048::score_point(unsigned int value)
{
	score += value;
	if (value > biggest_tile)
		biggest_tile = value;
}

void Terminal2048::add_tile()
{
	
	if (made_something == 1 && tiles_availables > 0)
	{
		unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<int> distribution_for_position(0, max_tiles - 1);
		std::uniform_int_distribution<int> distribution_for_2_or_4(0, 100);

		int pos;

		do{ 
			
			pos = distribution_for_position(generator);

		} while ((tile[pos].exist == true) || (tiles_availables >= 4 && previous_random_position_tile == pos));

		previous_random_position_tile = pos;

		(distribution_for_2_or_4(generator) < 90) ? (tile[pos].value = 2) : (tile[pos].value = 4);

		tile[pos].exist = true;

		tiles_availables--;
	}	
}

void Terminal2048::move_up()
{
	made_something = 0;
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

				score_point(tile[ref].value);
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
	made_something = 0;
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

				score_point(tile[ref].value);
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
	made_something = 0;
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

				score_point(tile[ref].value);
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
	made_something = 0;
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

				score_point(tile[ref].value);
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

void Terminal2048::check()
{
		int ref = 0; // reference
		int target = 0; // target to evaluate
		int col = 0; // column
		bool checker = false;

		//check if won
		if (biggest_tile == TARGET_TILE)
			call(WIN);
		else
		// If no more free moves
		if(tiles_availables == 0)
		{
			// check horizontal
			for (ref = 0, target = 0; target < max_tiles; ref++)
			{
				target = ref + 1;
				if ( target % 4 == 0)
				{
					ref++;
					target++;
				}

				if (tile[ref].value == tile[target].value) 
				{
					checker = false;
					break;
				}
				else
				{
					checker = true;
				}

			}

			// If can't move horizontal
			if(checker)
			{
				// check vertical
				for (ref = 0, col = 0, target = 0; col < SIZEGRID; ref += SIZEGRID)
				{
					if ((target = ref + 4) > max_tiles)
					{	
						col++;
						ref = col;
						target = ref + 4;
					}

					
					if ((tile[ref].value == tile[target].value) && col < SIZEGRID)
					{
						checker = false;
						break;
					}
					else
						checker = true;
				}

				// If can't move horizontal and vertical
				if(checker)
					call(GAMEOVER);
			}
		}
		else
			status_game = PLAYING;
}

void Terminal2048::call(int what)
{
	attron(A_BOLD);
	attron(COLOR_PAIR(1));

	switch(what)
	{
		case GAMEOVER: mvprintw(LINES-1, 1, "Game over! Try Again? (Y/n)");
		case WIN:			 mvprintw(LINES-1, 1, "You got a 2048 tile! Try Again? (Y/n)");
	}
	
	refresh();

	do{

		key_stroke = getch();
		key_stroke = toupper(key_stroke);

	} while (key_stroke != 'Y' && key_stroke != 'N' && (int)key_stroke != 13);

	if (key_stroke == 'Y' || (int)key_stroke == 13)
		status_game = PLAY_AGAIN;
	else
		status_game = EXIT_GAME;
}

bool Terminal2048::start()
{

	while (true)
	{
		
		add_tile(); // add new tiles to the game
		print(); 	 // print the new tiles
		check(); 	 // check if win or game over (could change status_game)
		
		if(status_game != PLAYING)
		{
			if (status_game == PLAY_AGAIN)
				return true;
			else if (status_game == EXIT_GAME)
				return false;
		}

		// Get only arrow keys
		do{
			
			key_stroke = getch();

		}while((int)key_stroke != K_UP && 
					 (int)key_stroke != K_DOWN &&
					 (int)key_stroke != K_RIGHT && 
					 (int)key_stroke != K_LEFT);

		switch ((int)key_stroke)
		{
			case K_UP: 		move_up(); 	  break;
			case K_DOWN:  move_down();	break;
			case K_RIGHT: move_right();	break;
			case K_LEFT:  move_left();	break;
		}
	}
}

void CtrlC_Event(int s){
    
	attron(COLOR_PAIR(0)); // attach the default fore and background colors again at the terminal
  refresh(); 						 // refresh the colors
  g->update_best_score();
  delete g;
  endwin();							 // End curses mode
  exit(1); 							 // Exit with code 1 (Ctrl+C event)
}

void init_ncurses()
{
	initscr();																	// Start curses mode
	start_color();															// Start the color functionality
	cbreak();																		// Line buffering disabled
	use_default_colors();												// Use background color default
	curs_set(0);																// hide cursor console
	keypad(stdscr, TRUE);												// For Arrow Keys
	noecho();																		// disable echo() in getch()

	// Set up Colors
	init_pair(1, COLOR_WHITE, COLOR_DEFAULT); 	// 0 and 128(bold)
	init_pair(2, COLOR_CYAN, COLOR_DEFAULT); 		// 2 and 256(bold)
	init_pair(3, COLOR_YELLOW, COLOR_DEFAULT); 	// 4 and 512(bold)
 	init_pair(4, COLOR_GREEN, COLOR_DEFAULT); 	// 8 and 1024(bold)
	init_pair(5, COLOR_MAGENTA, COLOR_DEFAULT); // 16 and 2048(bold)
	init_pair(6, COLOR_RED, COLOR_DEFAULT); 		// 32 and 4096(bold)
	init_pair(7, COLOR_BLUE, COLOR_DEFAULT); 		// 64 and 8192(bold)
}

// Main Program
int main(void)
{
	// Calls ctrlc_event when we get CTRL + C event
	struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = CtrlC_Event;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);
	init_ncurses();

  bool keep_playing;

	do{
		erase(); // clear screen
		g->update_best_score();
		keep_playing = g->start();
	} while (keep_playing == true);

	delete g;

	endwin();	// End curses mode
	
	return 0; // End program with success
}