#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cmath>

struct Entry {
    size_t doc_id;
    size_t count;

    bool operator==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;
    void indexate(const std::vector<std::string> &docs, size_t i);
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);
    std::vector<Entry> GetWordCount(const std::string& word);

private:
    std::vector<std::string> docs; // хранилище всех документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // инвертированный индекс
    std::mutex dict_mutex;

};



#endif //SEARCH_ENGINE_INVERTEDINDEX_H
