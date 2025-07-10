#include "SearchServer.h"

std::vector<std::vector<std::pair<int, float>>> SearchServer::search(const std::vector<std::string>& queries_input) {
    if (queries_input.empty()) return {};
    std::vector<std::vector<RelativeIndex>> results;

    for (const auto& query : queries_input) {
        std::map<size_t, float> doc_relevance;
        std::istringstream iss(query);
        std::string word;

        //все слова из запроса
        while (iss >> word) {
            for (const Entry& entry : _index.GetWordCount(word)) {
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        if (doc_relevance.empty()) {
            results.push_back({});
            continue;
        }

        float max_relevance = std::max_element(
                doc_relevance.begin(), doc_relevance.end(),
                [](const auto& a, const auto& b) {
                    return a.second < b.second;
                })->second;

        std::vector<RelativeIndex> rel_list;
        for (const auto& [doc_id, abs_rank] : doc_relevance) {
            rel_list.push_back({doc_id, abs_rank / max_relevance});
        }

        std::sort(rel_list.begin(), rel_list.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            if (a.rank != b.rank)
                return b.rank < a.rank;  // по убыванию rank
            return a.doc_id < b.doc_id;  // по возрастанию doc_id при равных rank
        });

        results.push_back(rel_list);
    }

    std::vector<std::vector<std::pair<int, float>>> converted;
    for (const auto& vec : results) {
        std::vector<std::pair<int, float>> line;
        for (const auto& rel : vec) {
            line.emplace_back(static_cast<int>(rel.doc_id), static_cast<float>(rel.rank));
        }
        converted.push_back(line);
    }

    return converted;
}
