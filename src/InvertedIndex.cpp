#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    {
        std::lock_guard<std::mutex> lock(dict_mutex);
        docs = input_docs;
        freq_dictionary.clear();
    }

    std::vector<std::thread> threads;
    for (size_t i = 0; i < input_docs.size(); ++i) {
        threads.emplace_back(&InvertedIndex::indexate, this, std::cref(input_docs), i);
    }

    for (auto& t : threads) {
        t.join();
    }
}


void InvertedIndex::indexate(const std::vector<std::string> &input_docs, size_t i) {

    std::map<std::string, size_t> word_count;
    std::istringstream iss(input_docs[i]);
    std::string word;

    while (iss >> word) {
        ++word_count[word];
    }

    std::lock_guard<std::mutex> lock(dict_mutex);
    for (const auto& [word, count] : word_count) {
        freq_dictionary[word].push_back(Entry{ i, count});
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::lock_guard<std::mutex> lock(dict_mutex);
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        auto result = it->second;
        std::sort(result.begin(), result.end(), [](const Entry& a, const Entry& b) {
            return a.doc_id < b.doc_id;
        });
        return result;
    }
    return {};
}
