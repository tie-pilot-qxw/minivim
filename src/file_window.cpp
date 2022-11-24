#include "file_window.h"
#include <algorithm>
#include <fstream>

file_window::file_window() {
    /*initialize the mousepos*/
    absoluteMousePos = std :: make_pair(0, 0);
    inFileMousePos = std :: make_pair(0, 0);
    realMousePos = std :: make_pair(0, 0);

    printBegin = std :: make_pair(0, 0);/*init the print-begin*/

    atBottom = true;

    /*create a window in (0, 0)*/
    updateWindowSize();
    win = newwin(windowSize.first, windowSize.second, 0, 0); 
    keypad(win,true);
    wnoutrefresh(win);

    wprintw(win, "Press Esc to Exit");
    wmove(win, 2, 2);
    wprintw(win, "Hello, world!"); /* Print Hello World */
    mvwprintw(win, 3, 2, "This is MiniVim, your input will appear on the next line."); /*Move and Print*/
    wnoutrefresh(win); /* Print it on to the virtual screen */

    wmove(win, 4, 2);

}

bool file_window :: keyboard() {
    int ch=getch();
    if (mode == NORMAL) return normal(ch);
    else if (mode == INSERT) {
        insert(ch);
        return false;
    }
    return true;
}

bool file_window :: normal (int ch) {
    if(hasChange != change) {
        hasChange ^= 1;
        /*flush*/
    }
    switch (ch) {
    case KEY_LEFT:
        mouseMoveLeft();
        break;
    case KEY_UP:
        mouseMoveUp();
        break;
    case KEY_RIGHT:
        mouseMoveRight();
        break;
    case KEY_DOWN:
        mouseMoveDown();
        break;
    case ERR:
        break;
    case ':':
        mode = COMMAND;
        return true;
        break;
    default:
        mode = INSERT;
        break;
    }
    changeMouse();
    return false;
}

void file_window :: getRealPrintBegin() {
    realPrintBegin = printBegin;
    while(realPrintBegin.second >= fileText[realPrintBegin.first].length() && realPrintBegin.second > 0)
        realPrintBegin.second -= windowSize.second;
}

void file_window :: mouseMoveDown() {
    if (absoluteMousePos.first >= fileText.size() - 1) {
        /*maybe some error report?*/
        return;
    }
    absoluteMousePos.first++;
    getInFilePos();
    if (sentenceMoveDown()) {
        getRealPrintBegin();
        getRealPos();
        print();
    }else {
        getRealPrintBegin();
        getRealPos();
    }
}

void file_window :: mouseMoveUp() {
    if (absoluteMousePos.first == 0) {
        /*maybe some error report?*/
        return;
    }
    absoluteMousePos.first--;
    getInFilePos();
    if (sentenceMoveUp()) {
        getRealPrintBegin();
        getRealPos();
        print();
    }else {
        getRealPrintBegin();
        getRealPos();
    }
}

void file_window :: mouseMoveLeft() {
    if(absoluteMousePos > inFileMousePos) {
        absoluteMousePos = inFileMousePos;
    }
    if(realPrintBegin.second < printBegin.second) printBegin = realPrintBegin;
    if(absoluteMousePos.second == 0) {
        /*maybe some error report?*/
        return;
    }
    absoluteMousePos.second--;
    getInFilePos();
    if (lineMoveLeft()) {
        getRealPrintBegin();
        getRealPos();
        print();
    }else {
        getRealPrintBegin();
        getRealPos();
    }
}

void file_window :: mouseMoveRight() {
    if(mode == NORMAL && absoluteMousePos.second >= fileText[inFileMousePos.first].length() - 1) {
        return;
    }
    if(absoluteMousePos.second >= fileText[inFileMousePos.first].length()) {
        /*maybe some error report?*/
        return;
    }
    absoluteMousePos.second++;
    getInFilePos();
    if (lineMoveRight()) {
        getRealPrintBegin();
        getRealPos();
        print();
    }else {
        getRealPrintBegin();
        getRealPos();
    }
}

void file_window :: getInFilePos() {
    inFileMousePos.first = absoluteMousePos.first;
    if(mode == INSERT)
        inFileMousePos.second = 
        std :: min (absoluteMousePos.second, (int)fileText[inFileMousePos.first].length());
    else {
        inFileMousePos.second = 
        std :: min (absoluteMousePos.second, (int)fileText[inFileMousePos.first].length() - 1);
        inFileMousePos.second = std :: max(inFileMousePos.second, 0);
    }
}

