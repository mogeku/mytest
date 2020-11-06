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
#include "common/md5.h"
#include "common/Sqlite3Wrapper.hpp"
#include <algorithm>
bool g_need_stop = false;

int InitAndroidQQDecryptSDK()
{
	int ret = 0;
	printf("AndroidQQDecryptSDK version: %s\n", LIB_ANDROID_QQ_DECRYPT_SDK_VERSION);
	g_need_stop = false;
	return ret;
}

class AndroidQQDecrypt
{
public:
	AndroidQQDecrypt() {}
	virtual ~AndroidQQDecrypt() = 0;

	int DecryptAndroidQQDB(const char* decrypted_folder);

protected:
	std::string key_path;
	std::string db_folder;
};

AndroidQQDecrypt::~AndroidQQDecrypt() {}

enum FieldType
{
	FIELD_TYPE_UNKNOW,
	FIELD_TYPE_TEXT = 1,
	FIELD_TYPE_BLOB,
	FIELD_TYPE_INTEGER,
	FIELD_TYPE_DOUBLE,
};

struct FieldInfo
{
	int field_type;
	std::string field_name;
};

struct TableInfo
{
	std::vector<FieldInfo> fields;
	std::string table_name;
};

typedef std::vector<TableInfo> TableInfoVector;

int GetNeedDecryptTableInfoVector(SQLite3Wrapper& db, TableInfoVector& decrypt_table_list)
{
	int ret = 0;
	

	return ret;
}

void Decrypt(const std::vector<unsigned char>& src, std::vector<unsigned char>& dst)
{

}

void Decrypt(const std::string& src, std::string& dst)
{

}

int DecryptByTable(SQLite3Wrapper& db, std::string table_name)
{
	int ret = 0;

	SQLite3Stmt query_stmt = db.GetDBStmt("SELECT * FROM " + table_name);
	while (SQLITE_ROW == query_stmt.Step())
	{
		for (size_t i = 0; i < query_stmt.GetColumnCount(); i++)
		{
			int field_type = query_stmt.GetColumnType(i);
			std::string field_name = query_stmt.GetColumnName(i);
			if (SQLITE_BLOB == field_type)
			{
				std::vector<unsigned char> value;
				Decrypt(query_stmt.GetColumnBlob(i), value);
				SQLite3Stmt update_stmt = db.GetDBStmt("UPDATE " + table_name + " SET " + field_name + " = ?");
				update_stmt.BindBlob(0, value, NULL);
				update_stmt.Step();
			}
			else if (SQLITE_TEXT == field_type)
			{
				std::string value;
				Decrypt(query_stmt.GetColumnText(i), value);
				SQLite3Stmt update_stmt = db.GetDBStmt("UPDATE " + table_name + " SET " + field_name + " = ?");
				update_stmt.BindText(0, value, NULL);
				update_stmt.Step();
			}
		}

	}
	TableInfo table_info;
	table_info.table_name = table_name;
	for (size_t i = 0; i < query_stmt.GetColumnCount(); i++)
	{
		table_info.fields.push_back({ query_stmt.GetColumnType(i), query_stmt.GetColumnName(i) });
	}

	

	return ret;
}

int AndroidQQDecrypt::DecryptAndroidQQDB(const char* decrypted_folder)
{
	int ret = ERROR_NO;
	if (!fs::exists(db_folder))
		return ERROR_PATH_NOT_EXIST;
	if (!fs::is_directory(db_folder))
		return ERROR_PATH_NOT_DIR;

	fs::directory_iterator file_list(db_folder);

	for (auto& file : file_list)
	{
		if (std::string::npos == file.path().stem().u8string().find_first_not_of("0123456789")
			&& ".db" == file.path().extension())
		{
			SQLite3Wrapper db(file.path().u8string());
			if (SQLITE_OK != db.OpenDB())
				continue;
			//TableInfoVector decrypt_table_list;
			//GetNeedDecryptTableInfoVector(db, decrypt_table_list);
			DecryptByTable(db, "TroopInfoV2");
		}
	}

	return ret;
}

class HuaweiQQDecrypt : public AndroidQQDecrypt
{
public:
	HuaweiQQDecrypt(const char* backup_folder)
	{
		key_path = std::string(backup_folder) + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "com.tencent.mobileqq" + FOLDER_SPLITOR + "files" + FOLDER_SPLITOR + "kc";
		db_folder = std::string(backup_folder) + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "com.tencent.mobileqq" + FOLDER_SPLITOR + "databases";
	}
};

class XiaomiQQDecrypt : public AndroidQQDecrypt
{
public:
	XiaomiQQDecrypt(const char* backup_folder)
	{
		key_path = std::string(backup_folder) + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "com.tencent.mobileqq" + FOLDER_SPLITOR + "files" + FOLDER_SPLITOR + "kc";
		db_folder = std::string(backup_folder) + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "data" + FOLDER_SPLITOR + "com.tencent.mobileqq" + FOLDER_SPLITOR + "databases";
	}
};

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