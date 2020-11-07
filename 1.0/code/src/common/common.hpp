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
	int field_type;
	std::string field_name;
};

bool CopyDatabase(const std::string& src, const std::string& dst);
std::string GetCreateTableSql(SQLite3Wrapper& db, const std::string& table_name);
std::vector<std::string>& Split(const std::string& str, char delim, std::vector<std::string>& items, bool skip_empty);
FieldType GetFieldTypeFromStr(std::string str);
bool AnalyseCreateSql(const std::string& sql, std::vector<FieldInfo>& fields);