void file_window :: getRealPos() {
    realMousePos = fileToReal(inFileMousePos);
}

POS file_window :: fileToReal(POS fp) {
    POS start = std :: make_pair(0, 0);
    POS end = std :: make_pair(0, 0);
    for(int i = 0; i < realPrintBegin.first ; i ++ ) {
        start.first += getLineNum(i);
    }
    int sl = (realPrintBegin.second) / windowSize.second;
    start.first += sl;
    start.second = realPrintBegin.second - sl * windowSize.second;
    for(int i = 0; i < fp.first ; i ++ ) {
        end.first += getLineNum(i);
    }
    int el = (fp.second) / windowSize.second;
    end.first += el;
    end.second = fp.second - el * windowSize.second;
    return std :: make_pair(end.first - start.first, end.second - start.second);
}
int file_window :: getLineNum(int l){
    /*may be you should tackle the tab*/
    return ((std :: max((int)fileText[l].length() - 1, 0)) / windowSize.second) + 1;
}

void file_window :: changeMouse() {
    wmove(win, realMousePos.first, realMousePos.second);
    wnoutrefresh(win);
}

void file_window :: print() {
    wclear(win);
    wmove(win, 0, 0);
    int linenum = 0;
    int i, k;
    for (i = realPrintBegin.first; i < (int)fileText.size() && linenum < windowSize.first; i++) {
        if (i == realPrintBegin.first) {
            k = realPrintBegin.second;
        }
        else k = 0;
        do{
            wmove(win, linenum, 0);
            for (int j = 0; j < windowSize.second && k < fileText[i].length(); j++ , k++) {
                wprintw (win, "%c", fileText[i][k]);
            }
            linenum++;
        }while (k < fileText[i].length() && linenum < windowSize.first);
    }
    if(i == fileText.size() && k == fileText[i - 1].length()) atBottom=true;
    else atBottom = false;
    wnoutrefresh(win);
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
    while (getline(fin, data)) {
        fileText.push_back(data);
    }

    fin.close();

    print();
}

void file_window :: updateWindowSize() {
    windowSize.first = consoleSize.first - 1;
    windowSize.second = consoleSize.second;
}

void file_window :: newFile() {
    /*the first line*/
    fileText.push_back("");
    /*to be continue*/
}

int file_window :: turnLimit() {
    return std :: min(5, (windowSize.first + 1) / 2);
}

bool file_window :: sentenceMoveDown() {
    getRealPos();
    bool rt = false;
    if(!atBottom) {
        if (realMousePos.first > windowSize.first - turnLimit()) {
            printBegin.first++;
            rt = true;
        }
        POS nxt = fileToReal(POS(inFileMousePos.first, fileText[inFileMousePos.first].length() - 1));
        if (nxt.first >= windowSize.first) {
            printBegin.first = inFileMousePos.first;
            rt = true;
        }
    }
    return rt;
}

bool file_window :: sentenceMoveUp() {
    bool rt = false;
    getRealPos();
    if (realMousePos.first < turnLimit() - 1) {
        if (printBegin.first > 0) {
            printBegin.first--;
            rt = true;
            POS the = fileToReal(POS(inFileMousePos.first,0));
            POS nxt = fileToReal(POS(printBegin.first, 0));
            if (nxt.first < 0 && the.first >= 0) {
                printBegin.first++;
                rt = false;
            }
        }
    }
    return rt;
}

bool file_window :: lineMoveLeft() {
    getRealPos();
    POS head = fileToReal(POS(inFileMousePos.first, 0));
    if(realMousePos.first < turnLimit() - 1 && head.first < 0) {
        if(printBegin.second > 0) {
            printBegin.second -= windowSize.second;
            return true;
        }
    }
    return false;
}

bool file_window :: lineMoveRight() {
    getRealPos();
    POS tail = fileToReal(POS(inFileMousePos.first, fileText[inFileMousePos.first].length() - 1));
    if(realMousePos.first > windowSize.first - turnLimit() && tail.first >= windowSize.first) {
        printBegin.second += windowSize.second;
        return true;
    }
    if(realMousePos.first > tail.first) {
        printBegin.second += windowSize.second;
        return true;
    }
    return false;
}