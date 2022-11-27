#include "window.h"
#include <fstream>
int window :: windowNum = 0;
int window :: colorNum = 0;
bool window :: change = false;
int window :: mode = NORMAL;
POS window :: consoleSize = std :: make_pair(0, 0);

window :: window() {
    freopen("error.txt", "w", stderr);
    if (windowNum == 0) {
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

        //get the input without delay
        nodelay(stdscr, true);

        updateConsoleSize();
    }
    windowNum++;
    hasChange = false;
}


window :: ~window() {
    windowNum--;
    delwin(win);
    if (windowNum == 0) {
        fclose(ferr);
        endwin(); /* End curses mode */
    }
}

void window :: updateConsoleSize() {
    POS newsize;
    getmaxyx(stdscr, newsize.first, newsize.second);
    if(newsize == consoleSize) return;
    consoleSize = newsize;
    change^=1;
}

int window :: getMode() {
    return mode;
}

void window :: updateMouse() {
    getyx(win, mousePos.first, mousePos.second);
}

void window :: updateStartPos() {
    startPos = std :: make_pair(0, 0);
}