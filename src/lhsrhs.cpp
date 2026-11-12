

#include <curses.h>
#include <cstring>
#include <stdlib.h>
#include "seqEditor.h"


int main() {
	int c=0;
	seqEditor theEditor; 
	theEditor.init(); 
	
	initscr();
	raw();     // block ctrl chars; treat as just keypresses	
	noecho();	 		
	timeout(50);  // unit is ms; 50=20Hz
	keypad(stdscr, TRUE);
	start_color(); 
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLUE);
	init_pair(5, COLOR_GREEN, COLOR_BLUE);
	init_pair(6, COLOR_YELLOW, COLOR_BLUE);
	init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	init_pair(8, COLOR_BLACK, COLOR_GREEN);

	while( (c = getch()) && (theEditor.keepOn) ) {
		if (c!=-1) { theEditor.keyPressed(c); }
		theEditor.draw(stdscr);
		move(theEditor.caretY, theEditor.caretX);
	}
	theEditor.abort(0,true);
	endwin();

}



 