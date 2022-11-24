#include "command_window.h"
bool command_window :: tackle() {
    return true;
}

void command_window :: print() {
    
}

void command_window :: init() {
    wclear(win);
    commandText += ':';
    print();
}