#include <gtest/gtest.h>
#include "ConverterJSON.h"
#include <filesystem>

TEST(ConverterJSONTest, ConfigFileContent) {
    std::ifstream cfg_f("config.json");
    ASSERT_TRUE(cfg_f.is_open()) << "config.json not found!";

    nlohmann::json data;
    EXPECT_NO_THROW(cfg_f >> data) << "Invalid JSON in config.json!";

    EXPECT_TRUE(data.contains("config")) << "No 'config' section";
    EXPECT_TRUE(data.contains("files")) << "No files list";
}

TEST(ConverterJSONTest, ConfigFileExists) {
    EXPECT_NO_THROW({
        ConverterJSON converter;
        auto docs = converter.get_text_documents();
        EXPECT_FALSE(docs.empty());
    });
}

TEST(ConverterJSONTest, ResponsesLimitValid) {
    ConverterJSON converter;
    int limit = converter.get_responds_limit();
    EXPECT_GT(limit, 0);
}

TEST(ConverterJSONTest, RequestsFileHandling) {
    ConverterJSON converter;
    EXPECT_NO_THROW({
        auto requests = converter.get_requests();
    });
}

TEST(ConverterJSONTest, AnswersFileGeneration) {
    ConverterJSON converter;

    EXPECT_NO_THROW(converter.put_answers(converter.search()));

    std::ifstream answer_f("answers.json");
    EXPECT_TRUE(answer_f.is_open());

    answer_f.seekg(0, std::ios::end);
    EXPECT_GT(answer_f.tellg(), 0);
}