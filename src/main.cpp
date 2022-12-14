#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include "command_window.h"
#include "file_window.h"
#include "information_window.h"
#include "window.h"
//#define debug


bool call_back (command_window &cw, file_window &fw, information_window &iw){
    bool rt = true;
    window::updateConsoleSize();
    if(window::getMode() == COMMAND) {
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

    /*minivim-bin <filename>*/
    if (argc == 2 && argv[1][0] != '-') { 
        fw.fileRead(argv[1]);
        iw.updateFileName(argv[1]);
        cw.updateFileName(argv[1]);
    }else {
        if (argv[1][0] != '-') {
            fprintf(stderr, "the command should be: minivim-bin [options] <filename>\n");
            return 0;
        }
        
        /*-t  clear the file before writing*/
        /*-R read only*/

        while ((ch = getopt(argc, argv, "t:R:")) != -1) {
            if (num == 1) {
                /*only one command can be used a time*/
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

            /*the ch is ? and the error will be reported by getopt*/
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
    fw.fileRead("/home/qxw/minivim/abc");
    iw.updateFileName("/home/qxw/minivim/abc");
    cw.updateFileName("/home/qxw/minivim/abc");
    #endif

    /*where the promgram actually begins*/
    while(call_back(cw,fw,iw));
    return 0;
}