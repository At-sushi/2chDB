#include "2chDB.h"
#include <boost/asio.hpp>

class TCPServer
{
private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::acceptor acceptor;

    void onAccept(boost::asio::ip::tcp::socket &socket, const boost::system::error_code &error);
    bool processRequest(const std::string &request, boost::asio::ip::tcp::socket &socket);
    // Function to handle authentication
    bool authenticate(boost::asio::ip::tcp::socket& socket);

public:
    explicit TCPServer(boost::asio::io_context &io_context);
    ~TCPServer();

    void startAccept();
};
