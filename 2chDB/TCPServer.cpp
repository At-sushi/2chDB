#include <memory>

#include "TCPServer.h"

namespace asio = boost::asio;
using asio::ip::tcp;

TCPServer::TCPServer(asio::io_context &io_context)
    : io_context(io_context)
    , acceptor(io_context, tcp::endpoint(tcp::v4(), 8080))
{
    startAccept();

    try {
        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
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
        }
    }
}
