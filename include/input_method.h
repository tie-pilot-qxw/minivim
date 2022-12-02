#pragma once
#include "window.h"
#include "trie.h"
#include <stack>
using Lexicon = trie::trie_map<char, trie::SetCounter>;
const int WORD_COMPLETE_COLOR = 2;
class input_method : public window {
    private:
        bool success;
        Lexicon dictionary;
        std::string prefix;
        std::string lowerPre;
        std::vector<std::string > wordC;
        int startWord;
        int nowWord;
        std::stack<int > st;
        void print();
        void updateWindowSize();
        void updateStartPos();
        void readFile();
        void getStr();
    public:
        input_method();
        void clear();
        void pageDown();
        void pageUp();
        bool initSuc();
        void update(const std::string &str);
        void nextWord();
        void lastWord();
        bool select();
        bool select(int num);
        void sleep();
        void wake();
        std::string getWord();
};