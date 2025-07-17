#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <filesystem>

#ifdef UNIT_TESTS
// Режим тестирования - пустая main()
int main() {
    std::cout << "Tests are running (no app logic here)" << std::endl;
    return 0;
}
#else
// Режим обычной работы
int main() {

    std::ifstream f1("../JSON/config.json");
    if (!f1.is_open()) {
        std::cerr << "Error opening config.json!" << std::endl;
        return 1;
    } else {
        std::cout << "File config.json loaded successfully" << std::endl;
    }
    f1.close();

    std::ifstream f2("../JSON/requests.json");
    if (!f2.is_open()) {
        std::cerr << "Error opening config.json!" << std::endl;
        return 1;
    } else {
        std::cout << "File config.json loaded successfully" << std::endl;
    }
    f2.close();

    std::string resDir = "../resources";
    if (std::filesystem::is_empty(resDir)) {
        std::cerr << "Resources directory is empty!" << std::flush;
    }


    try {
        ConverterJSON converter;
        std::vector<std::string> docs = converter.get_text_documents();
        std::vector<std::string> requests = converter.get_requests();

        InvertedIndex index;

        index.update_document_base(docs);
        SearchServer server(index);
        auto results = server.search(requests);

        converter.put_answers(results);

        std::cout << "Search finished. Answers were written to answers.json" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

#endif