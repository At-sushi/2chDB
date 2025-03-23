#include "2chDB.h"
#include <boost/asio.hpp>

class TCPServer
{
private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    void startAccept();
    void onAccept(boost::asio::ip::tcp::socket &socket, const boost::system::error_code &error);
    void processRequest(const std::string &request);
    bool authenticate(boost::asio::ip::tcp::socket& socket);

public:
    explicit TCPServer(boost::asio::io_context &io_context);
    ~TCPServer();
};
