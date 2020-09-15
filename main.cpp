#include <iostream>

#include "wordweb.hpp"

int main()
{
    WordWeb dict("pesky.txt");

    std::string word = dict.GetRandomStartWord();
    int max_words = 20, word_count = 0;

    std::cout << "pesky: ";

    while (!word.empty() && word_count < max_words) {
        std::cout << word << ' ';
        word = dict.GetRandomNextWord(word);
    }

    std::cout << "\n";
}
