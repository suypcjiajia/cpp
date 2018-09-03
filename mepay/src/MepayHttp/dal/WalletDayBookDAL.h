//#ifndef WALLETDAYBOOK_DAL_H
//#define WALLETDAYBOOK_DAL_H

//#include <string>
//#include <vector>
//#include <deque>
//#include <hash_map>
//#include "TMutex.hpp"
//#include "dal/TMysqlDAL.h"
//#include "bll/WalletDayBookBLL.h"
//#include "dal/MysqlConnect.h"
//using namespace std;

//class WalletDayBookDAL
//{
//    // 定义单键
//public:  static WalletDayBookDAL* GetInstance();
//private: static WalletDayBookDAL* mInstance;
//private: TMutex mMutex;//线程锁
//public:
//    //创建表
//    int CreateTable();

//    //增加
//    int Insert( const WalletDayBookEntity& entity );
//    //删除
//    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
//    //修改
//    int Update( const string& psPrimaryKey,const WalletDayBookEntity& entity);

//    //增加,批量
//    int InsertBatch( const WalletDayBookList & pLst );

//    //获取单个（根据订单号)
//    int GetEntityByOrderNumStepMoneyType( const string & psOrderNum ,const int & piStep,const int & piMoneyType,WalletDayBookEntity & pOut );
//    int GetEntityBySerialNum(const string & psSerialNum,WalletDayBookEntity & pOut);
//    int GetListByOrderNumStep( const string & psOrderNum ,const int & piStep, WalletDayBookList & plstOut );
//    int GetList(const WalletDayBookQueryEntity & psQuery, int piIndex, int piCount,WalletDayBookList & plstOut, int & piTotalCount);

//private:
//    int GetQueryTotalCount(const string & psQuery, int & piCount);
//    //事务处理（只能是增册改的操作）
//    int Transaction( TMultiMysqlDAL * con,const vector<string>  & pQueryLst);

//};

//#endif // WALLETDAYBOOK_DAL_H
