#include "ConverterJSON.h"

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    std::ifstream cfg_f("../JSON/config.json");
    if (!cfg_f.is_open()) {
        std::cerr << "Failed to open config.json file!" << std::endl;
        exit(1);
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
        exit(1);
    }

    std::ifstream requests_f("../JSON/requests.json");
    if (!requests_f.is_open()) {
        std::cerr << "Failed to open requests.json file!" << std::endl;
        exit(1);
        return;
    }

    try {
        json requests_data = json::parse(requests_f);
        requests = requests_data["requests"].get<std::vector<std::string>>();
        requests_f.close();
    } catch (const json::exception& e){
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        exit(1);
    }
}

std::vector<std::string> ConverterJSON::get_text_documents() {
    std::vector<std::string> texts;

    if (files.empty()) return texts;

    for (const auto& file : files) {
        if (file.empty()) continue;

        std::ifstream f(file);
        if (!f.is_open()) continue;

        std::stringstream buffer;
        buffer << f.rdbuf();
        texts.push_back(buffer.str());
    }

    return texts;
}


int ConverterJSON::get_responds_limit() const {
    return max_responses;
}

std::vector<std::string> ConverterJSON::get_requests() {
    return requests;
}

void ConverterJSON::put_answers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json resultJson;
    json answersObj;

    for (int i = 0; i < answers.size(); ++i) {
        std::ostringstream oss;
        oss << "request" << std::setw(3) << std::setfill('0') << (i + 1);
        std::string requestId = oss.str();

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

    std::ofstream answers_f("../JSON/answers.json");
    if (answers_f.is_open()) {
        answers_f << resultJson.dump(4);
    } else {
        std::cerr << "Error: Could not create/open answers.json" << std::endl;
        exit(1);
    }
}