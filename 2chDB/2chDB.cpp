// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。  
//  

#include "2chDB.h"  
#include <sstream> // Add this include to fix E0070 error

extern "C" {  
#include "readcgi/read.h"  
    extern int rawmode, isbusytime;  
    extern void atexitfunc();  
}  

void init();  
void testWrite(const char* bbs, const char* key, const char* source);  
const char* queryFromReadCGI(const char* bbs, const char* key);  

inline static constexpr std::string create_fname(const char* bbs, const char* key) {  
    // TODO: sanitize bbs and key
    return std::format("{}/dat/{}.dat", bbs, key);  
}  

#include <algorithm> // Add this include to use std::transform

// ...

int main()  
{  
    init();  
    testWrite("news4vip", "1234567890", "Hello, 2chDB.");  

    std::cout << "Hello CMake." << std::endl;  
    std::cout << queryFromReadCGI("news4vip", "1234567890") << std::endl;
   
    std::string line;
    while ( std::getline(std::cin, line) ) {  
        std::istringstream iss(line); // This line is now valid
        std::string command;

        iss >> command;
        std::ranges::views::transform(command, std::tolower); // Use std::transform to convert to lowercase

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
    }  

    return 0;  
}  

// ...

void init()  
{  
   rawmode = 1;  
   isbusytime = 1;  
   atexit(atexitfunc);  
}  

const char* queryFromReadCGI(const char* bbs, const char* key) {  
   const std::string fname = create_fname(bbs, key);  
   dat_read(fname.c_str(), 0, 0, 0);  

   return BigBuffer;  
}  

void testWrite(const char* bbs, const char* key, const char* source) {  
   const std::string fname = create_fname(bbs, key);  

   std::ofstream ofs(fname, std::ios_base::out | std::ios_base::binary);  
   if (!ofs) {  
       std::cerr << "Failed to open file: " << fname.data() << std::endl;  
       return;  
   }  

   ofs << source;  
}
