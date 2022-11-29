#pragma once
#include "window.h"
#include <string>
#include <vector>
const int WARNING_COLOR = 1;
class command_window : public window {
    private:
        std :: string fileName;
        std :: vector<std :: string > *fileText;
        std :: string commandText;
        bool quit;
        int pos;
        void print();
        void updateWindowSize();
        void updateStartPos();
        void moveLeft();
        void moveRight();
        void backSpace();
        void deleteChar();
        void insertChar(int ch);
        void changeMouse();
        bool check();
        void clear();
        void saveFile();
    public:
        command_window();
        bool tackle();
        void init(std :: vector<std :: string > *file);
        void updateFileName(std :: string str);
};