#pragma once
#include "window.h"
#include "input_method.h"
#include <string>
#include <vector>

class file_window : public window {
    private:
        input_method wordMap;
        std::vector<std::string > fileText;

        /*the pos of curse in different coordiantes*/
        POS absolutePos;
        POS inFilePos;
        POS realPos;

        /*where to start printing*/
        POS printBegin;
        POS realPrintBegin;

        int dNum;/*to tackle the command 'dd'*/

        bool atBottom;/*whether the screen can show the bottom of the file*/

        bool canWrite;/*read only mode*/

        bool isTrun;/*truncation mode*/

        int prefixLength;/*the length of the current prefix*/

        /*to show the window*/
        void print();/*update the window*/
        void changeMouse();/*update the curse*/

        /*tackle different modes*/
        bool normal (int ch);
        void insert (int ch);

        /*move the curse*/
        bool moveLeft(bool flush = true);
        bool moveRight(bool flush = true);
        bool moveUp(bool flush = true);
        bool moveDown(bool flush = true);

        /*change between coordiantes*/
        void getInFilePos();
        void getRealPos();
        int getLineNum(int l);
        POS fileToReal(POS fp);

        /*scroll the page*/
        bool lineMoveLeft();/*when the sentence is very long*/
        bool lineMoveRight();
        bool sentenceMoveDown();/*when the page needs to be turned*/
        bool sentenceMoveUp();
        int turnLimit();/*when to scroll the page*/
        void getRealPrintBegin();

        /*jump in the file*/
        void nextWordHead();
        void lastWordHead();
        bool lineHead(bool flush = true);
        bool lineTail(bool flush = true);
        bool isWord(char ch);

        /*modify the file*/
        void deleteLine();
        void insertChar(int ch);
        void backSpace();
        void deleteChar();
        void newLine();

        void getPerfix(std::string &str);
        void updateWindowSize();
    public:
        file_window();
        
        bool keyboard();/*the return value tells you whether to change to command mode or not*/

        void fileRead(const std::string &fname);/*read the file*/

        std::vector<std::string >* getFile();/*pass the file text*/

        POS getCurrentPos();/*get the curse pos*/
        
        /*command line arguments*/
        void readOnly();
        void truncation();
};