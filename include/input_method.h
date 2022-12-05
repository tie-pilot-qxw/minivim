#pragma once
#include "window.h"
#include "trie.h"
#include <stack>
using Lexicon = trie::trie_map<char, trie::SetCounter>;
const int WORD_COMPLETE_COLOR = 2;
class input_method : public window {
    private:
        bool success;/*whether the word dictionary has been read successfully*/
        Lexicon dictionary;
        std::string prefix;/*the prefix of the current word*/
        std::string lowerPre;/*the lower-case of the prefix*/
        std::vector<std::string > wordC;/*the words which have been found*/
        int startWord;/*the begining id(in trie) of the current words on screen*/
        int nowWord;/*which word is the curse on*/
        std::stack<int > st;/*the amount of word last page has*/
        void print();
        void updateWindowSize();
        void updateStartPos();
        void readFile();/*read the dictionary txt*/
        void getStr();/*get the str form trie*/
    public:
        input_method();
        void clear();/*clear the input_method window*/
        void pageDown();
        void pageUp();
        bool initSuc();
        void update(const std::string &str);/*update the prefix*/
        void nextWord();
        void lastWord();
        bool select();/*select with enter key*/
        bool select(int num);/*select with the number key*/
        void sleep();/*hide the window*/
        void wake();/*refresh the window*/
        std::string getWord();/*get the suffix of the word*/
};