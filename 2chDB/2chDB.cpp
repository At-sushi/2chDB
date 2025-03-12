// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"
#include <sstream>   // Add this include to fix E0070 error
#include <filesystem>
#include <unordered_set>

// ...

int main()
{
    init();

    std::string line, command;
    // std::unordered_set<std::string> delete_list;
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
        else if (command == "del")
        {
            std::string bbs, key;

            iss >> bbs >> key;

            const auto path = create_fname(bbs, key);
            std::cout << std::filesystem::remove(path) << std::endl;
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
            break;
        }
    }

    return 0;
}

// ...
