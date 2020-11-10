#include "CppUnitTest.h"
#include "AndroidQQDecryptSDK.hpp"
#include "../src/common/common.hpp"
#include "../src/common/AndroidQQDecrypt.hpp"
#include "../src/common/Sqlite3Wrapper.hpp"

static const char* g_test_db_path = R"(D:\DB\android\qq\2020-11-06_10-21-52\data\data\com.tencent.mobileqq\databases\1285087344.db)";
static const char* g_test_table_name = "TroopInfoV2";
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(CopyDatabaseTest)
	{
	public:
		TEST_METHOD(HandleNormalInput)
		{
			Assert::IsTrue(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52\com.tencent.mobileqq.db)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp\com.tencent.mobileqq.db)"));
		}
		TEST_METHOD(HandleErrorDestInput)
		{
			Assert::IsFalse(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52\com.tencent.mobileqq.db)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp)"));
		}
		TEST_METHOD(HandleErrorSrcInput)
		{
			Assert::IsFalse(CopyDatabase(R"(D:\DB\android\qq\2020-11-06_10-21-52)", R"(D:\DB\android\qq\2020-11-06_10-21-52\temp\com.tencent.mobileqq.db)"));
		}
	};
	TEST_CLASS(GetTableFieldInfoTest)

	{
	public:
		SQLite3Wrapper* db;
		TEST_METHOD_INITIALIZE(OpenDatabase)
		{
			db = new SQLite3Wrapper(g_test_db_path);
			Assert::AreEqual(db->OpenDB(), SQLITE_OK);
		}

		TEST_METHOD(GetCreateTableSqlTest)
		{
			std::string sql = GetCreateTableSql(*db, g_test_table_name);
			Assert::IsFalse(sql.empty());
			Assert::AreEqual(sql.c_str(), "CREATE TABLE TroopInfoV2 (_id INTEGER PRIMARY KEY AUTOINCREMENT,Administrator TEXT,adminNameShow TEXT,allowMemberAtAll INTEGER,allowMemberKick INTEGER,allowMemberModifTroopName INTEGER,appealDeadline INTEGER,associatePubAccount INTEGER,cAlbumResult INTEGER,cGroupLevel INTEGER,cGroupOption INTEGER,cGroupRankSysFlag INTEGER,cGroupRankUserFlag INTEGER,cNewGroupRankUserFlag INTEGER,cmdUinFlagEx2 INTEGER,cmduinFlagEx3Grocery INTEGER,dailyNewMemberUins TEXT,dwAdditionalFlag INTEGER,dwAppPrivilegeFlag INTEGER,dwAuthGroupType INTEGER,dwCmdUinJoinTime INTEGER,dwCmdUinUinFlag INTEGER,dwGagTimeStamp INTEGER,dwGagTimeStamp_me INTEGER,dwGroupClassExt INTEGER,dwGroupFlag INTEGER,dwGroupFlagExt INTEGER,dwGroupFlagExt3 INTEGER,dwGroupInfoSeq INTEGER,dwGroupLevelSeq INTEGER,dwLastBAFTipMsgUniSeq INTEGER,dwLastBAFTipMsgUniSeq2 INTEGER,dwLastInsertBAFTipTime INTEGER,dwOfficeMode INTEGER,dwTimeStamp INTEGER,eliminated INTEGER,exitTroopReason INTEGER,feeds_id TEXT,fingertroopmemo TEXT,gameSwitchStatus INTEGER,grade INTEGER,groupCardPrefixIntro TEXT,groupCardPrefixJson TEXT,groupFlagExt4 INTEGER,groupFreezeReason INTEGER,hasSetNewTroopHead INTEGER,hasSetNewTroopName INTEGER,hlGuildAppid INTEGER,hlGuildBinary INTEGER,hlGuildOrgid INTEGER,hlGuildSubType INTEGER,isAllowHistoryMsgFlag INTEGER,isNewTroop INTEGER,isShowInNearbyTroops INTEGER,isTroopBlocked INTEGER,joinTroopAnswer TEXT,joinTroopQuestion TEXT,lastMsgTime INTEGER,lastShareLbsMsgUniseq INTEGER default 0,location TEXT,mAtOrReplyMeUins TEXT,mCanSearchByKeywords INTEGER,mCanSearchByTroopUin INTEGER,mComparePartInt INTEGER,mCompareSpell TEXT,mGroupClassExtText TEXT,mHeaderUinsNew TEXT,mHeaderUinsOld TEXT,mIsFreezed INTEGER,mMemberCardSeq INTEGER,mMemberInvitingFlag INTEGER,mMemberNumSeq INTEGER,mOldMemberCardSeq INTEGER,mOldMemberNumSeq INTEGER,mQZonePhotoNum INTEGER,mRichFingerMemo TEXT,mSomeMemberUins TEXT,mTags TEXT,mTribeStatus INTEGER,mTroopAvatarId INTEGER,mTroopFileVideoIsWhite INTEGER,mTroopFileVideoReqInterval INTEGER,mTroopNeedPayNumber REAL,mTroopPicListJson TEXT,maxAdminNum INTEGER,maxInviteMemNum INTEGER,memberListToShow TEXT,myHonorList TEXT,nMsgLimitFreq INTEGER,nTroopGrade INTEGER,newTroopLevelMap TEXT,newTroopName TEXT,newTroopNameTimeStamp INTEGER,oldTroopName TEXT,ownerNameShow TEXT,school TEXT,showGameSwitchStatus INTEGER,strLastAnnouncement TEXT,strLocation TEXT,strQZonePhotoUrls TEXT,timeSec INTEGER,topicId TEXT,troopAuthenticateInfo TEXT,troopCreateTime INTEGER,troopCreditLevel INTEGER,troopCreditLevelInfo INTEGER,troopHonorGrayFlag INTEGER,troopInfoExtByte BLOB,troopLat INTEGER,troopLevelMap TEXT,troopLon INTEGER,troopPrivilegeFlag INTEGER,troopRepeatType INTEGER,troopTypeExt INTEGER,troopcode TEXT,troopface INTEGER,troopmask INTEGER,troopmemo TEXT,troopname TEXT,troopowneruin TEXT,trooptype INTEGER,troopuin TEXT UNIQUE,udwCmdUinRingtoneID INTEGER,uin TEXT,wClickBAFTipCount INTEGER,wInsertBAFTipCount INTEGER,wMemberMax INTEGER,wMemberNum INTEGER,wMemberNumClient INTEGER,wSpecialClass INTEGER)");
		}

		TEST_METHOD(HandleNoFilterInput)
		{
			std::vector<FieldInfo> fields;
			FieldInfo first_field { FIELD_TYPE_INTEGER, "_id" };
			FieldInfo last_field { FIELD_TYPE_INTEGER, "wSpecialClass" };
			Assert::IsTrue(GetTableFieldInfo(*db, g_test_table_name, fields, nullptr));
			Assert::IsFalse(fields.empty());
			Assert::IsTrue(fields[0] == first_field);
			Assert::IsTrue(fields[fields.size() - 1] == last_field);
		}

		TEST_METHOD(HandleFilterInput)
		{
			std::vector<FieldInfo> fields;
			auto filter = [](const FieldInfo& field) { return FIELD_TYPE_BLOB == field.field_type || FIELD_TYPE_TEXT == field.field_type; };
			Assert::IsTrue(GetTableFieldInfo(*db, g_test_table_name, fields, filter));
			if (!fields.empty())
			{
				for (auto field : fields)
					Assert::IsTrue(FIELD_TYPE_BLOB == field.field_type || FIELD_TYPE_TEXT == field.field_type);
			}
		}

		TEST_METHOD_CLEANUP(CloseDatabase)
		{
			db->CloseDB();
			delete db;
		}
	};

	TEST_CLASS(GetTableListTest)
	{
	public:
		TEST_METHOD(HandleNormalInput)
		{
			SQLite3Wrapper db(g_test_db_path);
			std::vector<std::string> table_list;
			Assert::AreEqual(SQLITE_OK, db.OpenDB());
			Assert::IsTrue(GetTableList(db, table_list));
			Assert::AreEqual(table_list[0].c_str(), "android_metadata");
			Assert::AreEqual(table_list[table_list.size() - 1].c_str(), "OfflineWebRes");
		}
	};

	TEST_CLASS(PackFieldsToStrTest)
	{
	public:
		TEST_METHOD(HandleNormalInput)
		{
			SQLite3Wrapper db(g_test_db_path);
			std::vector<FieldInfo> fields;
			Assert::AreEqual(db.OpenDB(), SQLITE_OK);
			Assert::IsTrue(GetTableFieldInfo(db, g_test_table_name, fields, nullptr));
			Assert::AreEqual(PackFieldsToStr(fields).substr(0, 200).c_str(), "_id,Administrator,adminNameShow,allowMemberAtAll,allowMemberKick,allowMemberModifTroopName,appealDeadline,associatePubAccount,cAlbumResult,cGroupLevel,cGroupOption,cGroupRankSysFlag,cGroupRankUserFlag");
		}

		TEST_METHOD(TestDelimParameter)
		{
			SQLite3Wrapper db(g_test_db_path);
			std::vector<FieldInfo> fields;
			Assert::AreEqual(db.OpenDB(), SQLITE_OK);
			Assert::IsTrue(GetTableFieldInfo(db, g_test_table_name, fields, nullptr));
			Assert::AreEqual(PackFieldsToStr(fields, "=?,").substr(0, 185).c_str(), "_id=?,Administrator=?,adminNameShow=?,allowMemberAtAll=?,allowMemberKick=?,allowMemberModifTroopName=?,appealDeadline=?,associatePubAccount=?,cAlbumResult=?,cGroupLevel=?,cGroupOption=?");
		}
	};

	TEST_CLASS(AndroidQQDecryptTest)
	{
	public:
		TEST_METHOD(SeachKeyTest)
		{
			HuaweiQQDecrypt decryptor(R"(D:\DB\android\qq\2020-11-06_10-21-52)");
			Assert::AreEqual(decryptor.SearchKey(), 0);
			Assert::AreEqual(decryptor.key.c_str(), "860986046736913");
		}

		TEST_METHOD(DecryptQQDBTest)
		{
			SQLite3Wrapper db(R"(D:\DB\android\qq\2020-11-06_10-21-52\1285087344_decrypted.db)");
			std::string value = "\xe6\x88\x91\xe9\x80\x9a\xe8\xbf\x87\xe4\xba\x86\xe4\xbd\xa0\xe7\x9a\x84\xe5\xa5\xbd\xe5\x8f\x8b\xe8\xaf\xb7\xe6\xb1\x82\xef\xbc\x8c\xe7\x8e\xb0\xe5\x9c\xa8\xe6\x88\x91\xe4\xbb\xac\xe5\x8f\xaf\xe4\xbb\xa5\xe5\xbc\x80\xe5\xa7\x8b\xe8\x81\x8a\xe5\xa4\xa9\xe4\xba\x86\xe3\x80\x82";
			Assert::AreEqual(0, DecryptAndroidQQDB(HUAWEI, R"(D:\DB\android\qq\2020-11-06_10-21-52)", R"(D:\DB\android\qq\2020-11-06_10-21-52)"));
			Assert::AreEqual(SQLITE_OK, db.OpenDB());
			SQLite3Stmt st = db.GetDBStmt("SELECT msgData FROM mr_friend_DD943929B6B25AEAAFF7C5393E7D07EC_New WHERE _id = 1");
			Assert::AreEqual(SQLITE_ROW, st.Step());
			Assert::AreEqual(value.c_str(), st.GetColumnText(0).c_str());
			st.Finalize();
			db.CloseDB();
		}
	};
}
