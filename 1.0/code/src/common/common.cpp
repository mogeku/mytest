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
#include <sstream>
#include <fstream>
#include <cassert>
#include "common.hpp"
#include "StringConvertion.hpp"
#include "Sqlite3Wrapper.hpp"

std::ofstream OpenOutPutFileUtfSafe(const std::string& filename, int mode)
{
#ifdef WIN32
	std::wstring w_filename = Utf8ToUtf16(filename);
	return std::ofstream(w_filename, mode);
#else
	return std::ofstream(filename, mode);
#endif
}

std::ifstream OpenInPutFileUtfSafe(const std::string& filename, int mode)
{
#ifdef WIN32
	std::wstring w_filename = Utf8ToUtf16(filename);
	return std::ifstream(w_filename, mode);
#else
	return std::ifstream(filename, mode);
#endif
}

bool IsChinese(unsigned char ch)
{
	return 0 != (ch & 0x80);
}

bool CopyDatabase(const std::string& src, const std::string& dst)
{
	bool ret = true;
	if (!fs::exists(src) || fs::is_directory(dst))
		return false;

	if (fs::exists(dst))
		fs::remove(dst);

	try
	{
		ret = fs::copy_file(src, dst);
	}
	catch (fs::filesystem_error& e)
	{
		printf("[%s], at %d line, copy file failed, error msg: %s\n", __FILE__, __LINE__, e.what());
		ret = false;
	}
	return ret;
}

std::string GetCreateTableSql(SQLite3Wrapper& db, const std::string& table_name)
{
	std::string ret;
	if (!db.IsOpen())
		return ret;
	SQLite3Stmt create_sql_query_st = db.GetDBStmt("SELECT sql FROM sqlite_master WHERE type = \'table\' AND tbl_name = ?");
	create_sql_query_st.BindText(1, table_name, SQLITE_STATIC);
	if (SQLITE_ROW == create_sql_query_st.Step())
		ret = create_sql_query_st.GetColumnText(0);
	return ret;
}

std::vector<std::string>& Split(const std::string& str, char delim, std::vector<std::string>& items, bool skip_empty = true)
{
	if (str.empty())
		return items;
	std::istringstream iss(str);
	for (std::string item; std::getline(iss, item, delim);)
	{
		if (skip_empty && item.empty())
			continue;
		else
			items.push_back(item);
	}
	return items;
}

FieldType GetFieldTypeFromStr(std::string str)
{
	if ("TEXT" == str || "text" == str)
		return FIELD_TYPE_TEXT;
	else if ("BLOB" == str || "blob" == str)
		return FIELD_TYPE_BLOB;
	else if ("INTEGER" == str || "integer" == str)
		return FIELD_TYPE_INTEGER;
	else if ("DOUBLE" == str || "double" == str)
		return FIELD_TYPE_DOUBLE;
	else
		return FIELD_TYPE_UNKNOW;
}

bool AnalyseCreateSql(const std::string& sql, std::vector<FieldInfo>& fields, FieldFilter filter = nullptr)
{
	bool ret = true;
	if (sql.empty())
		return false;

	size_t start = sql.find_first_of("(") + 1;
	size_t end = sql.find_last_of(")");
	std::string field_info_sql(sql.cbegin() + start, sql.cbegin() + end);
	if (field_info_sql.empty())
		return false;

	std::vector<std::string> items;
	Split(field_info_sql, ',', items);
	for (std::string& item : items)
	{
		std::istringstream iss(item);
		std::string field_type_str;
		FieldInfo field;
		iss >> field.field_name;
		iss >> field_type_str;
		field.field_type = GetFieldTypeFromStr(field_type_str);
		if (FIELD_TYPE_UNKNOW == field.field_type)
			continue;
		if (nullptr != filter && !filter(field))
			continue;
		fields.push_back(field);
	}

	return ret;
}

bool GetTableFieldInfo(SQLite3Wrapper& db, const std::string& table_name, std::vector<FieldInfo>& fields, FieldFilter filter = nullptr)
{
	bool ret = true;
	if (!db.IsOpen())
		return false;
	std::string create_sql = GetCreateTableSql(db, table_name);
	if (!AnalyseCreateSql(create_sql, fields, filter))
		return false;
	
	return ret;
}

bool GetTableList(SQLite3Wrapper& db, std::vector<std::string>& table_list)
{
	bool ret = true;
	if (!db.IsOpen())
		return false;

	SQLite3Stmt stmt = db.GetDBStmt("SELECT DISTINCT tbl_name FROM sqlite_master WHERE type = ?");
	stmt.BindText(1, "table", SQLITE_TRANSIENT);
	while (SQLITE_ROW == stmt.Step())
	{
		table_list.push_back(stmt.GetColumnText(0));
	}
	stmt.Finalize();
	return ret;
}

std::string PackFieldsToStr(const std::vector<FieldInfo>& fields, const char* delim)

{
	std::string ret;
	if (fields.empty())
		return ret;

	for (auto field : fields)
	{
		ret += field.field_name + delim;
	}
	ret.assign(ret.cbegin(), ret.cend() - 1);
	return ret;
}