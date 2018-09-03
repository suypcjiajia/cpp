//#ifndef SERVER_BLL_H
//#define SERVER_BLL_H
//#include "basebll.h"
//#include <string>
//#include <vector>
//#include "TMutex.hpp"
//#include <tr1/unordered_map>
//#include "define.h"
//#include"json/json.h"
//using namespace std;
///*
// *
// */
//class ServerBLL : public BaseBLL
//{
//public:  static ServerBLL* GetInstance();
//private: static ServerBLL* mInstance;
//private: TMutex mMutex;//线程锁
//public:
//    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
//    //初始化
//    int Init();
//    //更换手机号
//    int UpdatePhone( const  Json::Value  & pJson ,Json::Value & pResponse);
//private:
//    //更换用户服务的手机号
//    int UpdateAccount(const string& psAccount, const string& psPassword, const string& psNewAccount, const string& psCode);
//};

//#endif // SERVER_BLL_H
