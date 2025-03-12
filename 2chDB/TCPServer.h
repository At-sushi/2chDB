#include "2chDB.h"
#include <boost/asio.hpp>

class TCPServer
{
private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    void onAccept(const boost::system::error_code &error);

public:
    explicit TCPServer(boost::asio::io_context &io_context);
    ~TCPServer();
};
