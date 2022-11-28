#pragma once
#include "window.h"
#include <vector>
#include <string>

class file_window : public window {
    private:
        std :: vector<std :: string > fileText;
        POS absolutePos;
        POS inFilePos;
        POS realPos;
        POS printBegin;
        POS realPrintBegin;
        int dNum;
        bool atBottom;
        void print();
        bool normal (int ch);
        void insert (int ch);
        bool moveLeft(bool flush = true);
        bool moveRight(bool flush = true);
        bool moveUp(bool flush = true);
        bool moveDown(bool flush = true);
        void getInFilePos();
        void getRealPos();
        int getLineNum(int l);
        void changeMouse();
        bool lineMoveLeft();/*when the sentence is very long*/
        bool lineMoveRight();
        bool sentenceMoveDown();/*when the page needs to be turned*/
        bool sentenceMoveUp();
        void updateWindowSize();
        int turnLimit();
        POS fileToReal(POS fp);
        void getRealPrintBegin();
        void nextWordHead();
        void lastWordHead();
        bool lineHead(bool flush = true);
        bool lineTail(bool flush = true);
        bool isWord(char ch);
        void deleteLine();
        void insertChar(int ch);
        void backSpace();
        void deleteChar();
        void newLine();
    public:
        file_window();
        bool keyboard();/*the return value tells you whether to change to command mode or not*/
        void fileRead(std :: string fname);
        void newFile();
        POS getCurrentPos();
};