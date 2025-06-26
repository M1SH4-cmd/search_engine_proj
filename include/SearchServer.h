#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include "InvertedIndex.h"
#include <algorithm>

struct RelativeIndex {
    size_t doc_id;
    float rank;

    bool operator==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && std::abs(rank - other.rank) < 1e-5);
    }
};

class SearchServer {
public:
    explicit SearchServer(InvertedIndex& idx) : _index(idx) {}

    std::vector<std::vector<std::pair<int, float>>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;
};



#endif //SEARCH_ENGINE_SEARCHSERVER_H
