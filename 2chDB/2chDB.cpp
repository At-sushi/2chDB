// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"

extern "C" {
#include "readcgi/read.h"
    extern int rawmode, isbusytime;
    extern void atexitfunc();
}

void init();
void testWrite(const char* bbs, const char* key, const char* source);
const char* queryFromReadCGI(const char* bbs, const char* key);	

inline static constexpr std::string create_fname(const char* bbs, const char* key) {
    return std::format("{}/dat/{}.dat", bbs, key);
}

int main()
{
    init();
    testWrite("news4vip", "1234567890", "Hello, 2chDB.");

    std::cout << "Hello CMake." << std::endl;
    std::cout << queryFromReadCGI("news4vip", "1234567890") << std::endl;
    for (;;) {
    }
    return 0;
}

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
