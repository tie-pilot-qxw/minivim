#include "window.h"
#include <fstream>
#include <cstdlib>
int window::windowNum = 0;
int window::change = 0;
int window::mode = NORMAL;
POS window::consoleSize = std::make_pair(0, 0);
bool window::hasSave = true;

window::window() {
    if (windowNum == 0) {
        initscr(); /* Start curses mode */
        raw();
        noecho();
        keypad(stdscr, true); /*stdscr is the regular window*/
        
        // check window size is illegal
        // LINES and COLS store the total line and cols of stdscr
        if (LINES < 5) {
            endwin();
            fprintf(stderr, "window line size is small than 5");

            exit(1);
        }

        // init color
        start_color();
        init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);

        // set stdscr color
        wbkgd(stdscr, COLOR_PAIR(DEFAULT_COLOR));
        wrefresh(stdscr);

        //get the input without delay
        nodelay(stdscr, true);

        updateConsoleSize();
    }
    windowNum++;
    hasChange = 0;
}


window::~window() {
    windowNum--;
    delwin(win);
    if (windowNum == 0) {
        if (!isendwin()) endwin(); /* End curses mode */
    }
}

void window::updateConsoleSize() {
    POS newsize;
    getmaxyx(stdscr, newsize.first, newsize.second);
    if(newsize == consoleSize) return;
    consoleSize = newsize;
    change++;
}

int window::getMode() {
    return mode;
}

void window::updateStartPos() {
    startPos = std::make_pair(0, 0);
}

bool window::updateWindow() {
    if (hasChange == change) return false;
    hasChange = change;
    werase(win);
    updateWindowSize();
    updateStartPos();
    wresize(win, windowSize.first, windowSize.second);
    mvwin(win, startPos.first, startPos.second);
    print();
    return true;
}