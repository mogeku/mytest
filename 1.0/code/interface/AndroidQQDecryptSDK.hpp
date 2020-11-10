// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the ANDROIDQQDECRYPTSDK_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// ANDROIDQQDECRYPTSDK_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef _ANDROIDQQDECRYPTSDK_H
#define _ANDROIDQQDECRYPTSDK_H

#ifdef _WIN32
#ifdef ANDROIDQQDECRYPTSDK_EXPORTS
#define ANDROIDQQDECRYPTSDK_API __declspec(dllexport)
#else
#define ANDROIDQQDECRYPTSDK_API __declspec(dllimport)
#endif
#else
#ifdef ANDROIDQQDECRYPTSDK_EXPORTS
#define ANDROIDQQDECRYPTSDK_API extern "C" __attribute__((visibility ("default")))
#else
#define ANDROIDQQDECRYPTSDK_API extern "C"
#endif
#endif

enum PhoneType
{
	HUAWEI = 0,
	XIAOMI,
};

enum QQDecryptErrorType
{
	ERROR_NO,
	ERROR_INVALID_PARAM = 2000,
	ERROR_PATH_NOT_EXIST,
	ERROR_UNKNOW_PHONE_TYPE,
	ERROR_PATH_NOT_DIR,
	ERROR_DATABASE_COPY_FAILED,
	ERROR_GET_TABLE_FIELD_FAILED,
	ERROR_SEARCH_KEY_FAILED,
};

ANDROIDQQDECRYPTSDK_API int InitAndroidQQDecryptSDK();
ANDROIDQQDECRYPTSDK_API int  DecryptAndroidQQDB(PhoneType phone_type, const char* backup_folder, const char* decrypted_folder);
ANDROIDQQDECRYPTSDK_API void StopAndroidQQDBDecrypt();

#endif 
