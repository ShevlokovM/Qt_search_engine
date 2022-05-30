#include <QtTest>

// add necessary includes here

class SearchEngineTest : public QObject
{
    Q_OBJECT

public:
    SearchEngineTest();
    ~SearchEngineTest();

private slots:
    void test_case1();

};

SearchEngineTest::SearchEngineTest()
{

}

SearchEngineTest::~SearchEngineTest()
{

}

void SearchEngineTest::test_case1()
{

}

QTEST_APPLESS_MAIN(SearchEngineTest)

#include "tst_searchenginetest.moc"
