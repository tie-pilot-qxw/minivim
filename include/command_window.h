#pragma once
#include "window.h"
#include <string>
#include <vector>
const int WARNING_COLOR = 1;
class command_window : public window {
    private:
        std::string fileName;
        std::vector<std::string > *fileText;
        std::string commandText;/*the current command*/
        std::vector<std::string > commandHistory;

        bool quit;/*whether to quit command mode*/

        int historyNum;/*the id of the current command in the command history*/

        int pos;/*curse pos*/

        void print();
        void changeMouse();
        void updateWindowSize();
        void updateStartPos();

        /*move the curse*/
        void moveLeft();
        void moveRight();

        /*modify the command*/
        void backSpace();
        void deleteChar();
        void insertChar(int ch);
        
        bool check();/*tackle the command*/

        void clear();
        
        void saveFile();
    public:
        command_window();
        bool tackle();
        void init(std::vector<std::string > *file);
        void updateFileName(const std::string &str);
};