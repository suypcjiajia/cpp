//#ifndef PERSONALAGENT_BLL_H
//#define PERSONALAGENT_BLL_H
//#include "basebll.h"
//#include <string>
//#include <vector>
//#include "TMutex.hpp"
//#include <tr1/unordered_map>
//#include "define.h"
//#include"json/json.h"
//using namespace std;

//class PersonalAgentBLL : public BaseBLL
//{
//public:
//    static PersonalAgentBLL* GetInstance();

//public:
//    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
//    //初始化
//    int Init();
//    //增加
//    int Insert( const  Json::Value  & pJson ,Json::Value & pResponse);
//    //删除
//    int Delete( const  Json::Value  & pJson  ,Json::Value & pResponse);
//    //修改
//    int Update( const  Json::Value  & pJson  ,Json::Value & pResponse);

//    int GetEntity(const  Json::Value  & pJson,
//                  Json::Value & pResponse);

//    //是否开通个代并付款了
//    int IsPersonAgent(const  Json::Value  & pJson  ,Json::Value & pResponse);

//    //查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//    int GetData(const  Json::Value  & pJson,
//                              Json::Value & pResponse);

//    void SetDefault(PersonalAgentEntity &  pEntity);

//private:
//    static PersonalAgentBLL* mInstance;
//    TMutex mMutex;//线程锁
//private:


//};

//#endif // PERSONALAGENT_BLL_H
