#include "ConverterJSON.h"
#include <iostream>

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
        converter.get_text_documents();

        converter.put_answers(converter.search());
        std::cout << "Search completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#endif