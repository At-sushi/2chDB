#include <unistd.h>

#include "2chDB.h"

extern "C"
{
#include "readcgi/read.h"
    extern int rawmode, isbusytime;
    extern void atexitfunc();
}

void init()
{
    rawmode = 1;
    isbusytime = 1;
    atexit(atexitfunc);

    // for the security
    chroot(".");
    if (getuid() == 0)
        std::cerr << "WARNING: Running as a root may causes several security issues." << std::endl;
}

const char *queryFromReadCGI(const char *bbs, const char *key)
{
    const std::string fname = create_fname(bbs, key);
    dat_read(fname.c_str(), 0, 0, 0);

    return BigBuffer;
}

void testWrite(const char *bbs, const char *key, const char *source)
{
    const std::filesystem::path fname = create_fname(bbs, key);

    // create a directory if it does not exist
    std::filesystem::create_directory(fname.parent_path());

    std::ofstream ofs(fname, std::ios_base::out | std::ios_base::binary);
    if (!ofs)
    {
        std::cerr << "Failed to open file: " << fname << std::endl;
        return;
    }

    ofs << source;
}
