#include "UserDataCache.h"
#include "log/Logger.h"
#include "TConvert.h"
#include "bll/TBLL.h"
#include <stdlib.h>
#include "com/PostsAes.h"
#include "com/Request.h"
/*
 *用户质料cache
 *
 *
 */


UserDataCache* UserDataCache::mInstance = NULL;
UserDataCache* UserDataCache::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new UserDataCache;
    }
    return mInstance;
}


void UserDataCache::Start()
{
    pthread_t tid;

    pthread_create(&tid,NULL,ThreadFun,this);
}


void * UserDataCache::ThreadFun(void *p)
{
    UserDataCache * myself =  (UserDataCache*) p;
    myself->RealRun();
}
//线程运行逻辑
void UserDataCache::RealRun()
{


}

//获取属于我的供应商 and 联盟商家
//return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试供应商店"  } ] //lUserId开的店列表，没有则为[]
Json::Value UserDataCache::GetMyAllShops(long lUserId)
{
    Json::Value jRet;
    jRet.clear();
    UserShopsHash::iterator it =  mUserALLShopsHash.find(lUserId);
    if( it != mUserALLShopsHash.end()){
        jRet =  it->second;
    }else{
        int iRet = GetMyALLShop(lUserId,jRet);
        if( iRet == 0 ){
          mUserALLShopsHash[lUserId] = jRet;
        }else{
            jRet.resize(0);
        }
    }
    return jRet;
}

//获取属于我的供应商
//return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试供应商店"  } ] //lUserId开的店列表，没有则为[]
Json::Value UserDataCache::GetMyShops(long lUserId)
{
    Json::Value jRet;
    jRet.clear();
    UserShopsHash::iterator it =  mUserShopsHash.find(lUserId);
    if( it != mUserShopsHash.end()){
        jRet =  it->second;
    }else{
        int iRet = GetMyShop(lUserId,jRet);
        if( iRet == 0 ){
          mUserShopsHash[lUserId] = jRet;
        }else{
            jRet.resize(0);
        }
    }
    return jRet;
}

//获取属于我的联盟商家
//return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试店"  } ] //lUserId开的店列表，没有则为[]
Json::Value UserDataCache::GetMyNearShops(long lUserId)
{
    Json::Value jRet;
    jRet.clear();
    UserShopsHash::iterator it =  mUserNearShopHash.find(lUserId);
    if( it != mUserNearShopHash.end()){
        jRet =  it->second;
    }else{
        int iRet = GetMyNearShop(lUserId,jRet);
        if( iRet == 0 ){
          mUserNearShopHash[lUserId] = jRet;
        }else{
            jRet.resize(0);
        }
    }
    return jRet;
}


int UserDataCache::GetMyShop(long lUserId, Json::Value & jShopIds)
{
    TConvert tConvert;

    Json::Value jPost,jsonRet;
    jPost["lUserId"] = tConvert.LongToStr(lUserId);

    int iRet = Request::ShopGetMyShop(TBLL::GetInstance()->GetMepayIP(),jPost,jsonRet);
    if( iRet != 0){
        return -1;
    }

    Json::Value & jLst = jsonRet["list"];
    for( int i  = 0, index = 0; i < jLst.size() ;  i++ ){
        Json::Value & jShop = jLst[i];
        string s =  tConvert.Json2String( jShop["sCtiy"] );
        if( tConvert.Json2String( jShop["sCtiy"] ) != "美支付供应商")
            continue;
        jShopIds[index]["lPlatformId"] = jShop["sPrimaryKey"];
        jShopIds[index]["sShopName"] = jShop["sShopName"];
        index++;
    }
    if( jShopIds.size() == 0 ){
        jShopIds.resize(0);
    }

    return tConvert.Json2Int(jsonRet["iRet"]);
}



int UserDataCache::GetMyALLShop(long lUserId, Json::Value & jShopIds)
{
    TConvert tConvert;

    Json::Value jPost,jsonRet;
    jPost["lUserId"] = tConvert.LongToStr(lUserId);

    int iRet = Request::ShopGetMyShop(TBLL::GetInstance()->GetMepayIP(),jPost,jsonRet);
    if( iRet != 0){
        return -1;
    }

    Json::Value & jLst = jsonRet["list"];
    for( int i  = 0, index = 0; i < jLst.size() ;  i++ ){
        Json::Value & jShop = jLst[i];

        jShopIds[index]["lPlatformId"] = jShop["sPrimaryKey"];
        jShopIds[index]["sShopName"] = jShop["sShopName"];
        index++;
    }
    if( jShopIds.size() == 0 ){
        jShopIds.resize(0);
    }

    return tConvert.Json2Int(jsonRet["iRet"]);
}


//return 0 -sucessful
//return @jShopIds :  [ { "lPlatformId" : "7600226", "sShopName" : "测试供应商店"  } ] //lUserId开的店列表，没有则为[]
int UserDataCache::GetMyNearShop(long lUserId, Json::Value & jShopIds)
{
    TConvert tConvert;

    Json::Value jPost,jsonRet;
    jPost["lUserId"] = tConvert.LongToStr(lUserId);

    int iRet = Request::ShopGetMyShop(TBLL::GetInstance()->GetMepayIP(),jPost,jsonRet);
    if( iRet != 0){
        return -1;
    }

    Json::Value & jLst = jsonRet["list"];
    for( int i  = 0, index = 0; i < jLst.size() ;  i++ ){
        Json::Value & jShop = jLst[i];
        if( tConvert.Json2String( jShop["sCtiy"] ) == "美支付供应商")
            continue;
        jShopIds[index]["lPlatformId"] = jShop["sPrimaryKey"];
        jShopIds[index]["sShopName"] = jShop["sShopName"];
        index++;
    }
    if( jShopIds.size() == 0 ){
        jShopIds.resize(0);
    }

    return tConvert.Json2Int(jsonRet["iRet"]);
}


