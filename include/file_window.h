#pragma once
#include "window.h"
#include <vector>
#include <string>

class file_window : public window {
    private:
        std :: vector< std :: string> fileText;
        WINDOW *win;
    public:
        file_window();
};