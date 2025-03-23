#include <memory>

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

    for (;;) {
        std::string buffer;
        boost::system::error_code ec;
        asio::read(socket, asio::buffer(buffer), ec);

        if (ec)
        {
            std::cerr << "Failed to read: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Received: " << buffer << std::endl;
            processRequest(buffer);
        }
    }
}

void TCPServer::processRequest(const std::string &request)
{
}

bool TCPServer::authenticate(tcp::socket& socket)
{
    asio::write(socket, asio::buffer("Please specify your username: "));
    std::string username;
    std::getline(std::cin, username);
    return true;
}
