#ifndef POWER_BLL_H
#define POWER_BLL_H
#include "basebll.h"
#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "define.h"
#include"json/json.h"
#include "TConvert.h"
#include "dal/UserPowerDAL.h"

#include "TypeDef.h"

using namespace std;

class PowerBLL : public BaseBLL
{
public:  static PowerBLL* GetInstance();
private: static PowerBLL* mInstance;
private: TMutex mMutex;//线程锁
public:
    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
    //处理带token，不加密的情况
    int Process2(EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse);
    //初始化
    int Init();

    //
    int GetUser2(long idPlatform,const  Json::Value  & pJson,
                              Json::Value & pResponse);

    int Add2(long idPlatform,const  Json::Value  & pJson,
                              Json::Value & pResponse);

    int Del2(long idPlatform,const  Json::Value  & pJson,
                              Json::Value & pResponse);

    //检验商家资料完整性
    int integrality(const  Json::Value  & pJson,
                    Json::Value & pResponse);





    //检验是否有权限
    int Check(const  Json::Value  & pJson,
            Json::Value & pResponse);


private:


    TConvert tConvert;
    UserPowerDAL mUserPowerDAL;

};

#endif // POWER_BLL_H
