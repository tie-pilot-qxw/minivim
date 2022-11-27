#include <ncurses.h>
#include <iostream>
#define dubug
#include "command_window.h"
#include "file_window.h"
#include "information_window.h"
#include "window.h"

bool call_back (command_window &cw, file_window &fw, information_window &iw){
    bool rt = true;
    window :: updateConsoleSize();
    if(window :: getMode() == COMMAND) {
        rt = cw.tackle();
    }else {
        if(fw.keyboard()) {
            iw.clear();
            cw.init();
        }
        else iw.update(fw.getCurrentPos());
    }
    doupdate();
    return rt;
}

int main() {
    information_window iw;
    command_window cw;
    file_window fw;
    fw.fileRead("../abc");
    iw.updateFileName("hello world");
    while(call_back(cw,fw,iw));
    return 0;
}