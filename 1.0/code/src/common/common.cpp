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
#include <cassert>
#include "common.hpp"
#include "Sqlite3Wrapper.hpp"

bool CopyDatabase(const std::string& src, const std::string& dst_folder)
{
	bool ret = true;
	fs::is_directory(dst_folder);
	if (!fs::exists(src) || !fs::is_directory(dst_folder))
		return false;

	fs::path p_src(src);
	if (".db" != p_src.extension())
		return false;
	fs::path p_dst_file(dst_folder + FOLDER_SPLITOR + p_src.filename().u8string());
	if (fs::exists(p_dst_file))
		fs::remove(p_dst_file);

	try
	{
		ret = fs::copy_file(src, p_dst_file);
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
	assert(str.empty());
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

bool AnalyseCreateSql(const std::string& sql, std::vector<FieldInfo>& fields)
{
	bool ret = true;
	if (sql.empty())
		return false;

	size_t start = sql.find_first_of("(");
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
		fields.push_back(field);
	}

	return ret;
}

bool GetTableFieldInfo(SQLite3Wrapper& db, const std::string& table_name, std::vector<FieldInfo>& fields)
{
	bool ret = true;
	std::string create_sql = GetCreateTableSql(db, table_name);
	
	return ret;
}