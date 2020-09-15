#include "wordweb.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <time.h>
#include <cstdlib>

WordWeb::WordWeb(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        exit(1);
        return;
    }

    const std::streamsize len = 512;
    char* line = (char*) malloc(len);
    memset(line, 0, 512);

    while (file.getline(line, 512)) {
        std::string str(line);
        if (str.empty()) continue;

        std::vector<std::string> words;

        std::string::size_type start_pos = 0, end_pos = 0;

        while (end_pos < str.size()) {
            end_pos = str.find(' ', start_pos);
            if (end_pos == std::string::npos) end_pos = str.size();

            std::string word = str.substr(start_pos, end_pos - start_pos);
            if (word.empty()) {
                start_pos++;
                continue;
            }

            std::transform(word.begin(), word.end(), word.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            words.push_back(word);

            start_pos = end_pos + 1;
        }

        for (int i = 0; i < words.size(); i++) {
            std::string prev = (i == 0 ? std::string() : words[i-1]);
            std::string after = (i == words.size() - 1 ? std::string() : words[i + 1]);


            AddWordInstance(words[i], prev, after);
        }
    }

    free(line);

    file.close();
}

void WordWeb::AddWordInstance(std::string word, std::string prev, std::string after)
{
    WordConnections blank_wc = {
        std::map<std::string, unsigned int>(),
        0,
        0
    };

    words.try_emplace(word, blank_wc);

    if (!prev.empty()) {
        words[word].preceded_count++;
        total_preceded++;
        if (words[word].preceded_count > highest_preceded) {
            highest_preceded = words[word].preceded_count;
        }
    }

    if (!after.empty()) {
        words[word].succeeded_count++;
        words[word].connections.try_emplace(after, 0);

        unsigned int count = words[word].connections[after];
        words[word].connections[after] = count + 1;
    }
}


std::string WordWeb::GetRandomStartWord()
{
    std::srand(std::time(nullptr)); // Might swap this out for a more sophisticated
                                    // random number generator
    if (words.size() < 1) return std::string();

    unsigned int start_range = 0, end_range;
    unsigned int value = std::rand() % total_preceded; // May be biased, but don't care

    for (auto word_pair : words) {
        std::string word = std::get<0>(word_pair);
        end_range = start_range + highest_preceded - words[word].preceded_count + 1;

        if (value >= start_range && value <= end_range) {
            return word;
        }

        start_range = end_range + 1;
    }

    return std::string();
}

std::string WordWeb::GetRandomNextWord(std::string word)
{
    // See GetRandomStartWord
    if (!words.count(word)) return std::string();
    if (words[word].succeeded_count < 1) return std::string();

    unsigned int start_range = 0, end_range;
    unsigned int value = std::rand() % words[word].succeeded_count;
    for (auto next_word : words[word].connections) {
        end_range = start_range + std::get<1>(next_word) - 1;

        if (value >= start_range && value <= end_range) {
            return std::get<0>(next_word);
        }

        start_range = end_range + 1;
    }

    return std::string();
}
