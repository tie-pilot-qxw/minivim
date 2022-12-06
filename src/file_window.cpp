#include "file_window.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

file_window::file_window() {
    /*initialize the mousepos*/
    absolutePos = std::make_pair(0, 0);
    inFilePos = std::make_pair(0, 0);
    realPos = std::make_pair(0, 0);

    printBegin = std::make_pair(0, 0);/*init the print-begin*/

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

    canWrite = true;

    isTrun = false;
}

bool file_window::keyboard() {
    int ch=getch();
    if (updateWindow()) {
        getRealPrintBegin();
        getRealPos();
    }
    if (mode == NORMAL) return normal(ch);
    else if (mode == INSERT) {
        insert(ch);
        return false;
    }
    return true;
}

bool file_window::normal (int ch) {
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
    case ':':
        mode = COMMAND;
        return true;
        break;
    case 'h':
        moveLeft();
        break;
    case 'l':
        moveRight();
        break;
    case 'j':
        moveDown();
        break;
    case 'k':
        moveUp();
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
        if (!canWrite) break;
        dNum++;
        if (dNum == 2) {
            deleteLine();
            dNum = 0;
        }
        break;
    case 'i':
        if (!canWrite) break;
        mode = INSERT;
        if (wordMap.initSuc()) {
            updateWindowSize();
            wresize(win, windowSize.first, windowSize.second);
            wordMap.wake();
            print();
        }
        break;
    default:
        break;
    }
    if (ch != ERR && ch != 'd') dNum = 0;
    changeMouse();
    return false;
}

void file_window::getRealPrintBegin() {
    realPrintBegin = printBegin;
    while(realPrintBegin.second >= fileText[realPrintBegin.first].length() && realPrintBegin.second > 0)
        realPrintBegin.second -= windowSize.second;
}

