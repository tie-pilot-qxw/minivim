#pragma once
#include "window.h"
#include <string>
class information_window : public window {
    private:
        std :: string fileName;
        POS currentPos;
        void print();
        void updateWindowSize();
        void updateStartPos();
        int lastMode;
    public:
        information_window();
        void update(POS cpos);
        void clear();
        void updateFileName(std :: string str);
};