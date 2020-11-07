#ifdef WIN32 
#include <filesystem>
namespace fs = std::experimental::filesystem;
#define FOLDER_SPLITOR "\\"
#else
#include "common/filesystem.hpp"
namespace fs = ghc::filesystem;
#define FOLDER_SPLITOR "/"
#endif
#include <string>


bool CopyDatabase(const std::string& src, const std::string& dst)
{
	if (!fs::exists(src) || !fs::exists(dst))
	{
		return false;
	}

	return fs::copy_file(src, dst);
}