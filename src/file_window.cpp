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

    dNum = 0;

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
    case 'h':
        mouseMoveLeft();
        break;
    case 'l':
        mouseMoveRight();
        break;
    case 'j':
        mouseMoveDown();
        break;
    case 'k':
        mouseMoveUp();
        break;
    case 'w':
        nextWordHead();
        break;
    case 'b':
        lastWordHead();
        break;
    case '0':
        lineHead();
        break;
    case '$':
        lineTail();
        break;
    case 'd':
        dNum++;
        if (dNum == 2) {
            deleteLine();
            dNum = 0;
        }
        break;
    default:
        mode = INSERT;
        break;
    }
    if (ch != ERR && ch != 'd') dNum = 0;
    changeMouse();
    return false;
}

void file_window :: getRealPrintBegin() {
    realPrintBegin = printBegin;
    while(realPrintBegin.second >= fileText[realPrintBegin.first].length() && realPrintBegin.second > 0)
        realPrintBegin.second -= windowSize.second;
}

bool file_window :: mouseMoveDown(bool flush) {
    if (absoluteMousePos.first >= fileText.size() - 1) {
        /*maybe some error report?*/
        return false;
    }
    absoluteMousePos.first++;
    getInFilePos();
    if (sentenceMoveDown()) {
        getRealPrintBegin();
        getRealPos();
        if (flush) print();
        return true;
    }else {
        getRealPrintBegin();
        getRealPos();
        return false;
    }
}

bool file_window :: mouseMoveUp(bool flush) {
    if (absoluteMousePos.first == 0) {
        /*maybe some error report?*/
        return false;
    }
    absoluteMousePos.first--;
    getInFilePos();
    if (sentenceMoveUp()) {
        getRealPrintBegin();
        getRealPos();
        if (flush) print();
        return true;
    }else {
        getRealPrintBegin();
        getRealPos();
        return false;
    }
}

bool file_window :: mouseMoveLeft(bool flush) {
    if(absoluteMousePos > inFileMousePos) {
        absoluteMousePos = inFileMousePos;
    }
    if(realPrintBegin.second < printBegin.second) printBegin = realPrintBegin;
    if(absoluteMousePos.second == 0) {
        /*maybe some error report?*/
        return false;
    }
    absoluteMousePos.second--;
    getInFilePos();
    if (lineMoveLeft()) {
        getRealPrintBegin();
        getRealPos();
        if (flush) print();
        return true;
    }else {
        getRealPrintBegin();
        getRealPos();
        return false;
    }
}

bool file_window :: mouseMoveRight(bool flush) {
    if(mode == NORMAL && absoluteMousePos.second >= fileText[inFileMousePos.first].length() - 1) {
        return false;
    }
    if(absoluteMousePos.second >= fileText[inFileMousePos.first].length()) {
        /*maybe some error report?*/
        return false;
    }
    absoluteMousePos.second++;
    getInFilePos();
    if (lineMoveRight()) {
        getRealPrintBegin();
        getRealPos();
        if (flush) print();
        return true;
    }else {
        getRealPrintBegin();
        getRealPos();
        return false;
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
        std :: string temp;
        for (int i = 0; i < data.length(); i ++) {
            if (data[i] == '\t') {
                for (int j = 1; j <= 4; j++) {
                    temp +=' ';
                }
            }else temp += data[i];
        }
        fileText.push_back(temp);
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

bool file_window :: isWord(char ch) {
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch >= '0' && ch <= '9') return true;
    if (ch == '_' || ch == '-') return true;
    return false;
}

void file_window :: nextWordHead() {
    POS now = inFileMousePos;
    bool end = false;
    bool change = false;
    bool ok = false;
    int j = now.second + 1;
    if (!isWord(fileText[now.first][now.second])) end = true;
    for(int i = now.first; i < fileText.size(); i++) {
        if (now.first != i && fileText[i].length() == 0) break;
        for (; j < fileText[i].length(); j++) {
            if(fileText[i][j] == ' ') end = true;
            else if (end && isWord(fileText[i][j])) ok = true;
            else if (!isWord(fileText[i][j])) ok = true;
            change = (change | mouseMoveRight(false));
            if(ok) break;
        }
        if(ok) break;
        change = (change | lineHead(false));
        change = (change | mouseMoveDown(false));
        if(end && isWord(fileText[i][0])) break;
        else if (!isWord(fileText[i][0]) && fileText[i][0] != ' ') break;
        j = 1;
    }
    if(change) print();
}

void file_window :: lastWordHead() {
    POS now = inFileMousePos;
    bool end = false;
    bool change = false;
    bool ok = false;
    bool islast = false;
    int j = now.second - 1;
    bool start = false;
    for (int i = now.first; i >= 0; i--) {
        if (i == now.first - 1 && fileText[i].length() == 0) {
            if (islast && start) {
                change = (change | mouseMoveDown(false));
                change = (change | lineHead(false));
            }
            break;
        }
        for (; j >= 0; j--) {
            start = true;
            if (!isWord(fileText[i][j]) && end) {
                if (islast) {
                    change = (change | mouseMoveDown(false));
                    change = (change | lineHead(false));
                }
                ok = true;
                break;
            }else if (isWord(fileText[i][j])) end = true;
            else if (!isWord(fileText[i][j]) && fileText[i][j] != ' ') ok = true;
            if(!islast) change = (change | mouseMoveLeft(false));
            islast = false;
            if(ok) break;
        }
        if (ok) break;
        if (i == 0) break;
        change = (change | mouseMoveUp(false));
        change = (change | lineTail(false));
        islast = true;
        j = fileText[i-1].length() - 1;
    }
    if(change) print();
}

bool file_window :: lineHead(bool flush) {
    bool change = false;
    for(int i = inFileMousePos.second; i > 0; i --) {
        change = (change | mouseMoveLeft(false));
    }
    if (flush && change) print();
    return change;
}

bool file_window :: lineTail(bool flush) {
    bool change = false;
    for(int i = inFileMousePos.second + 1; i < fileText[inFileMousePos.first].length(); i ++) {
        change = (change | mouseMoveRight(false));
    }
    if (flush && change) print();
    return change;
}

void file_window :: deleteLine() {
    fileText.erase(fileText.begin() + inFileMousePos.first);
    lineHead(false);
    print();
}