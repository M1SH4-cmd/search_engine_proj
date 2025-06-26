#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    for (size_t i = 0; i < docs.size(); ++i) {
        std::map<std::string, size_t> word_count;
        std::istringstream iss(docs[i]);
        std::string word;

        while (iss >> word) {
            ++word_count[word];
        }

        for (const auto& [word, count] : word_count) {
            freq_dictionary[word].push_back(Entry{ i, count });
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) {
        return freq_dictionary[word];
    } else {
        return {};
    }
}
