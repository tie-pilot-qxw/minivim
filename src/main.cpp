#include <ncurses.h>
#include <unistd.h>
#include <iostream>
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
            auto file = fw.getFile();
            iw.clear();
            cw.init(file);
        }
        else iw.update(fw.getCurrentPos());
    }
    doupdate();
    return rt;
}

int main(int argc, char *argv[]) {
    information_window iw;
    command_window cw;
    file_window fw;
    extern int optind;
    extern char *optarg;
    int ch, num = 0;

    #ifndef debug
    endwin();
    if (argc == 1) {
        fprintf(stderr, "please input the file name\n");
        return 0;
    }
    if (argc == 2 && argv[1][0] != '-') {
        fw.fileRead(argv[1]);
        iw.updateFileName(argv[1]);
        cw.updateFileName(argv[1]);
    }else {
        if (argv[1][0] != '-') {
            fprintf(stderr, "the command should be: minivim-bin [options] <filename>\n");
            return 0;
        }
        while ((ch = getopt(argc, argv, "t:R:")) != -1) {
            if (num == 1) {
                fprintf(stderr, "too many options\n");
                return 0;
            }
            switch (ch) {
            case 't':
                fw.truncation();
                break;
            case 'R':
                fw.readOnly();
                break;
            default:
                return 0;
                break;
            }
            num++;
            fw.fileRead(optarg);
            iw.updateFileName(optarg);
            cw.updateFileName(optarg);
        }
    }
    #endif
    
    #ifdef debug
    fw.fileRead("/home/qxw/minivim/abcd");
    iw.updateFileName("/home/qxw/minivim/abc");
    cw.updateFileName("/home/qxw/minivim/abc");
    #endif

    while(call_back(cw,fw,iw));
    return 0;
}