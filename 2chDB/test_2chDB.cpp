// test_2chDB.cpp

#include <cppunit/extensions/HelperMacros.h>
#include "2chDB.h"
#include <filesystem>

class 2chDBTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(2chDBTest);
    CPPUNIT_TEST(testQueryFromReadCGI);
    CPPUNIT_TEST(testWrites);
    CPPUNIT_TEST_SUITE_END();

public:
    constexpr char* TEST_STR = "Hello, 2chDB.",
                    TEST_BBS = "news4vip";

    void setUp() override {
        // 初期化コード
        init();
        std::filesystem::create_directories(std::format("{}/dat", TEST_BBS));
    }

    void tearDown() override {
        // クリーンアップコード
        std::filesystem::remove_all(TEST_BBS);
    }

    void testQueryFromReadCGI() {
    }

    void testWrites() {
        testWrite(TEST_BBS, "1234567890", TEST_STR);
        // ファイルの内容を確認するコード
        const char* result = queryFromReadCGI(TEST_BBS, "1234567890");
        CPPUNIT_ASSERT(result != nullptr);
        CPPUNIT_ASSERT_EQUAL(std::string(TEST_STR, std::string(result));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(2chDBTest);
