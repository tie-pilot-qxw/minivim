#include "input_method.h"
#include <fstream>
#include <stdlib.h>

input_method::input_method() {
    /*create a window*/
    updateWindowSize();
    updateStartPos();
    win = newwin(windowSize.first, windowSize.second, startPos.first, startPos.second); 
    keypad(win,true);
    init_pair(WORD_COMPLETE_COLOR, COLOR_WHITE, COLOR_BLUE);
    readFile();
    startWord = 0;
    nowWord = 0;
    print();
}

void input_method::wake() {
    updateWindow();
    wbkgd(win, COLOR_PAIR(WORD_COMPLETE_COLOR));
    wnoutrefresh(win);
}

void input_method::sleep() {
    clear();
    wbkgd(win, COLOR_PAIR(DEFAULT_COLOR));
    wnoutrefresh(win);
}

void input_method::print() {
    werase(win);
    updateWindow();
    curs_set(0);
    wmove(win, 0, 0);
    if (prefix.length() != 0) {
        for (int i = 0; i < wordC.size(); i++) {
            if (i == nowWord) wprintw(win, "%2d.*%s%s    "
            , i + 1, prefix.c_str(), wordC[i].substr(prefix.length()).c_str());
            else wprintw(win, "%2d. %s%s    "
            , i + 1, prefix.c_str(), wordC[i].substr(prefix.length()).c_str());
        }
    }
    wnoutrefresh(win);
}

void input_method::updateWindowSize() {
    windowSize.first = 1;
    windowSize.second = consoleSize.second;
}

void input_method::updateStartPos() {
    startPos.first = consoleSize.first - 2;
    startPos.second = 0;
}

bool input_method::initSuc() {
    return success;
}

void input_method::readFile() {
    std::fstream fin;
    std::string path = getenv("HOME");
    path += "/minivim/res/words_alpha.txt";
    fin.open(path.c_str());
    if (fin.is_open()) {
        success = true;
        std::string text;
        while (getline(fin, text)) {
            fin >> text;
            dictionary.insert(text);
        }
    }else {
        success = false;
    }
}

void input_method::clear() {
    startWord = 0;
    nowWord = 0;
    prefix.clear();
    lowerPre.clear();
    wordC.clear();
    while (!st.empty()) st.pop();
    print();
}

void input_method::update(const std::string &str) {
    clear();
    prefix = str;
    /*tackle the captial char*/
    for (int i = 0; i < prefix.length(); i++) {
        if (prefix[i] >= 'A' && prefix[i] <= 'Z') lowerPre += prefix[i] - 'A' +'a';
        else lowerPre += prefix[i];
    }
    getStr();
    print();
}

void input_method::getStr() {
    int pos = 0;
    wordC.clear();
    auto itor = dictionary.find_prefix(lowerPre);
    for (int i = 1; i <= startWord && itor != dictionary.end(); i++) ++itor;
    while (pos < windowSize.second && itor != dictionary.end()) {
        pos += itor.key().length() + 4;
        if (pos > windowSize.second) break;
        if (wordC.size() >= 9) break;
        wordC.push_back(itor.key());
        pos += 4;
        ++itor;
    }
}

bool input_method::select() {
    if (prefix.length() == 0 || wordC.size() == 0) return false;
    return true;
}

std::string input_method::getWord() {
    return wordC[nowWord].substr(prefix.length());
}

void input_method::pageUp() {
    auto itor = dictionary.find_prefix(lowerPre);
    if (st.empty()) return;
    nowWord = 0;
    startWord -= st.top();
    st.pop();
    getStr();
    print();
}

void input_method::pageDown() {
    auto itor = dictionary.find_prefix(lowerPre);
    for (int i = 1; i <= startWord + wordC.size(); i++) {
        if (itor == dictionary.end()) return;
        ++itor;
    }
    if (itor == dictionary.end()) return;
    nowWord = 0;
    st.push(wordC.size());
    startWord += wordC.size();
    getStr();
    print();
}

void input_method::nextWord() {
    if (nowWord < wordC.size() - 1) {
        nowWord++;
        print();
    }
    else pageDown();
}

void input_method::lastWord() {
    if (nowWord > 0) {
        nowWord--;
        print();
    }
    else pageUp();
}

bool input_method::select(int num) {
    num--;
    if (num >= 0 && num < wordC.size()) {
        nowWord = num;
        return true;
    }else return false;
}