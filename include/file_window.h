#pragma once
#include "window.h"
#include <vector>
#include <string>

class file_window : public window {
    private:
        std :: vector<std :: string > fileText;
        std :: pair<int, int > absolutePos;
        std :: pair<int, int > inFilePos;
        std :: pair<int, int > realPos;
        void print();
        void normal (char ch);
        void insert (char ch);
        void moveLeft();
        void moveRight();
        void moveUp();
        void moveDown();
        void getInFilePos();
        void getRealPos();
        int getLineNum(int l);
        void changeMouse();
    public:
        file_window();
        void keyboard();
        void fileRead(std :: string fname);
};