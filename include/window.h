#pragma once
#include <ncurses.h>
#include <cstdlib>
#include <utility>
const int DEFAULT_COLOR = 0;
const int NORMAL = 0;
const int INSERT = 1;
const int COMMAND = 2;
class window {
    private:
        static int windowNum;
        
    protected:
        static std :: pair <int, int > windowWidth;
        static int colorNum;
        static bool change; /*if change != hasChange, you need to update the window size*/
        static int state;/*the mode the program is in*/
        bool hasChange;
        std :: pair <int, int > windowSize;
        std :: pair <int, int > mousePos;
        WINDOW *win;
        void updateMouse();
        virtual void print() = 0;
    public:
        static void updateWindowWith();
        static int getState();
        window();
        ~window();
};