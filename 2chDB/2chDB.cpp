// 2chDB.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "2chDB.h"
std::string create_fname(const char* bbs, const char* key);
extern int rawmode;

void init();
void testWrite(const char* bbs, const char* key, const char* source);
const char* queryFromReadCGI(const char* bbs, const char* key);	

std::string create_fname(const char* bbs, const char* key) {
	return std::format("{}/dat/{}.dat", bbs, key);
}

using namespace std;

int main()
{
	init();
	testWrite("news4vip", "1234567890", "Hello, 2chDB.");

	cout << "Hello CMake." << endl;
	cout << queryFromReadCGI("news4vip", "1234567890") << endl;
	return 0;
}

void init()
{
	rawmode = 1;
}

const char* queryFromReadCGI(const char* bbs, const char* key) {
	const std::string fname = create_fname(bbs, key);
	dat_read(fname.c_str(), 0, 0, 0);

	return BigBuffer;
}

void testWrite(const char* bbs, const char* key, const char* source) {
	const std::string fname = create_fname(bbs, key);
	
	std::ofstream ofs(fname);
	if (!ofs) {
		std::cerr << "Failed to open file: " << fname.data() << std::endl;
		return;
	}

	ofs << source;
}
