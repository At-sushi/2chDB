// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"
#include <sstream>   // Add this include to fix E0070 error
#include <filesystem>
#include <unordered_set>
#include "TCPServer.h"

using boost::asio::ip::tcp;
namespace asio = boost::asio;

// ...

// struct FileHeader
// {
//     std::uint16_t magic;
//     std::uint16_t version;
//     std::uint32_t size;
// };

int main()
{
    init();

    asio::io_context io_context;
    TCPServer server(io_context);

    server.startAccept();
    std::cout << "Server started. Listening on port " << DEFAULT_PORT << std::endl;

    try {
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    server.deleteFlaggedFiles(); // Delete flagged files before exiting
    std::cout << "Server stopped." << std::endl;

    return 0;
}

// ...
