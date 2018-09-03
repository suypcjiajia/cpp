#ifndef USERDATACACHE_H
#define USERDATACACHE_H
#include <string>
#include <list>
#include <tr1/unordered_map>
#include"json/json.h"
#include "TMutex.hpp"
using namespace std;
/*
 *用户质料cache
 *
 *
 */

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
typedef std::tr1::unordered_map<long ,Json::Value> UserShopsHash;

class UserDataCache
{
public:

    //生成多线程
    void Start();
    static void * ThreadFun(void *);
    //线程运行逻辑
    void RealRun();


    static UserDataCache* GetInstance();

    //获取属于我的供应商
    //return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试供应商店"  } ] //lUserId开的店列表，没有则为[]
    Json::Value GetMyShops(long lUserId);


    //获取属于我的联盟商家
    Json::Value GetMyNearShops(long lUserId);

    //获取属于我的供应商 and 联盟商家
    //return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试供应商店"  } ] //lUserId开的店列表，没有则为[]
    Json::Value GetMyAllShops(long lUserId);

private:


    int GetMyShop(long lUserId, Json::Value & jShopIds);

    int GetMyNearShop(long lUserId, Json::Value & jShopIds);


    int GetMyALLShop(long lUserId, Json::Value & jShopIds);

    UserDataCache(){}
    ~UserDataCache(){}

private:

    TMutex mMutex;

    UserShopsHash mUserShopsHash;

    UserShopsHash mUserNearShopHash;

    UserShopsHash mUserALLShopsHash;

    static UserDataCache* mInstance;

};

#endif // USERDATACACHE_H
