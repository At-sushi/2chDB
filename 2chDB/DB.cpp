#ifdef __unix__
#include <unistd.h>
#endif
#include <mutex>

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
    #ifdef __unix__
    chroot(".");
    if (getuid() == 0)
        std::cerr << "WARNING: Running as a root may causes several security issues." << std::endl;
    #endif
}

static std::mutex mtx;

const char *queryFromReadCGI(const char *bbs, const char *key)
{
    const std::string fname = create_fname(bbs, key);
    std::lock_guard<std::mutex> lock(mtx);  // lock

    if (dat_read(fname.c_str(), 0, 0, 0) == 0)
        return "";

    return BigBuffer;
}

void testWrite(const char *bbs, const char *key, const char *source, bool isAppend /* = false*/)
{
    const std::filesystem::path fname = create_fname(bbs, key);

    // create a directory if it does not exist
    std::filesystem::create_directory(fname.parent_path());

	const std::ios_base::openmode mode = isAppend ? std::ios_base::app : std::ios_base::trunc;

    std::ofstream ofs(fname, mode | std::ios_base::out | std::ios_base::binary);
    if (!ofs)
    {
        std::cerr << "Failed to open file: " << fname << std::endl;
        return;
    }

    ofs << source;
    
    ofs.close();
    if (ofs.fail())
    {
        std::cerr << "Failed to write to file: " << fname << std::endl;
        return;
    }
}
