// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"
#include <sstream>   // Add this include to fix E0070 error
#include <filesystem>
#include <unordered_set>

// ...

// struct FileHeader
// {
//     std::uint16_t magic;
//     std::uint16_t version;
//     std::uint32_t size;
// };

struct BBSKey {
    std::string bbs;
    std::string key;

    auto operator <=> (const BBSKey&) const = default;
};

template <> struct std::hash<BBSKey>{
    using argument_type = BBSKey;
    using result_type = std::size_t;

    result_type operator()(const argument_type &arg) const noexcept
    {
        return std::hash<std::string>{}(arg.bbs) ^ std::hash<std::string>{}(arg.key);
    }
};

int main()
{
    init();

    std::unordered_set<BBSKey> delete_flag;
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
            if (delete_flag.contains({bbs, key}))
            {
                std::cout << "-ERR Not Found" << std::endl;
                continue;
            }
            
            std::cout << queryFromReadCGI(bbs.c_str(), key.c_str()) << std::endl;
        }
        else if (command == "set") {
            std::string bbs, key, source;

            iss >> bbs >> key;
            std::getline(iss, source);
            testWrite(bbs.c_str(), key.c_str(), source.data());

            delete_flag.erase({std::move(bbs), std::move(key)});
        }
        else if (command == "del")
        {
            std::string bbs, key;

            iss >> bbs >> key;

            delete_flag.insert({std::move(bbs), std::move(key)});
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
    }

    for (const auto &i : delete_flag)
    {
        const auto path = create_fname(i.bbs, i.key);
        std::filesystem::remove(path);
    }

    return 0;
}

// ...
