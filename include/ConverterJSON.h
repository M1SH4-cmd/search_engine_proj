#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
#include <iomanip>

class ConverterJSON {
private:
    std::string name;
    std::string version;
    int max_responses;
    std::vector<std::string> files;
    std::vector<std::string> requests;


public:
    ConverterJSON();
    std::vector<std::string> get_text_documents();
    int get_responds_limit() const;
    std::vector<std::string> get_requests();
    void put_answers(const std::vector<std::vector<std::pair<int, float>>>& answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
