#include "window.h"
int window :: windowNum = 0;
int window :: colorNum = 0;
bool window :: change = false;
int window :: state = NORMAL;
std :: pair<int, int> window :: windowWidth = std :: make_pair(0, 0);

window :: window() {
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

        updateWindowWith();
    }
    windowNum++;
    hasChange = false;
}


window :: ~window() {
    windowNum--;
    delwin(win);
    if (windowNum == 0) {
        endwin(); /* End curses mode */
    }
}

void window :: updateWindowWith() {
    std :: pair <int, int> newpos;
    getmaxyx(stdscr, newpos.first, newpos.second);
    if(newpos == windowWidth) return;
    windowWidth = newpos;
    change^=1;
}

int window :: getState() {
    return state;
}

void window :: updateMouse() {
    getyx(win, mousePos.first, mousePos.second);
}
