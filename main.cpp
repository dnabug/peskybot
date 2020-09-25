#include <iostream>

#include "wordweb.hpp"

int generate_line(WordWeb dict, int max_words, std::vector<std::string> req_words)
{
    std::string word = dict.GetRandomStartWord();
    std::vector<std::string> words;

    while (!word.empty() && words.size() < max_words) {
        word = dict.GetRandomNextWord(word);
        if (word.empty() && words.size() < 5) {
            word = dict.GetRandomStartWord();
        }
        words.push_back(word);

        for (int i = 0; i < req_words.size(); i++) {
            if (req_words[i] == word) {
                req_words.erase(req_words.begin() + i);
                break;
            }
        }
    }

    if (req_words.size() > 0) return 1;

    std::cout << "pesky says: ";
    for (std::string w : words) {
        std::cout << w << ' ';
    }
    std::cout << "\n";
    return 0;
}


int main(int argc, char** argv)
{
    WordWeb dict("pesky.txt");

    std::vector<std::string> required_words;
    int max_words = 50;
    int lines = 1;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::string arg(argv[i]);
            if (arg == "-n" && i < argc - 1) {
                try {
                    lines = std::stoi(std::string(argv[i + 1]));
                } catch (const std::invalid_argument& e) {
                    std::cout << e.what() << "\n";
                    std::cout << "Invalid argument given for number of lines\n";
                    return 1;
                }
                i++;
            } else required_words.push_back(arg);
        }
    }

    for (int i = 0; i < lines; i++) {
        while (generate_line(dict, max_words, required_words));
    }
}
