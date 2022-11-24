#include <ncurses.h>
#include <iostream>
#define dubug
#include "command_window.h"
#include "file_window.h"
#include "information_window.h"
#include "window.h"

bool call_back (command_window &cw, file_window &fw, information_window &iw){
    window :: updateConsoleSize();
    if(window :: getMode() == COMMAND) {
        return cw.tackle();
    }else {
        if(fw.keyboard()) {
            iw.clear();
            cw.init();
        }
        else iw.update();
    }
    return true;
}

int main() {
    command_window cw;
    file_window fw;
    information_window iw;
    fw.fileRead("../abc");
    while(call_back(cw,fw,iw));
    return 0;
}