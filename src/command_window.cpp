#include "command_window.h"
#include <fstream>
#include <iostream>

command_window::command_window() {
    quit = false;
    /*create a window*/
    updateWindowSize();
    updateStartPos();
    win = newwin(windowSize.first, windowSize.second, startPos.first, startPos.second); 
    keypad(win,true);
    init_pair(WARNING_COLOR, COLOR_WHITE, COLOR_RED);
    wnoutrefresh(win);
    print();
}

void command_window::print() {
    werase(win);
    wmove(win, 0, 0);
    wprintw(win, "%s", commandText.c_str());
    wnoutrefresh(win);
}

void command_window::init(std::vector<std::string > *file) {
    fileText = file;
    pos = 1;
    commandText.clear();
    commandText += ':';
    wbkgd(win, COLOR_PAIR(DEFAULT_COLOR));
    print();
}

void command_window::updateWindowSize() {
    windowSize.first = 1;
    windowSize.second = consoleSize.second;
}

void command_window::updateStartPos() {
    startPos.first = consoleSize.first - 1;
    startPos.second = 0;
}

void command_window::clear() {
    werase(win);
    wnoutrefresh;
}
bool command_window::tackle() {
    int ch = getch();
    bool rt = true;
    if(quit && ch != ERR) {
        quit = false;
        mode = NORMAL;
        clear();
        return true;
    }
    switch (ch) {
    case KEY_LEFT:
        moveLeft();
        break;
    case KEY_RIGHT:
        moveRight();
        break;
    case 27:/*esc*/
        mode = NORMAL;
        break;
    case ERR:
        break;
    case KEY_BACKSPACE:/*backspace*/
        backSpace();
        break;
    case '\t':
        break;
    case 330:/*del*/
        deleteChar();
        break;
    case '\n':
        rt = check();
        break;
    default:
        insertChar(ch);
        break;
    }
    if(!quit) changeMouse();
    return rt;
}

void command_window::moveLeft() {
    if(pos == 0) {
        /*maybe some error report?*/
        return;
    }
    pos--;
}

void command_window::moveRight() {
    if(pos >= commandText.length()) {
        /*maybe some error report?*/
        return;
    }
    pos++;
}

void command_window::changeMouse() {
    wmove(win, 0, pos);
    wrefresh(win);
    curs_set(1);
}

void command_window::insertChar(int ch) {
    commandText.insert(pos, 1, (char)ch);
    moveRight();
    print();
}

void command_window::backSpace() {
    if (pos == 0) return;
    moveLeft();
    commandText.erase(pos, 1);
    print();
    if (commandText.length() == 0) mode = NORMAL;
}

void command_window::deleteChar() {
    if (pos == commandText.length()) return;
    commandText.erase(pos, 1);
    print();
    if (pos == commandText.length()) mode = NORMAL;
}

bool command_window::check() {
    if (commandText == ":w") {
        hasSave = true;
        saveFile();
        mode = NORMAL;
        return true;
    }else if (commandText == ":q") {
        if(!hasSave) {
            werase(win);
            wbkgd(win, COLOR_PAIR(WARNING_COLOR));
            wmove(win, 0, 0);
            wprintw(win, "No write since last change (add ! to override)");
            wnoutrefresh(win);
            quit = true;
            return true;
        }else {
            return false;
        }
    }else if (commandText == ":q!") {
        return false;
    }else if (commandText == ":wq") {
        hasSave = true;
        saveFile();
        return false;
    }else {
        werase(win);
        wbkgd(win, COLOR_PAIR(WARNING_COLOR));
        wmove(win, 0, 0);
        wprintw(win, "Wrong commmand");
        wnoutrefresh(win);
        quit = true;
        return true;
    }
}

void command_window::saveFile() {

    //get the file
    std::fstream fout(fileName.c_str(), std::ios::out | std::ios::trunc);
    if (!fout.is_open()) {
        endwin();
        fprintf(stderr, "Can't open the file");
        exit(1);
    }

    for(int i = 0; i < fileText->size(); i++) {
        fout << fileText->at(i) << "\n";
    }

    fout.close();

}

void command_window::updateFileName(const std::string &str) {
    fileName = str;
    print();
}