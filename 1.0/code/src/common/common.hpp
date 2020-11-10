#pragma once
#include "Sqlite3Wrapper.hpp"

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
	FieldType field_type;
	std::string field_name;

	bool operator==(const FieldInfo& another)
	{
		return field_type == another.field_type && field_name == another.field_name;
	}
};

typedef bool(*FieldFilter) (const FieldInfo&);

std::ofstream OpenOutPutFileUtfSafe(const std::string& filename, int mode);
std::ifstream OpenInPutFileUtfSafe(const std::string& filename, int mode);
bool IsChinese(unsigned char);
bool CopyDatabase(const std::string& src, const std::string& dst);
std::string GetCreateTableSql(SQLite3Wrapper& db, const std::string& table_name);
std::vector<std::string>& Split(const std::string& str, char delim, std::vector<std::string>& items, bool skip_empty);
FieldType GetFieldTypeFromStr(std::string str);
bool AnalyseCreateSql(const std::string& sql, std::vector<FieldInfo>& fields, FieldFilter filter);
bool GetTableFieldInfo(SQLite3Wrapper& db, const std::string& table_name, std::vector<FieldInfo>& fields, FieldFilter filter);
bool GetTableList(SQLite3Wrapper& db, std::vector<std::string>& table_list);
std::string PackFieldsToStr(const std::vector<FieldInfo>& fields, const char* delim = ",");
