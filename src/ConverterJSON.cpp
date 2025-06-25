#include "ConverterJSON.h"

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    std::ifstream cfg_f("config.json");
    if (!cfg_f.is_open()) {
        std::cerr << "Failed to open config.json file!" << std::endl;
        return;
    }

    try {
        json cfg_data = json::parse(cfg_f);
        name = cfg_data["config"]["name"];
        version = cfg_data["config"]["version"];
        max_responses = cfg_data["config"]["max_responses"];
        files = cfg_data["files"].get<std::vector<std::string>>();
        cfg_f.close();
    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    std::ifstream requests_f("requests.json");
    if (!requests_f.is_open()) {
        std::cerr << "Failed to open config.json file!" << std::endl;
        return;
    }

    try {
        json requests_data = json::parse(requests_f);
        requests = requests_data["requests"].get<std::vector<std::string>>();
        requests_f.close();
    } catch (const json::exception& e){
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

std::vector<std::string> ConverterJSON::get_text_documents() {
    std::vector<std::string> texts;
    std::string line;
    for(int i = 0; i < files.size(); i++){
        std::ifstream f(files[i]);
        while(true){
            line = "";
            std::getline(f, line);
            texts.push_back(line);
            if(f.eof()) {
                f.close();
                break;
            }
        }

    }
    return texts;
}

int ConverterJSON::get_responds_limit() const {
    return max_responses;
}

std::vector<std::string> ConverterJSON::get_requests() {
    return requests;
}

void ConverterJSON::build_index() {
    for (size_t doc_id = 0; doc_id < files.size(); ++doc_id) {
        std::ifstream file(files[doc_id]);
        std::string word;

        while (file >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            auto& entries = word_index[word];
            auto it = std::find_if(entries.begin(), entries.end(), [doc_id](const auto& pair) { return pair.first == doc_id; });

            if (it != entries.end()) {
                it->second++;
            } else {
                entries.emplace_back(doc_id, 1);
            }
        }
    }
}

    const std::vector<std::vector<std::pair<int, float>>> ConverterJSON::search() {
    build_index();

    std::vector<std::vector<std::pair<int, float>>> results;

    for (const auto& query : requests) {
        std::istringstream iss(query);
        std::string word;
        std::map<int, float> doc_rank; // doc_id -> суммарный ранг

        while (iss >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            if (word_index.count(word)) {
                for (const auto& [doc_id, count] : word_index[word]) {
                    doc_rank[doc_id] += count;
                }
            }
        }

        // Нормализация рангов
        float max_rank = 0;
        for (const auto& [_, rank] : doc_rank) {
            if (rank > max_rank) max_rank = rank;
        }

        std::vector<std::pair<int, float>> query_result;
        for (const auto& [doc_id, rank] : doc_rank) {
            query_result.emplace_back(doc_id, max_rank ? rank / max_rank : 0);
        }

        // Сортировка
        std::sort(query_result.begin(), query_result.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });

        if (max_responses > 0 && query_result.size() > max_responses) {
            query_result.resize(max_responses);
        }

        results.push_back(query_result);
    }

    return results;
}

void ConverterJSON::put_answers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json resultJson;
    json answersObj;

    for (size_t i = 0; i < answers.size(); ++i) {
        std::string requestId = "request" + std::to_string(i + 1);
        requestId = std::string(3 - requestId.length() % 3, '0') + requestId; //ведущие нули

        json responseObj;

        if (answers[i].empty()) {
            responseObj["result"] = "false";
        } else if (answers[i].size() == 1) {
            responseObj["result"] = "true";
            responseObj["docid"] = answers[i][0].first;
            responseObj["rank"] = answers[i][0].second;
        } else {
            responseObj["result"] = "true";
            json relevanceArray = json::array();

            for (const auto& [doc_id, rank] : answers[i]) {
                relevanceArray.push_back({
                                                 {"docid", doc_id},
                                                 {"rank", rank}
                                         });
            }

            responseObj["relevance"] = relevanceArray;
        }

        answersObj[requestId] = responseObj;
    }

    resultJson["answers"] = answersObj;

    std::ofstream answers_f("answers.json");
    if (answers_f.is_open()) {
        answers_f << resultJson.dump(4);
    } else {
        std::cerr << "Error: Could not create/open answers.json" << std::endl;
    }
}