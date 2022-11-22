#include <ncurses.h>
#include "command_window.h"
#include "file_window.h"
#include "information_window.h"


#define CUS_COLOR_NUM 2

void call_back (char ch) {

}

int main() {
    command_window cw;
    file_window fw;
    information_window iw;

//  /*
//     wmove(win, 2, 2);
//     wprintw(win, "Hello, world!"); /* Print Hello World */
//     mvwprintw(win, 3, 2, "This is MiniVim, your input will appear on the next line."); /*Move and Print*/
//     wrefresh(win); /* Print it on to the real screen */

//     wmove(win, 4, 2);
//     int ch = 0;
//     // get char in loop until get `Esc`
//     while ((ch = getch()) != 27) {
//  //       wprintw(win, "%c", ch);
//  //       wrefresh(win);
//     }
//     endwin(); /* End curses mode */
    return 0;
}