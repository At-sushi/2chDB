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

struct BBSKey {
    std::string bbs;
    std::string key;

    auto operator <=> (const BBSKey&) const = default;
};

template <> struct std::hash<BBSKey>{
    using argument_type = BBSKey;
    using result_type = std::size_t;

    result_type operator()(const argument_type &arg) const noexcept
    {
        return std::hash<std::string>{}(arg.bbs) ^ std::hash<std::string>{}(arg.key);
    }
};

int main()
{
    init();

    asio::io_context io_context;
    TCPServer server(io_context);

    server.startAccept();
    // std::cout << "Server started. Listening on port " << DEFAULT_PORT << std::endl;

    try {
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

// ...
