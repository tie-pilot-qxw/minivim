#pragma once
#include "window.h"
#include <string>
class information_window : public window {
    private:
        std::string fileName;
        POS currentPos;
        int lastMode;/*to check whether the mode has been changed*/
        bool hasClear;/*whether the window has been cleared*/
        void print();
        void updateWindowSize();
        void updateStartPos();
    public:
        information_window();
        void update(POS cpos);
        void clear();
        void updateFileName(const std::string &str);
};