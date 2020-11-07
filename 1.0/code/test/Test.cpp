#include "CppUnitTest.h"
#include "../src/common/common.hpp"
#include "../src/common/Sqlite3Wrapper.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(CopyDatabaseTest)
	{
	public:
		TEST_METHOD(HandleNormalInput)
		{
			Assert::IsTrue(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52\com.tencent.mobileqq.db)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp)"));
		}
		TEST_METHOD(HandleErrorDestInput)
		{
			Assert::IsFalse(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52\com.tencent.mobileqq.db)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp\com.tencent.mobileqq.db)"));
		}
		TEST_METHOD(HandleErrorSrcInput)
		{
			Assert::IsFalse(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp)"));
		}
	};
	TEST_CLASS(GetTableFieldInfoTest)
	{
	public:
		//SQLite3Wrapper db;
		//TEST_METHOD_INITIALIZE(OpenDatabase)
		//{
		//	db = SQLite3Wrapper(R"(D:\DB\android\qq\2020-11-06_10-21-52\data\data\com.tencent.mobileqq\databases\1209816754.db)");
		//	Assert::AreEqual(SQLITE_OK, db.OpenDB());
		//}

		TEST_METHOD(GetCreateTableSqlTest)
		{
			SQLite3Wrapper db = SQLite3Wrapper(R"(D:\DB\android\qq\2020-11-06_10-21-52\data\data\com.tencent.mobileqq\databases\1209816754.db)");
			db.OpenDB();
			std::string sql = GetCreateTableSql(db, "TroopInfoV2");
			Assert::IsTrue(!sql.empty());
			db.CloseDB();
		}

		TEST_METHOD_CLEANUP(CloseDatabase)
		{
			//db.CloseDB();
		}
	};
}
