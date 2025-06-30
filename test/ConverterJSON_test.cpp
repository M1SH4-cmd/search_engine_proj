#include <gtest/gtest.h>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

// InvertedIndex
TEST(TestInvertedIndex, BasicCountTest) {
    std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };

    InvertedIndex index;
    index.UpdateDocumentBase(docs);

    EXPECT_EQ(index.GetWordCount("milk"), (std::vector<Entry>{
            {0, 4}, {1, 1}, {2, 5}
    }));

    EXPECT_EQ(index.GetWordCount("water"), (std::vector<Entry>{
            {0, 3}, {1, 2}, {2, 5}
    }));

    EXPECT_EQ(index.GetWordCount("americano"), (std::vector<Entry>{
            {3, 1}
    }));

    EXPECT_TRUE(index.GetWordCount("tea").empty());
}

// SearchServer
TEST(TestSearchServer, SimpleRelevanceTest) {
    std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };

    InvertedIndex index;
    index.UpdateDocumentBase(docs);

    SearchServer server(index);

    std::vector<std::string> queries = {
            "milk water",  // есть совпадения
            "tea"          // ничего не найдено
    };

    auto results = server.search(queries);

    ASSERT_EQ(results.size(), 2);

    EXPECT_EQ(results[0].size(), 3); // 3 документа с релевантностью
    EXPECT_EQ(results[0][0].first, 2); // doc 2 — самый релевантный
    EXPECT_NEAR(results[0][0].second, 1.0f, 0.001); // максимальная релевантность

    EXPECT_TRUE(results[1].empty()); // ничего не найдено
}

// ConverterJSON
TEST(TestConverterJSON, ConfigReading) {
    ConverterJSON conv;

    EXPECT_NO_THROW({
        auto docs = conv.get_text_documents();
        auto requests = conv.get_requests();
        int limit = conv.get_responds_limit();
        EXPECT_GT(docs.size(), 0);
        EXPECT_GT(requests.size(), 0);
        EXPECT_GT(limit, 0);
    });
}

// put_answers()
TEST(TestConverterJSON, WriteAnswersTest) {
    ConverterJSON conv;

    std::vector<std::vector<std::pair<int, float>>> answers = {
            { {0, 1.0f}, {2, 0.7f} },
            {},
            { {1, 1.0f} }
    };

    EXPECT_NO_THROW(conv.put_answers(answers));

    std::ifstream in("../JSON/answers.json");
    EXPECT_TRUE(in.is_open());
    in.close();
}
