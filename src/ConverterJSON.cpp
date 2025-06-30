#include "ConverterJSON.h"

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    std::ifstream cfg_f("../JSON/config.json");
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

    std::ifstream requests_f("../JSON/requests.json");
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

    if(files.empty()) return texts;
    for(const auto & file : files){
        std::ifstream f(file);
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

    std::ofstream answers_f("../JSON/answers.json");
    if (answers_f.is_open()) {
        answers_f << resultJson.dump(4);
    } else {
        std::cerr << "Error: Could not create/open answers.json" << std::endl;
    }
}