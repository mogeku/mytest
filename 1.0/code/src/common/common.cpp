#ifdef WIN32 
#include <filesystem>
namespace fs = std::experimental::filesystem;
#define FOLDER_SPLITOR "\\"
#else
#include "common/filesystem.hpp"
namespace fs = ghc::filesystem;
#define FOLDER_SPLITOR "/"
#endif


int CopyDatabase(const char* src, const char* dst)
{
	if (!fs::exists(src) || !fs::exists(dst))
	{
		return -1;
	}

	return fs::copy_file(src, dst);
}