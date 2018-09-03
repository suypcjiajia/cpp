#ifndef PRICEDISPATCH_H
#define PRICEDISPATCH_H
#include "TConvert.h"
#include "dal/UserSignDAL.h"
#include "dal/UserValueDAL.h"
#include "dal/UserDataDAL.h"
#include "dal/KValueDAL.h"
#include "dal/ConfigDAL.h"

#include <tr1/unordered_map>


class PriceDispatch
{
public:
    PriceDispatch();

    void Run();

private:
    //产生每天活跃用户价值
    int CaluSign();
    //计算用户在联盟商家交易额，产生交易用户价值
    int CaluTrade();
    //贬值处理
    int DeIncreemValue();

    //更新日K线数据
    int UpdateKValue();

    //更新日K线数据
    int RealUpdateKValue(LongLongMap & IdMap, enum_kvalue_type eType);

    string GetShopName(long Id);
    int GetShopEntity(long Id,Json::Value & jRes);


    double GetTradeValue( double dRecTotalMoney,double dTrade, int iType);

    //根据实体店流水，得到价值比例
    double GetNearPoint(double dRecTotalMoney);

    //获取商家收款信息,调用桌斌
    int GetMerRecInfo( long lPlatformId ,string sStartTime, string sEndTime, Json::Value & jResponse);

    double GetRandByPrice(double dPrice);
    void GetRandByPrice(double dPrice, double & dOpen, double & dClose, double & dHight, double & dLow);

    static void* TimerThread(void*ptr);

    //每天产生大于10个粉丝的行情
    static void* MakeKvalueThread(void*ptr);
    void RealMakevalue();
    void MakeKvalue(longs & ls, enum_kvalue_type eType);

    //把注册的用户同步到用户价值表
    static void* SyncUserThread(void*ptr);
    //把注册的用户同步到用户价值表
    void RealSyncUser();


    //处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
    static void* HotThread(void*ptr);
    //处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
    void RealHot();
    //处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
    static void* ConsumHotThread(void*ptr);
    //处理数据库用户表的hot字段（可交易状态 0-不能交易 1-可交易）
    void RealConsumHot();
    //让已绑定微信的粉丝变成可交易状态
    void LetBindWeixin2Hot();
    //完善粉丝资料变成可交易状态
    void LetPerfectInfo2Hot();
public:
    //从钱包服务批量获取消费手机号，把这批手机粉丝设置成可交易状态
    bool LetConsume2Hot();


private:
    UserSignDAL mUserSignDAL;
    UserValueDAL mUserValueDAL;
    UserDataDAL mUserDataDAL;
    KValueDAL mKValueDAL;
    ConfigDAL mConfigDAL;

    LongLongMap mChageIdMap;
    LongLongMap mPlatformMap;
    LongLongMap mPushMap;

    TConvert mtConvert;

};

#endif // PRICEDISPATCH_H
