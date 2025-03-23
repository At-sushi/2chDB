#include <memory>
#include <sstream>

#include "TCPServer.h"

namespace asio = boost::asio;
using asio::ip::tcp;

constexpr int DEFAULT_PORT = 8082;

TCPServer::TCPServer(asio::io_context &io_context)
    : io_context(io_context)
    , acceptor(io_context, tcp::endpoint(tcp::v4(), DEFAULT_PORT))
{
    startAccept();
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

    std::string buffer;
    boost::system::error_code ec;

    for (;;) {
        asio::read(socket, asio::buffer(buffer), ec);

        if (ec)
        {
            std::cerr << "Failed to read: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Received: " << buffer << std::endl;
            if (!processRequest(buffer))
                break;
        }
    }
}

bool TCPServer::processRequest(const std::string &request)
{
    std::istringstream iss(request);
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
        
        std::cout << queryFromReadCGI(bbs.c_str(), key.c_str()) << std::endl;
    }
    else if (command == "set") {
        std::string bbs, key, source;

        iss >> bbs >> key;
        std::getline(iss, source);

        testWrite(bbs.c_str(), key.c_str(), source.data());
    }
    else if (command == "del")
    {
        std::string bbs, key;

        iss >> bbs >> key;

        testWrite(bbs.c_str(), key.c_str(), "");
    }
    else if (command == "create") {
        std::string bbs;

        iss >> bbs;

        const auto directory = std::format("{}/dat", bbs);

        if (std::filesystem::create_directories(directory))
            std::cout << "Created" << std::endl;
    }
    else if (command == "remove") {
        std::string bbs;

        iss >> bbs;
        std::filesystem::remove_all(bbs);
        std::cout << "Remove Completed" << std::endl;
    }

    return true;
}

bool TCPServer::authenticate(tcp::socket& socket)
{
    asio::write(socket, asio::buffer("Please specify your username: "));
    std::string username;
    std::getline(std::cin, username);
    return true;
}
