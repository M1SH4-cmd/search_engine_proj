#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#ifdef UNIT_TESTS
// Режим тестирования - пустая main()
int main() {
    std::cout << "Tests are running (no app logic here)" << std::endl;
    return 0;
}
#else
// Режим обычной работы
int main() {
    try {
        ConverterJSON converter;
        std::vector<std::string> docs = converter.get_text_documents();
        std::vector<std::string> requests = converter.get_requests();

        InvertedIndex index;

        index.UpdateDocumentBase(docs);
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