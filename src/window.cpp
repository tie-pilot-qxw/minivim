#include "../include/window.h"
int window::windowNum = 0;
int window::colorNum = 0;
window::window() {
    if(windowNum == 0) {
        initscr(); /* Start curses mode */
        raw();
        noecho();
        keypad(stdscr, true); /*stdscr is the regular window*/
        
        // check window size is illegal
        // LINES and COLS store the total line and cols of stdscr
        if (LINES < 5) {
            fprintf(stderr, "window line size is small than 5");
            endwin();
            exit(1);
        }

        // init color
        start_color();
        init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
        colorNum++;

        // set stdscr color
        wbkgd(stdscr, COLOR_PAIR(DEFAULT_COLOR));
        wrefresh(stdscr);
    }
}