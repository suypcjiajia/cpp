//#ifndef WALLETDAYBOOK_BLL_H
//#define WALLETDAYBOOK_BLL_H
//#include "basebll.h"
//#include <string>
//#include "TMutex.hpp"
//#include <tr1/unordered_map>
//#include "define.h"
//#include"json/json.h"
//using namespace std;
////主键-哈唏表
//typedef std::tr1::unordered_map<string,WalletDayBookEntity> WalletDayBookHashMap;

//class WalletDayBookBLL : public BaseBLL
//{
//public:  static WalletDayBookBLL* GetInstance();
//private: static WalletDayBookBLL* mInstance;
//private: TMutex mMutex;//线程锁
//public:
//    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
//    //初始化
//    int Init();

//    //增加
//    int Insert( const  Json::Value  & pJson, Json::Value & pResponse );
//    //删除
//    int Delete( const  Json::Value  & pJson, Json::Value & pResponse  );
//    //修改
//    int Update( const  Json::Value  & pJson, Json::Value & pResponse  );
//    //查询一行
//    int GetEntityByNum( const  Json::Value  & pJson, Json::Value & pResponse );
//    //查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//    int GetData(const  Json::Value  & pJson, Json::Value & pResponse );
//};

//#endif // WALLETDAYBOOK_BLL_H
