#include <iostream>

#include "wordweb.hpp"

int main()
{
    WordWeb dict("pesky.txt");

    std::string word = dict.GetRandomStartWord();
    int max_words = 50, word_count = 0;

    std::cout << "pesky says: ";

    while (!word.empty() && word_count < max_words) {
        std::cout << word << ' ';
        word = dict.GetRandomNextWord(word);
        if (word.empty() && word_count < 5) {
            word = dict.GetRandomStartWord();
        }
        word_count++;
    }

    std::cout << "\n";
}
