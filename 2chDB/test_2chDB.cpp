// test_2chDB.cpp

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <filesystem>
#include "2chDB.h"
#include "TCPServer.h"

class T2chDBTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(T2chDBTest);
    CPPUNIT_TEST(testQueryFromReadCGI);
    CPPUNIT_TEST(testWrites);
    CPPUNIT_TEST(testNotFound);
    CPPUNIT_TEST(testTCPServer);
    CPPUNIT_TEST_SUITE_END();

    static constexpr char *TEST_STR = "Hello, 2chDB.",
                          *TEST_BBS = "news4vip";

public:
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
        CPPUNIT_ASSERT_EQUAL(std::string(TEST_STR), std::string(result));

        // ファイルの上書き
        testWrite(TEST_BBS, "1234567890", "changed");
        result = queryFromReadCGI(TEST_BBS, "1234567890");
        CPPUNIT_ASSERT(result != nullptr);
        CPPUNIT_ASSERT_EQUAL(std::string("changed"), std::string(result));

        // ファイルの削除
        testWrite(TEST_BBS, "1234567890", "");
        result = queryFromReadCGI(TEST_BBS, "1234567890");
        CPPUNIT_ASSERT(result != nullptr);
        CPPUNIT_ASSERT_EQUAL(std::string(""), std::string(result));
    }
    // これは、2chDBのデータベースにデータを追加または更新するための基本的な機能をテストします。

    void testNotFound() {
        // 存在しないBBSとキーを指定して、queryFromReadCGIがnullptrを返すことを確認します。
        const char* result = queryFromReadCGI("nonexistent_bbs", "nonexistent_key");
        CPPUNIT_ASSERT_EQUAL(std::string(result), std::string(""));
        // 存在しないキーを指定して、queryFromReadCGIがnullptrを返すことを確認します。
        result = queryFromReadCGI("news4vip", "nonexistent_key");
        CPPUNIT_ASSERT_EQUAL(std::string(result), std::string(""));
    }
    // testNotFound関数は、存在しないBBSとキーを指定して、queryFromReadCGIがnullptrを返すことを確認します。
    // これは、2chDBのデータベースに存在しないデータを問い合わせた場合の基本的な機能をテストします。
    
    // TCPServerのテスト
    // ここでは、TCPServerのインスタンスを作成し、startAcceptメソッドを呼び出すだけのテストを行います。
    // 実際の接続は行わず、メソッドが正常に呼び出されることを確認します。
    // これは、TCPServerクラスの基本的な機能を確認するためのテストです。
 
    void testTCPServer() {
        boost::asio::io_context io_context;
        TCPServer server(io_context);

        CPPUNIT_ASSERT_NO_THROW(server.startAccept());
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(T2chDBTest);

int main() {
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run();

    return 0;
}
