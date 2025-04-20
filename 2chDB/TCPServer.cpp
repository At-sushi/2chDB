#include <memory>
#include <sstream>

#include "TCPServer.h"

namespace asio = boost::asio;
using asio::ip::tcp;

TCPServer::TCPServer(asio::io_context &io_context, int port)
    : io_context(io_context)
    , acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
}

TCPServer::~TCPServer()
{
    acceptor.close();
}

void TCPServer::startAccept()
{
    auto socket = std::make_shared<tcp::socket>(io_context);

    acceptor.async_accept(*socket, [this, socket](const boost::system::error_code &error) { onAccept(*socket, error); });
}

void TCPServer::onAccept(tcp::socket& socket, const boost::system::error_code &error)
{
    startAccept();

    if (error)
    {
        std::cerr << "Failed to connect: " << error.message() << std::endl;
        return;
    }

    std::cout << "Connected from: " << socket.remote_endpoint() << std::endl;

    if (!authenticate(socket))
    {
        std::cerr << "Authentication failed" << std::endl;
        return;
    }

    asio::streambuf response;
    boost::system::error_code ec;

    for (;;) {
        asio::read_until(socket, response, "\n", ec);
 
        if (ec == asio::error::eof) {
            break; // Connection closed cleanly by peer
        }
        else if (ec == asio::error::operation_aborted) {
            break; // Operation was aborted
        }
        else if (ec) {
            std::cerr << "Failed to read: " << ec.message() << std::endl;
            break;
        }
        else {
            if (!processRequest(std::istream(&response), socket))
                break;
        }
    }

    std::cout << "Disconnected: " << socket.remote_endpoint() << std::endl;
}

bool TCPServer::processRequest(std::istream iss, tcp::socket& socket)
{
    std::string command;

    iss >> command;
    for (auto &&i : command)
        i = std::tolower(i);

    if (command == "exit") {
        return false;
    }
    else if (command == "get") {
        std::string bbs, key;

        iss >> bbs >> key;
        
        std::string result = queryFromReadCGI(bbs.c_str(), key.c_str());
        if (result.empty())
            result = "Not Found";
        socket.send(asio::buffer(result + "\n"));
    }
    else if (command == "set") {
        std::string bbs, key, source;

        iss >> bbs >> key;
        std::getline(iss, source);

        testWrite(bbs.c_str(), key.c_str(), source.data());
        deleteFlag.erase(bbs, key);

        socket.send(asio::buffer("Written\n"));
    }
    else if (command == "del")
    {
        std::string bbs, key;

        iss >> bbs >> key;

        testWrite(bbs.c_str(), key.c_str(), "");
        deleteFlag.add(bbs, key);

        socket.send(asio::buffer("Deleted\n"));
    }
    else if (command == "create") {
        std::string bbs;

        iss >> bbs;

        const auto directory = std::format("{}/dat", bbs);

        if (std::filesystem::create_directories(directory))
            socket.send(asio::buffer("Created\n"));
    }
    else if (command == "remove") {
        std::string bbs;

        iss >> bbs;
        std::filesystem::remove_all(bbs);
        socket.send(asio::buffer("Remove Completed\n"));
    }
    else if (command == "ls") {
        std::string bbs;

        iss >> bbs;
        std::ostringstream oss;
        for (const auto& entry : std::filesystem::directory_iterator(bbs)) {
            oss << entry.path().stem().string() << "\n";
        }
        socket.send(asio::buffer(oss.str()));
    }
    else if (command == "help") {
        const std::string help_message = "Available commands:\n"
                                    "get <bbs> <key> - Get data\n"
                                    "set <bbs> <key> <data> - Set data\n"
                                    "del <bbs> <key> - Delete data\n"
                                    "create <bbs> - Create directory\n"
                                    "remove <bbs> - Remove directory\n"
                                    "ls <bbs> - List files in directory\n"
                                    "help - Show this help message\n"
                                    "gc - Run garbage collection\n"
                                    "exit - Exit the server\n";
        socket.send(asio::buffer(help_message));
    }
    else if (command == "gc") {
        deleteFlag.flush();
        socket.send(asio::buffer("Garbage collection completed\n"));
    }
    else {
        socket.send(asio::buffer("Unknown command\n"));
    }

    return true;
}

bool TCPServer::authenticate(tcp::socket& socket)
{
    asio::write(socket, asio::buffer("Please specify your username: "));
    std::string username;
    return true;
}
