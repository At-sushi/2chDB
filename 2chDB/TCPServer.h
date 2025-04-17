#pragma once

#include "2chDB.h"
#include <boost/asio.hpp>
#include "DeleteFlag.h"


class TCPServer
{
private:
    boost::asio::io_context &io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    DeleteFlag deleteFlag; // DeleteFlagのインスタンスを追加

    void onAccept(boost::asio::ip::tcp::socket &socket, const boost::system::error_code &error);
    bool processRequest(const std::string &request, boost::asio::ip::tcp::socket &socket);
    // Function to handle authentication
    bool authenticate(boost::asio::ip::tcp::socket& socket);

public:
    explicit TCPServer(boost::asio::io_context &io_context, int port = DEFAULT_PORT);
    // Destructor to close the acceptor
    ~TCPServer();

    void startAccept();
    void deleteFlaggedFiles() {
        deleteFlag.flush(); // キャッシュから削除
    }
};
