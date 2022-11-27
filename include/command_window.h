#pragma once
#include "window.h"
#include <string>
class command_window : public window {
    private:
        std :: string commandText;
        void print();
        void updateWindowSize();
    public:
        bool tackle();
        void init();
};