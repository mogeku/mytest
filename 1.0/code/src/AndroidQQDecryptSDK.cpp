// AndroidQQDecryptSDK.cpp : Defines the exported functions for the DLL.
//
#ifdef WIN32 
#include "version.hpp"
#include "AndroidQQDecryptSDK.hpp"
#include <filesystem>
namespace fs = std::experimental::filesystem;
#define FOLDER_SPLITOR "\\"
#else
#include "../inc/version.hpp"
#include "../interface/AndroidQQDecryptSDK.hpp"
#include "common/filesystem.hpp"
namespace fs = ghc::filesystem;
#define FOLDER_SPLITOR "/"
#endif
#include "common/common.hpp"
#include "common/md5.h"
#include "common/Sqlite3Wrapper.hpp"
#include "common/AndroidQQDecrypt.hpp"

bool g_need_stop = false;

int InitAndroidQQDecryptSDK()
{
	int ret = 0;
	printf("AndroidQQDecryptSDK version: %s\n", LIB_ANDROID_QQ_DECRYPT_SDK_VERSION);
	g_need_stop = false;
	return ret;
}


AndroidQQDecrypt* CreateAndroidQQDecrypt(PhoneType phone_type, const char* backup_folder)
{
	switch (phone_type)
	{
	case HUAWEI:
		return new HuaweiQQDecrypt(backup_folder);
	case XIAOMI:
		return new XiaomiQQDecrypt(backup_folder);
	default:
		return nullptr;
	}
}

int DecryptAndroidQQDB(PhoneType phone_type, const char* backup_folder, const char* decrypted_folder)
{
	int ret = ERROR_NO;
	if (NULL == backup_folder || NULL == decrypted_folder)
		return ERROR_INVALID_PARAM;
	if (!fs::exists(backup_folder) || !fs::exists(decrypted_folder))
		return ERROR_PATH_NOT_EXIST;

	AndroidQQDecrypt* decryptor = CreateAndroidQQDecrypt(phone_type, backup_folder);
	if (nullptr == decryptor)
		return ERROR_UNKNOW_PHONE_TYPE;

	ret = decryptor->DecryptAndroidQQDB(decrypted_folder);
	delete decryptor;

	return ret;
}


void StopAndroidQQDBDecrypt()
{
	g_need_stop = true;
}