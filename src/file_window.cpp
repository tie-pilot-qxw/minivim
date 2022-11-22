#include "../include/file_window.h"
file_window::file_window():window(){
    win = newwin(5, COLS, 0, 0); /*create a 5*10 window in (0, 0)*/
    keypad(win,true);
    wrefresh(win);

    //wbkgd(win, COLOR_PAIR(2)); /*set customized window color*/
    wrefresh(win);

    wprintw(win, "Press Esc to Exit");
    wmove(win, 2, 2);
    wprintw(win, "Hello, world!"); /* Print Hello World */
    mvwprintw(win, 3, 2, "This is MiniVim, your input will appear on the next line."); /*Move and Print*/
    wrefresh(win); /* Print it on to the real screen */

    wmove(win, 4, 2);
}