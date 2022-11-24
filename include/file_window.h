#pragma once
#include "window.h"
#include <vector>
#include <string>

class file_window : public window {
    private:
        std :: vector<std :: string > fileText;
        POS absoluteMousePos;
        POS inFileMousePos;
        POS realMousePos;
        POS printBegin;
        bool atBottom;
        void print();
        bool normal (int ch);
        void insert (int ch);
        void mouseMoveLeft();
        void mouseMoveRight();
        void mouseMoveUp();
        void mouseMoveDown();
        void getInFilePos();
        void getRealPos();
        int getLineNum(int l);
        void changeMouse();
        bool lineMove();/*when the sentence is very long*/
        bool sentenceMoveDown();/*when the page needs to be turned*/
        bool sentenceMoveUp();
        void updateWindowSize();
        int turnLimit();
        POS fileToReal(POS fp);
    public:
        file_window();
        bool keyboard();/*the return value tells you whether to change to command mode or not*/
        void fileRead(std :: string fname);
        void newFile();
};