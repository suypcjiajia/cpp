#ifndef ZONEAGET_BLL_H
#define ZONEAGET_BLL_H
#include "basebll.h"
#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "define.h"
#include"json/json.h"
using namespace std;

class ZoneAgentBLL : public BaseBLL
{
public:  static ZoneAgentBLL* GetInstance();
private: static ZoneAgentBLL* mInstance;
private: TMutex mMutex;//线程锁
public:
    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
    //处理带token，不加密的情况
    int Process2(EvhtpSvr * pEvhtp,const  Json::Value  & pJson, Json::Value & pResponse);
    //初始化
    int Init();
    //增加
    int Insert( const  Json::Value  & pJson ,Json::Value & pResponse);
    //删除
    int Delete( const  Json::Value  & pJson  ,Json::Value & pResponse);
    //修改
    int Update( const  Json::Value  & pJson  ,Json::Value & pResponse);

    int GetEntity(const  Json::Value  & pJson,
                  Json::Value & pResponse);

    int IsOpen(const  Json::Value  & pJson,
               Json::Value & pResponse);

    //查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
    int GetData(const  Json::Value  & pJson,
                              Json::Value & pResponse);

    int GetZoneByCity(const  Json::Value  & pJson,
                      Json::Value & pResponse);



    int ModifyPwd(const  Json::Value  & pJson ,Json::Value & pResponse);
    //登陆
    int Login( const  Json::Value  & pJson ,Json::Value & pResponse);
    int ResetPwd(  const  Json::Value  & pJson ,Json::Value & pResponse);
    void SetDefault(ZoneAgentEntity & entity);



};

#endif // ZONEAGET_BLL_H
