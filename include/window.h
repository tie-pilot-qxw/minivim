#pragma once
#include <ncurses.h>
#include <cstdlib>
const int DEFAULT_COLOR = 0;
class window {
    private:
        void flush();
        static int windowNum;
    protected:
        static int colorNum;
    public:
        window();
};