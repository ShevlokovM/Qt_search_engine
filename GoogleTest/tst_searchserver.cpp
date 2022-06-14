#include <memory>
#include "gtest/gtest.h"
#include "../include/searchserver.h"

TEST(TestCaseSearchServer, TestSimple)
{
    const std::vector<std::string> docs =
    {
        "milk milk milk milk water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "Americano Cappuccino sugar"
    };

    auto docss = std::make_shared< std::vector<std::string> >(docs);
    const std::vector<std::string> request = {"milk water", "sugar"};
    const std::vector<std::vector<std::pair<int, float> > > expected =
    {
        {
            {2, 1}, {0, 0.6}, {1, 0.3}
        },

        {
            {3, 1}
        }
    };

    auto idx = std::make_shared<InvertedIndex>();
    idx->UpdateDocumentBase(docss);
    SearchServer srv(idx);
    auto result = srv.Search(request);

    ASSERT_EQ(*result, expected);
}

TEST(TestCaseSearchServer, TestTop5)
{
    const std::vector<std::string> docs =
    {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };

    auto docss = std::make_shared< std::vector<std::string> >(docs);
    const std::vector<std::string> request = {"moscow the capital of russia"};
    const std::vector<std::vector<std::pair<int, float> > > expected =
    {
        {
            {14, 1}, {7, 0.625}
        }
    };

    auto idx = std::make_shared<InvertedIndex>();
    idx->UpdateDocumentBase(docss);
    SearchServer srv(idx);
    auto result = srv.Search(request);

    ASSERT_EQ(*result, expected);
}
