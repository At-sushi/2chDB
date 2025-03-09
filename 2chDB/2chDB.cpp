// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"
#include <sstream>   // Add this include to fix E0070 error
#include <algorithm> // Add this include to use std::transform
#include <filesystem>
#include <concepts>
#include <unistd.h>
#include <boost/asio.hpp>

extern "C"
{
#include "readcgi/read.h"
    extern int rawmode, isbusytime;
    extern void atexitfunc();
}

void init();
void testWrite(const char *bbs, const char *key, const char *source);
const char *queryFromReadCGI(const char *bbs, const char *key);

template <typename T>
    requires std::is_convertible_v<T, std::string_view>
inline static constexpr std::string create_fname(const T &bbs, const T &key)
{
    // TODO: sanitize bbs and key
    return std::format("{}/dat/{}.dat", bbs, key);
}

// ...

int main()
{
    init();
    testWrite("news4vip", "1234567890", "Hello, 2chDB.");

    std::cout << "Hello CMake." << std::endl;
    std::cout << queryFromReadCGI("news4vip", "1234567890") << std::endl;

    std::string line, command;
    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line); // This line is now valid

        iss >> command;
        for (auto &&i : command)
            i = std::tolower(i);

        if (command == "exit") {
            break;
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
        else if (command == "create") {
            std::string bbs;

            iss >> bbs;

            const auto directory = std::format("{}/dat", bbs);

            if (std::filesystem::create_directories(directory))
                std::cout << "Created" << std::endl;
        }
        else if (command == "remove") {
            iss >> bbs;
            std::filesystem::remove_all(bbs);
            std::cout << "Remove Completed" << std::endl;
            break;
        }
    }

    return 0;
}

// ...

void onAccept(const boost::system::error_code &error)
{
    if (error)
    {
        /* code */
        std::cerr << "Failed to connect: " << error.message() << std::endl;
        return;
    }
}

void init()
{
    rawmode = 1;
    isbusytime = 1;
    atexit(atexitfunc);

    // for the security
    chroot(".");
    if (getuid() == 0)
        std::cerr << "WARNING: Running as a root may cause several security issues." << std::endl;
}

const char *queryFromReadCGI(const char *bbs, const char *key)
{
    const std::string fname = create_fname(bbs, key);
    dat_read(fname.c_str(), 0, 0, 0);

    return BigBuffer;
}

void testWrite(const char *bbs, const char *key, const char *source)
{
    const std::string fname = create_fname(bbs, key);

    std::ofstream ofs(fname, std::ios_base::out | std::ios_base::binary);
    if (!ofs)
    {
        std::cerr << "Failed to open file: " << fname.data() << std::endl;
        return;
    }

    ofs << source;
}
