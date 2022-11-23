#include "file_window.h"
#include <algorithm>
#include <fstream>

file_window::file_window() {
    /*initialize the mousepos*/
    absolutePos = std :: make_pair(0, 0);
    inFilePos = std :: make_pair(0, 0);
    realPos = std :: make_pair(0, 0);

    /*the first line*/
    fileText.push_back("");

    win = newwin(LINES-2, COLS, 0, 0); /*create a window in (0, 0)*/
    keypad(win,true);
    wrefresh(win);

    //wbkgd(win, COLOR_PAIR(2)); /*set customized window color*/
    wrefresh(win);

    wprintw(win, "Press Esc to Exit");
    wmove(win, 2, 2);
    wprintw(win, "Hello, world!"); /* Print Hello World */
    mvwprintw(win, 3, 2, "This is MiniVim, your input will appear on the next line."); /*Move and Print*/
    wrefresh(win); /* Print it on to the real screen */

    wmove(win, 4, 2);
}

void file_window :: keyboard() {
    int ch=getch();
    if (state == NORMAL) normal(ch);
    else if (state == INSERT) insert(ch);
}

void file_window :: normal (int ch) {
    if(hasChange != change) {
        hasChange ^= 1;
        /*flush*/
    }
    switch (ch) {
    case KEY_LEFT:
        moveLeft();
        break;
    case KEY_UP:
        moveUp();
        break;
    case KEY_RIGHT:
        moveRight();
        break;
    case KEY_DOWN:
        moveDown();
        break;
    case ERR:
        break;
    default:
        state = INSERT;
        break;
    }
    changeMouse();
}

void file_window :: moveDown() {
    if (absolutePos.first >= fileText.size() - 1) {
        /*maybe some error report?*/
        return;
    }
    absolutePos.first++;
    getInFilePos();
    getRealPos();
}

void file_window :: moveUp() {
    if (absolutePos.first == 0) {
        /*maybe some error report?*/
        return;
    }
    absolutePos.first--;
    getInFilePos();
    getRealPos();
}

void file_window :: moveLeft() {
    if(absolutePos > inFilePos) {
        absolutePos = inFilePos;
    }
    if(absolutePos.second == 0) {
        /*maybe some error report?*/
        return;
    }
    absolutePos.second--;
    getInFilePos();
    getRealPos();
}

void file_window :: moveRight() {
    if(absolutePos.second > fileText[inFilePos.first].length()) {
        /*maybe some error report?*/
        return;
    }
    absolutePos.second++;
    getInFilePos();
    getRealPos();
}

void file_window :: getInFilePos() {
    inFilePos.first = absolutePos.first;
    inFilePos.second = std :: min (absolutePos.second, (int)fileText[inFilePos.first].length());
}

void file_window :: getRealPos() {
    realPos.first = 0;
    for(int i = 0; i < inFilePos.first ; i ++ ) {
        realPos.first += getLineNum(i);
    }
    int l = (inFilePos.second) / windowWidth.second;
    realPos.first += l;
    realPos.second = inFilePos.second - l * windowWidth.second;
}

int file_window :: getLineNum(int l){
    /*may be you should tackle the tab*/
    return ((std :: max((int)fileText[l].length() - 1, 0)) / windowWidth.second) + 1;
}

void file_window :: changeMouse() {
    wmove(win, realPos.first, realPos.second);
    wrefresh(win);
}

void file_window :: print() {
    wclear(win);
    wmove(win, 0, 0);
    int linenum = 0;
    for (int i = 0; i < (int)fileText.size(); i++) {
        int k=0;
        do{
            wmove(win, linenum, 0);
            for (int j = 0; j < windowWidth.second && k < fileText[i].length(); j++ , k++) {
                wprintw (win, "%c", fileText[i][k]);
            }
            linenum++;
        }while (k < fileText[i].length());
    }
    wrefresh(win);
}

void file_window :: insert(int ch) {
    
}

void file_window :: fileRead(std :: string fname) {
    fileText.clear();

    //get the file
    std :: fstream fin(fname.c_str());
    if (!fin.is_open()) {
        fprintf(stderr, "Can't open the file");
        endwin();
        exit(1);
    }

    //read the data
    std :: string data;
    char str[3020] = {0};
    while (fin.getline(str, 3000)) {
        data = str;
        fileText.push_back(data);
    }

    fin.close();

    print();
}