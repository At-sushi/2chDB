// test_2chDB.cpp

#include <cppunit/extensions/HelperMacros.h>
#include "2chDB.h"

class 2chDBTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(2chDBTest);
    CPPUNIT_TEST(testQueryFromReadCGI);
    CPPUNIT_TEST(testWrites);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() override {
        // 初期化コード
        init();
    }

    void tearDown() override {
        // クリーンアップコード
    }

    void testQueryFromReadCGI() {
    }

    void testWrites() {
        constexpr char* TEST_STR = "Hello, 2chDB.";

        testWrite("news4vip", "1234567890", TEST_STR);
        // ファイルの内容を確認するコード
        const char* result = queryFromReadCGI("news4vip", "1234567890");
        CPPUNIT_ASSERT(result != nullptr);
        CPPUNIT_ASSERT_EQUAL(std::string(TEST_STR, std::string(result));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(2chDBTest);
