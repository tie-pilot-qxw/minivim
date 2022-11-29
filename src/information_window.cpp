#include "information_window.h"

information_window :: information_window() {

    /*create a window*/
    updateWindowSize();
    updateStartPos();
    win = newwin(windowSize.first, windowSize.second, startPos.first, startPos.second); 
    keypad(win,true);
    wnoutrefresh(win);

    lastMode = NORMAL;
    hasClear = true;

    print();

}

void information_window :: print() {
    curs_set(0);
    wmove(win, 0, 0);
    wdeleteln(win);
    wprintw(win, "MODE:");
    switch (window :: mode) {
    case NORMAL:
        wprintw(win, "NORMAL ");
        break;
    case INSERT:
        wprintw(win, "INSERT ");
        break;
    case COMMAND:
        wprintw(win, "COMMAND");
        break;
    default:
        fprintf(stderr, "mode error");
        exit(1);
        break;
    }
    wprintw(win, "\t%s", fileName.c_str());
    wprintw(win, "\t%4d-%-4d", currentPos.first, currentPos.second);
    wnoutrefresh(win);
}

void information_window :: clear() {
    werase(win);
    wnoutrefresh(win);
    hasClear = true;
}

void information_window :: update(POS cpos) {
    if (cpos != currentPos || mode != lastMode || hasClear) {
        currentPos = cpos;
        lastMode = mode;
        print();
        hasClear = false;
    }
}

void information_window :: updateFileName(std :: string str) {
    fileName = str;
    print();
}

void information_window :: updateWindowSize() {
    windowSize.first = 1;
    windowSize.second = consoleSize.second;
}

void information_window :: updateStartPos() {
    startPos.first = consoleSize.first - 1;
    startPos.second = 0;
}