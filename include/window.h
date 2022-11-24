#pragma once
#include <ncurses.h>
#include <cstdlib>
#include <utility>
typedef std :: pair<int, int > POS;
const int DEFAULT_COLOR = 0;
const int NORMAL = 0;
const int INSERT = 1;
const int COMMAND = 2;
class window {
    private:
        static int windowNum;
        
    protected:
        static POS consoleSize;
        static int colorNum;
        static bool change; /*if change != hasChange, you need to update the window size*/
        static int mode;/*the mode the program is in*/
        bool hasChange;
        POS windowSize;
        POS mousePos;
        WINDOW *win;
        FILE *ferr;
        void updateMouse();
        virtual void updateWindowSize();
        virtual void print() = 0;
    public:
        static void updateConsoleSize();
        static int getMode();
        window();
        ~window();
};