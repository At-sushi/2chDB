#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_context &io_context)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 8080);

    acceptor = boost::asio::ip::tcp::acceptor(io_context, endpoint);
    acceptor.async_accept([this](const boost::system::error_code &error) { onAccept(error); });

    try {
        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

TCPServer::~TCPServer()
{
}

void TCPServer::onAccept(const boost::system::error_code &error)
{
    if (error)
    {
        std::cerr << "Failed to connect: " << error.message() << std::endl;
        return;
    }
}
