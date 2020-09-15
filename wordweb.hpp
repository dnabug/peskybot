#pragma once

#include <map>
#include <string>
#include <vector>

struct WordConnections
{
    std::map<std::string, unsigned int> connections; // Word, frequency of succession
    unsigned int succeeded_count; // total number of times succeeded
    unsigned int preceded_count; // total number of times preceded
};

typedef struct WordConnections WordConnections;

class WordWeb
{
public:
    WordWeb(std::string filename);

    void AddWordInstance(std::string word, std::string prev, std::string after);
    std::string GetRandomStartWord();
    std::string GetRandomNextWord(std::string word);

private:
    std::map<std::string, WordConnections> words;
    std::vector<std::string> start_words;
    unsigned int total_connections = 0;
};
