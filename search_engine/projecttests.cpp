#include<memory>

#include "gtest/gtest.h"

#include "invertedindex.h"

using namespace std;
void TestInvertedIndexFunctionality(shared_ptr<vector<string> > docs, const vector<string>& requests,
const std::vector<vector<Entry>>& expected)
{
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
        for(auto& request : requests)
        {
            std::vector<Entry> word_count = idx.GetWordCount(request);
            result.push_back(word_count);
        }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic)
{
    auto docs = make_shared< vector<string> >();
    docs->push_back("london is the capital of great britain");
    docs->push_back("big ben is the nickname for the Great bell of the striking clock");

    const vector<string> requests = {"london", "the"};

    const vector<vector<Entry>> expected = {
    {
    {0, 1}
    }, {{
    0, 1}, {1, 3}
    }
    };

    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2)
{

    auto docs = make_shared< vector<string> >();
    docs->push_back("milk milk milk milk water water water");
    docs->push_back("milk water water");
    docs->push_back("milk milk milk milk milk water water water water water");
    docs->push_back("Americano Cappuccino");

const vector<string> requests = {"milk", "water", "cappuchino"};
const vector<vector<Entry>> expected = {
{
{0, 4}, {1, 1}, {2, 5}
}, {
{0, 2}, {1, 2}, {2, 5}
}, {
{3, 1}
}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}


TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord)
{
    auto docs = make_shared< vector<string> >();
    docs->push_back("a b c d e f g h i j k l");
    docs->push_back("statement");

const vector<string> requests = {"m", "statement"};
const vector<vector<Entry>> expected = {
{
{}
}, {
{1, 1}
}
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