bool file_window::moveDown(bool flush) {
    if (absolutePos.first >= fileText.size() - 1) {
        /*maybe some error report?*/
        return false;
    }
    absolutePos.first++;
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

bool file_window::moveUp(bool flush) {
    if (absolutePos.first == 0) {
        /*maybe some error report?*/
        return false;
    }
    absolutePos.first--;
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

bool file_window::moveLeft(bool flush) {
    if(absolutePos > inFilePos) {
        absolutePos = inFilePos;
    }
    if(realPrintBegin.second < printBegin.second) printBegin = realPrintBegin;
    if(absolutePos.second == 0) {
        /*maybe some error report?*/
        return false;
    }
    absolutePos.second--;
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

bool file_window::moveRight(bool flush) {
    if(mode == NORMAL && absolutePos.second >= fileText[inFilePos.first].length() - 1) {
        return false;
    }
    if(absolutePos.second >= fileText[inFilePos.first].length()) {
        /*maybe some error report?*/
        return false;
    }
    absolutePos.second++;
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

void file_window::getInFilePos() {
    inFilePos.first = absolutePos.first;
    if(mode == INSERT)
        inFilePos.second = 
        std::min (absolutePos.second, (int)fileText[inFilePos.first].length());
    else {
        inFilePos.second = 
        std::min (absolutePos.second, (int)fileText[inFilePos.first].length() - 1);
        inFilePos.second = std::max(inFilePos.second, 0);
    }
}

void file_window::getRealPos() {
    realPos = fileToReal(inFilePos);
}

POS file_window::fileToReal(POS fp) {
    POS start = std::make_pair(0, 0);
    POS end = std::make_pair(0, 0);
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
    return std::make_pair(end.first - start.first, end.second - start.second);
}

int file_window::getLineNum(int l){
    /*may be you should tackle the tab*/
    return ((std::max((int)fileText[l].length() - 1, 0)) / windowSize.second) + 1;
}

void file_window::changeMouse() {
    wmove(win, realPos.first, realPos.second);
    wrefresh(win);
    curs_set(1);
}

void file_window::print() {
    werase(win);
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

void file_window::insert(int ch) {
    bool wordchange = false;
    switch (ch) {
    case KEY_RESIZE:/*when the window size change, it will receive -102*/
        break;
    case KEY_LEFT:
        moveLeft();
        break;
    case KEY_UP:
        if (prefixLength > 0 && wordMap.initSuc()) {
            wordMap.lastWord();
            wordchange = true;
        }else moveUp();
        break;
    case KEY_RIGHT:
        moveRight();
        break;
    case KEY_DOWN:
        if (prefixLength > 0 && wordMap.initSuc()) {
            wordMap.nextWord();
            wordchange = true;
        }else moveDown();
        break;
    case 27:/*esc*/
        mode = NORMAL;
        if (wordMap.initSuc()) {
            wordchange = true;
            updateWindowSize();
            wresize(win, windowSize.first, windowSize.second);
            prefixLength = 0;
            wordMap.sleep();
            wresize(win, windowSize.first, windowSize.second);
            print();
        }
        break;
    case ERR:
        break;
    case KEY_BACKSPACE:/*backspace*/
        backSpace();
        if (wordMap.initSuc() && prefixLength > 0) {
            wordchange = true;
            std::string pre;
            getPerfix(pre);
            prefixLength = pre.length();
            wordMap.update(pre);
        }
        break;
    case '\t':
        for (int i = 1; i <= 4; i++) insertChar(' ');
        break;
    case 330:/*del*/
        deleteChar();
        break;
    case '\n':
        if (wordMap.initSuc() && prefixLength > 0 && wordMap.select()) {
            std::string str = wordMap.getWord();
            for (int i = 0; i < str.length(); i++) insertChar(str[i]);
        }else newLine();
        break;
    case 338:/*PageDn*/
        if (wordMap.initSuc() && prefixLength > 0) {
            wordchange = true;
            wordMap.pageDown();
        }
        break;
    case 339:/*PageUp*/
        if (wordMap.initSuc() && prefixLength > 0) {
            wordchange = true;
            wordMap.pageUp();
        }
        break;
    default:
        if (ch >= '1' && ch <= '9' ) {
            if (wordMap.select(ch-'0')) {
                std::string str = wordMap.getWord();
                for (int i = 0; i < str.length(); i++) insertChar(str[i]);
                break;
            }
        }
        insertChar(ch);
        if (((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && wordMap.initSuc()) {
            wordchange = true;
            std::string pre;
            getPerfix(pre);
            prefixLength = pre.length();
            wordMap.update(pre);
        }
        
        break;
    }
    if (!wordchange && wordMap.initSuc() && ch != ERR) {
        prefixLength = 0;
        wordMap.clear();
    }
    changeMouse();
}

void file_window::fileRead(const std::string &fname) {
    fileText.clear();

    //get the file
    std::fstream fin;
    fin.open(fname.c_str());
    if (!fin.is_open()) {
        struct stat check;
        if (stat(fname.c_str(), &check) != -1 && (check.st_mode & S_IFDIR)) {
            endwin();
            fprintf(stderr, "This is a folder\n");
            exit(1);
        }
        int pos = fname.find_last_of('/');
        std::string path = fname.substr(0, pos);
        if (pos != std::string::npos){
            if (stat(path.c_str(), &check) == -1 || !(check.st_mode & S_IFDIR)) {
                endwin();
                fprintf(stderr, "Wrong path\n");
                exit(1);
            }
        }
    }

    //read the data
    std::string data;
    while (getline(fin, data)) {
        std::string temp;
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
    if (isTrun) {
        fileText.clear();
        hasSave = false;
    }
    if (fileText.size() == 0) fileText.push_back("");
    print();
}

void file_window::updateWindowSize() {
    if (mode == INSERT) windowSize.first = consoleSize.first - 2;
    else windowSize.first = consoleSize.first - 1;
    windowSize.second = consoleSize.second;
}

int file_window::turnLimit() {
    return std::min(5, (windowSize.first + 1) / 2);
}

bool file_window::sentenceMoveDown() {
    getRealPos();
    bool rt = false;
    if(!atBottom) {
        if (realPos.first > windowSize.first - turnLimit()) {
            printBegin.first++;
            rt = true;
        }
        POS nxt = fileToReal(POS(inFilePos.first, fileText[inFilePos.first].length() - 1));
        if (nxt.first >= windowSize.first) {
            printBegin.first = inFilePos.first;
            rt = true;
        }
    }
    return rt;
}

bool file_window::sentenceMoveUp() {
    bool rt = false;
    getRealPos();
    if (realPos.first < turnLimit() - 1) {
        if (printBegin.first > 0) {
            printBegin.first--;
            rt = true;
            POS the = fileToReal(POS(inFilePos.first,0));
            POS nxt = fileToReal(POS(printBegin.first, 0));
            if (nxt.first < 0 && the.first >= 0) {
                printBegin.first++;
                rt = false;
            }
        }
    }
    return rt;
}

bool file_window::lineMoveLeft() {
    getRealPos();
    POS head = fileToReal(POS(inFilePos.first, 0));
    if(realPos.first < turnLimit() - 1 && head.first < 0) {
        if(printBegin.second > 0) {
            printBegin.second -= windowSize.second;
            return true;
        }
    }
    return false;
}

bool file_window::lineMoveRight() {
    getRealPos();
    POS tail = fileToReal(POS(inFilePos.first, fileText[inFilePos.first].length() - 1));
    if(realPos.first > windowSize.first - turnLimit() && tail.first >= windowSize.first) {
        printBegin.second += windowSize.second;
        return true;
    }
    if(realPos.first > tail.first) {
        printBegin.second += windowSize.second;
        return true;
    }
    return false;
}

bool file_window::isWord(char ch) {
    if (ch >= 'a' && ch <= 'z') return true;
    if (ch >= 'A' && ch <= 'Z') return true;
    if (ch >= '0' && ch <= '9') return true;
    if (ch == '_' || ch == '-') return true;
    return false;
}

void file_window::nextWordHead() {
    POS now = inFilePos;
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
            change = (change | moveRight(false));
            if(ok) break;
        }
        if(ok) break;
        end = true;
        change = (change | lineHead(false));
        change = (change | moveDown(false));
        if(end && isWord(fileText[inFilePos.first][0])) break;
        else if (!isWord(fileText[inFilePos.first][0])
        && fileText[inFilePos.first][0] != ' ') break;
        j = 1;
    }
    if(change) print();
}

void file_window::lastWordHead() {
    POS now = inFilePos;
    bool end = false;
    bool change = false;
    bool ok = false;
    bool islast = false;
    int j = now.second - 1;
    bool start = false;
    for (int i = now.first; i >= 0; i--) {
        if (i == now.first - 1 && fileText[i].length() == 0) {
            if (islast && start) {
                change = (change | moveDown(false));
                change = (change | lineHead(false));
            }
            break;
        }
        for (; j >= 0; j--) {
            start = true;
            if (!isWord(fileText[i][j]) && end) {
                if (islast) {
                    change = (change | moveDown(false));
                    change = (change | lineHead(false));
                }
                ok = true;
                break;
            }else if (isWord(fileText[i][j])) end = true;
            else if (!isWord(fileText[i][j]) && fileText[i][j] != ' ') ok = true;
            if(!islast) change = (change | moveLeft(false));
            islast = false;
            if(ok) break;
        }
        if (ok) break;
        if (i == 0) break;
        change = (change | moveUp(false));
        change = (change | lineTail(false));
        islast = true;
        j = fileText[i-1].length() - 1;
    }
    if(change) print();
}

bool file_window::lineHead(bool flush) {
    bool change = false;
    for(int i = inFilePos.second; i > 0; i --) {
        change = (change | moveLeft(false));
    }
    if (flush && change) print();
    return change;
}

bool file_window::lineTail(bool flush) {
    bool change = false;
    for(int i = inFilePos.second + 1; i < fileText[inFilePos.first].length(); i ++) {
        change = (change | moveRight(false));
    }
    if (flush && change) print();
    return change;
}

void file_window::deleteLine() {
    if (fileText.size() == 0) return;
    hasSave = false;
    fileText.erase(fileText.begin() + inFilePos.first);
    lineHead(false);
    print();
}

POS file_window::getCurrentPos() {
    return inFilePos;
}

void file_window::insertChar(int ch) {
    hasSave = false;
    absolutePos = inFilePos;
    fileText[inFilePos.first].insert(inFilePos.second, 1, (char)ch);
    moveRight(false);
    print();
}

void file_window::backSpace() {
    if (inFilePos.first == 0 && inFilePos.second == 0) return;
    if (inFilePos.second == 0) {
        /*merge to the last line*/
        moveUp(false);
        lineTail(false);
        moveRight(false);
        fileText[inFilePos.first] += fileText[inFilePos.first + 1];
        fileText.erase(fileText.begin() + inFilePos.first + 1);
    }else {
        moveLeft(false);
        fileText[inFilePos.first].erase(inFilePos.second, 1);
    }
    hasSave = false;
    print();
}

void file_window::deleteChar() {
    if (inFilePos.first == fileText.size() - 1
    && inFilePos.second == fileText[inFilePos.first].length()) return;
    if (inFilePos.second == fileText[inFilePos.first].length()) {
        /*merge the next line*/
        fileText[inFilePos.first] += fileText[inFilePos.first + 1];
        fileText.erase(fileText.begin() + inFilePos.first + 1);
    }else {
        fileText[inFilePos.first].erase(inFilePos.second, 1);
    }
    print();
    hasSave = false;
}

void file_window::newLine() {
    if (inFilePos.second == fileText[inFilePos.first].length()) {
        fileText.insert(fileText.begin() + inFilePos.first + 1, "");
        moveDown(false);
    }else {
        fileText.insert(fileText.begin() + inFilePos.first + 1,
        fileText[inFilePos.first].substr(inFilePos.second));
        fileText[inFilePos.first].erase(inFilePos.second);
        moveDown(false);
        lineHead(false);
    }
    print();
    hasSave = false;
}

std::vector<std::string >* file_window::getFile() {
    return &fileText;
}

void file_window::readOnly() {
    canWrite = false;
}

void file_window::truncation() {
    isTrun = true;
}

void file_window::getPerfix(std::string &str) {
    int i = inFilePos.second - 1;
    for (; i >= 0; i--) {
        if (!((fileText[inFilePos.first][i] >= 'a' && fileText[inFilePos.first][i] <= 'z')
        || (fileText[inFilePos.first][i] >= 'A' && fileText[inFilePos.first][i] <= 'Z'))) {
            break;
        }
    }
    str = fileText[inFilePos.first].substr(i + 1, inFilePos.second - i - 1);
}