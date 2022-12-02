#pragma once
#include "window.h"
#include <string>
class information_window : public window {
    private:
        std::string fileName;
        POS currentPos;
        int lastMode;
        bool hasClear;
        void print();
        void updateWindowSize();
        void updateStartPos();
    public:
        information_window();
        void update(POS cpos);
        void clear();
        void updateFileName(const std::string &str);
};