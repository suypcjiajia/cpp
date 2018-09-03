
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户状态业务接口
 *Others:
*/



#include "FansBankBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "TBLL.h"
#include "MyAES.h"
#include "TMd5.h"
#include "com/PostsAes.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "cache/UserDataCache.h"
#include "com/Request.h"
#include "param/Param.h"

#include "dal/MysqlConnect.h"

#include "MyUtil.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if("GetBankInfo" == psMethod ){

        iRet =GetBankInfo(pJson,pResponse);
    }else if("GetMarketOnSalling" == psMethod ){

        iRet =GetMarketOnSalling(pJson,pResponse);
    }else if("GetMarketK" == psMethod ){

        iRet = GetMarketK(pJson,pResponse);
    }else if("GetPageMarket" == psMethod ){

        iRet = GetPageMarket(pJson,pResponse);
    }else if("Sell" == psMethod ){

        iRet = Sell(pJson,pResponse);
    }else if("GetMarketValue" == psMethod ){

        iRet = GetMarketValue(pJson,pResponse);
    }else if("SearchMarket" == psMethod ){

        iRet = SearchMarket(pJson,pResponse);
    }else if("GetSelling" == psMethod ){

        iRet = GetSelling(pJson,pResponse);
    }else if("SearchSelling" == psMethod ){

        iRet = SearchSelling(pJson,pResponse);
    }else if("Buy" == psMethod ){

        iRet = Buy(pJson,pResponse);
    }else if("GetPageOrder" == psMethod ){

        iRet = GetPageOrder(pJson,pResponse);
    }else if("CancelOrder" == psMethod ){

        iRet = CancelOrder(pJson,pResponse);
    }else if("Search" == psMethod ){

        iRet = Search(pJson,pResponse);
    }else if("GetTradeById" == psMethod ){

        iRet = GetTradeById(pJson,pResponse);
    }else if( "GetAllTrade" == psMethod){
        iRet = GetAllTrade(pJson,pResponse);
    }else if( "RedPoin" == psMethod){
        iRet = RedPoin(pJson,pResponse);
    }else if( "GetOrderByManage" == psMethod){
        iRet = GetOrderByManage(pJson,pResponse);
    }else if( "GetConfig" == psMethod){
        iRet = GetConfig(pJson,pResponse);
    }else if( "SetConfig" == psMethod){
        iRet = SetConfig(pJson,pResponse);
    }else if( "InnerBuy" == psMethod){
        iRet = InnerBuy(pJson,pResponse);
    }else if( "OrderState" == psMethod){
        iRet = OrderState(pJson,pResponse);
    }else if( "InnerCreate" == psMethod){
        iRet = InnerCreate(pJson,pResponse);
    }else if( "FansGive" == psMethod){
        iRet = FansGive(pJson,pResponse);
    }else if("GetDasaiBaodanOrder" == psMethod){
        iRet = GetDasaiBaodanOrder(pJson,pResponse);
    }
    else{
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}

/*
Description:初始化
Input: null
Output: null
Return: 0 -success
*/
int FansBankBLL::Init()
{

    return 0;
}




/*
Description:获取粉丝银行
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetBankInfo( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["lUserId"].isNull()  || pJson["iType"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    int iType = tConvert.Json2Int( pJson["iType"]);


    string sPhone = GetPhoneByUserId(lUserId);


    LongStringMap longMap;


    if( iType == enum_kvalue_type_push){

        longMap[lUserId] = "";

    }else {
        Json::Value jOpen = UserDataCache::GetInstance()->GetMyAllShops(lUserId);
        for(int j= 0; j < jOpen.size() ; j++){

            longMap[ tConvert.Json2Long( jOpen[j]["lPlatformId"])] = "";

        }
    }


    int iFansNum = 0;//粉丝数量
    double dFansAsset = 0.0;//粉丝总资产的现金
    double dFansCAsset = 0.0;
    double dPrice = 0.0;//均价中的现金
    double dCPrice = 0.0;
    double dTotalSell = 0.0;//累计卖出粉丝收入中的现金
    double dCTotalSell = 0.0;
    double dTotalBuy = 0.0;//累计买入粉丝支出中的现金
    double dCTotalBuy = 0.0;

    double dYestorDayFansAsset = 0.0;
    double dYestorDayPrice = 0.0;

    int mapSize = longMap.size();

    for(LongStringMap::iterator it = longMap.begin(); it != longMap.end(); it++ ){
        long Id = it->first;
        int iCount = 0;


        mUserDataDAL.GetWeixinBindNum(Id,iType,iCount);


        iFansNum += iCount;


        KValueEntity kvalue,ykvalue;
        MakeK(Id,iType ,tConvert.GetCurrentDay());
        MakeK(Id,iType ,tConvert.GetYesterDay());
        mKValueDAL.GetEntity(Id,iType ,tConvert.GetCurrentDay() ,kvalue);
        mKValueDAL.GetEntity(Id,iType ,tConvert.GetYesterDay() ,ykvalue);
        dFansAsset += iCount * kvalue.price;
        dYestorDayFansAsset += iCount*ykvalue.price;


        double dSell = 0.0,dBuy = 0.0;
        mFansBankOrderDAL.GetTotalSell(Id,iType, dSell);
        mFansBankOrderDAL.GetTotalBuy(Id,iType, dBuy);
        dTotalSell += dSell;
        dTotalBuy += dBuy;
    }


    if( iFansNum > 0){
        dPrice = dFansAsset/iFansNum;
        dYestorDayPrice = dYestorDayFansAsset/iFansNum;
    }


    dFansCAsset = dFansAsset;
    dCPrice = dPrice;
    dCTotalSell = dCTotalSell;
    dCTotalBuy = dCTotalBuy;


    Json::Value jRes;
    double dUserPushMoney = 0.0,dMerPushMoney = 0.0;
    if( 0 == GetPushIncome(sPhone,jRes)){
        dUserPushMoney = mtConvert.Json2Double( jRes["dUserPushMoney"]);
        dMerPushMoney = mtConvert.Json2Double( jRes["dMerPushMoney"]);
    }



    pResponse["dFansAsset"] = dFansAsset;
    pResponse["dFansCAsset"] = dFansCAsset;

    pResponse["iFansNum"] = iFansNum;

    pResponse["iPlatformNum"] = (int)longMap.size();


    pResponse["dPrice"] = dPrice;
    pResponse["dCPrice"] = dCPrice;
    pResponse["dUpPrice"] = dPrice - dYestorDayPrice ;


    pResponse["dTotalSell"] = dTotalSell;
    pResponse["dCTotalSell"] = dTotalSell;

    pResponse["dTotalBuy"] = dTotalBuy;
    pResponse["dCTotalBuy"] = dTotalBuy;

    //
    pResponse["dPushIncome"] = dUserPushMoney;
    pResponse["dYiyeIncome"] = dMerPushMoney;
    pResponse["dAdvIncome"] = 0.00;


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}



/*
Description:获取行情中的我的
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetMarketOnSalling ( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["lUserId"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long iType = tConvert.Json2Int(pJson["iType"]);

    LongStringMap longMap;

    if( iType == enum_kvalue_type_platform ){
        Json::Value jOpen = UserDataCache::GetInstance()->GetMyAllShops(lUserId);
        for(int j= 0; j < jOpen.size() ; j++){

            longMap[ tConvert.Json2Long( jOpen[j]["lPlatformId"])] = "";

        }
    }else{
        longMap[lUserId] = "";
    }

    int iIndex = 0;
    for(LongStringMap::iterator it = longMap.begin(); it != longMap.end(); it++ ){
        long Id = it->first;

        KValueEntity kvalue;
        double dUplist = 0.0;
        double dUpPrice = 0.0;

        MakeK(Id,iType ,mtConvert.GetCurrentDay());
        mKValueDAL.GetEntity(Id,iType ,mtConvert.GetCurrentDay() ,kvalue);
        dUpPrice = kvalue.price - kvalue.yclose;
        if( kvalue.yclose != 0){
            dUplist = dUpPrice/kvalue.yclose*100;
        }

        int iLots = 0;
        mFansBankOrderDAL.GetLotsSell(Id, iType, iLots);


        int iCount = 0;
        if( iType == enum_kvalue_type_platform ){
            mUserDataDAL.GetFansByPlatformId(Id,iCount);
        }else{
            long lCount = 0;
            mUserDataDAL.GetMyPushCount(tConvert.LongToStr(Id),lCount);
            iCount = (int)lCount;
        }

        if( iCount == 0)
            continue;


        pResponse["list"][iIndex]["Id"] = tConvert.LongToStr( Id );
        pResponse["list"][iIndex]["sName"] = kvalue.sName;

        pResponse["list"][iIndex]["dPrice"] = kvalue.price;
        pResponse["list"][iIndex]["dCPrice"] = kvalue.price;
        pResponse["list"][iIndex]["dUpPrice"] = dUpPrice;
        pResponse["list"][iIndex]["dUplift"] = dUplist;
        pResponse["list"][iIndex]["iLots"] = iLots;

        iIndex++;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:获取行情中的K线图
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetMarketK ( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["lUserId"].isNull() ||   pJson["Id"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["Ks"].resize(0);
    TConvert tConvert;
    long Id = tConvert.Json2Long(pJson["Id"]);
    long iType = tConvert.Json2Int(pJson["iType"]);
    long lUserId = tConvert.Json2Int(pJson["lUserId"]);

    Make20K(Id,iType);


    KValueEntity kvalue;
    double dUplist = 0.0;
    double dUpPrice = 0.0;
    MakeK(Id,iType ,mtConvert.GetCurrentDay());
    mKValueDAL.GetEntity(Id,iType ,mtConvert.GetCurrentDay() ,kvalue);
    dUpPrice = kvalue.price - kvalue.yclose;
    if( kvalue.yclose != 0){
        dUplist =  dUpPrice/kvalue.yclose*100;
    }

    double dPrice = kvalue.price;



    int iCanSellLots = GetCanSellLots(Id,iType);
    int iSellingLots = 0;
    mFansBankOrderDAL.GetLotsSell(Id,iType,iSellingLots);


    bool bOwer = false;
    if( iType == enum_kvalue_type_platform){
        long lManId = GetShopManId(Id);
        if( lManId == lUserId){
            bOwer = true;
        }
    }else{
        if( Id == lUserId){
            bOwer = true;
        }
    }



    KValueLst Ks;
    mKValueDAL.GetKs(Id,iType,Ks);


    pResponse["dPrice"] = dPrice;
    pResponse["dCPrice"] = dPrice;
    pResponse["dUpPrice"] =dUpPrice;
    pResponse["dUplift"] = dUplist;

    pResponse["dOpen"] = kvalue.open;
    pResponse["dClose"] = kvalue.yclose;
    pResponse["dHight"] = kvalue.hight;
    pResponse["dLow"] = kvalue.low;

    if( kvalue.open > kvalue.hight ){
        pResponse["dHight"] = kvalue.open;
    }

    pResponse["dTurnOver"] = kvalue.turnOver;
    pResponse["dTurnRate"] = kvalue.turnRate;

    pResponse["iCanSellLots"] = iCanSellLots;
    pResponse["iSellingLots"] = iSellingLots;
    pResponse["bOwer"] = bOwer;

    for( int i = 0; i < Ks.size() ; i++){//K线图
        KValueEntity & entity = Ks[i];

        double dMa5 = 0.0;
        mKValueDAL.GetMaAvg(Id,iType,entity.sDay,5,dMa5);
        double dMa10 = 0.0;
        mKValueDAL.GetMaAvg(Id,iType,entity.sDay,10,dMa10);
        double dMa20 = 0.0;
        mKValueDAL.GetMaAvg(Id,iType,entity.sDay,20,dMa20);
        double dAmount = 0.0;
        mFansBankOrderDAL.GetOneDayTrade(Id,iType,entity.sDay,dAmount);

        pResponse["Ks"][i]["dOpen"] = entity.open;
        pResponse["Ks"][i]["dClose"] = entity.close;
        pResponse["Ks"][i]["dHight"] = entity.hight;
        pResponse["Ks"][i]["dLow"] = entity.low;

        if( entity.open > entity.hight ){
            pResponse["dHight"] = entity.open;
        }

        pResponse["Ks"][i]["ma5"] = dMa5;
        pResponse["Ks"][i]["ma10"] = dMa10;
        pResponse["Ks"][i]["ma20"] = dMa20;

        pResponse["Ks"][i]["amount"] = dAmount;
        pResponse["Ks"][i]["balance"] = 10.55;

        pResponse["Ks"][i]["sDay"] = entity.sDay;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}


/*
Description:获取详情中的成交记录
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetTradeById( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() ||   pJson["Id"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    long Id = tConvert.Json2Long(pJson["Id"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);

    FansBankOrderLst lst;
    mFansBankOrderDAL.GetPageById(iType,Id,iPage,iCount,lst);


    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];

        bool bBuy = true;
        if( entity.lIdSell == Id){
            bBuy = false;
        }

        pResponse["list"][i]["lEndTime"] = mtConvert.LongToStr( entity.lEndTime/1000000 );
        pResponse["list"][i]["iLots"] = entity.iLots;
        pResponse["list"][i]["dPrice"] = entity.dPrice;
        pResponse["list"][i]["dCPrice"] = entity.dPrice;
        pResponse["list"][i]["bBuy"] = bBuy;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}



/*
Description:获取所有卖出记录
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetAllTrade( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iType = tConvert.Json2Int(pJson["iType"]);
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);

    FansBankOrderLst lst;
    mFansBankOrderDAL.GetAllTrade(iType,iPage,iCount,lst);


    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];


        pResponse["list"][i]["lEndTime"] = mtConvert.LongToStr( entity.lEndTime/1000000 );
        pResponse["list"][i]["iLots"] = entity.iLots;
        pResponse["list"][i]["dPrice"] = entity.dPrice;
        pResponse["list"][i]["dCPrice"] = entity.dPrice;
        pResponse["list"][i]["lIdSell"] = mtConvert.LongToStr(entity.lIdSell);
        pResponse["list"][i]["sNameSell"] = entity.sNameSell;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:获取全部行情（按分页）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetPageMarket( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);
    int iType = tConvert.Json2Int(pJson["iType"]);

    KValueLst Ks;
    mKValueDAL.GetPage( iType, iPage,iCount,"", Ks);

    for( int i = 0 ; i < Ks.size() ; i++){



        KValueEntity & kvalue = Ks[i];


        double dUplist =0.0;
        double dUpPrice = kvalue.price - kvalue.yclose;
        if( kvalue.yclose != 0){
            dUplist =  dUpPrice/kvalue.yclose*100;
        }


        pResponse["list"][i]["dPrice"] = kvalue.price;
        pResponse["list"][i]["dCPrice"] = kvalue.price;
        pResponse["list"][i]["dUpPrice"] = dUpPrice;
        pResponse["list"][i]["dUplift"] = dUplist;

        pResponse["list"][i]["Id"] = tConvert.LongToStr( kvalue.Id );
        pResponse["list"][i]["sName"] = kvalue.sName;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:卖出操作
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::Sell( const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    TConvert tConvert;
    int iRet = -1;
    if( pJson["Id"].isNull()  || pJson["iType"].isNull() ||
            pJson["iLots"].isNull() || pJson["sPassWord"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    int iLots = tConvert.Json2Int(pJson["iLots"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    long Id = tConvert.Json2Int(pJson["Id"]);


    int iSelledLots = 0, iSellingLots = 0;
    mFansBankOrderDAL.GeTodaySelledLots(iType,tConvert.LongToStr(Id),iSelledLots);
    mFansBankOrderDAL.GetLotsSell(Id,iType,iSellingLots);
    if( iSelledLots + iSellingLots + iLots > 20){
        RetJsonMsg::SetRetMsg(-1,pResponse,"今天您交易手数达到封顶");
        return -1;
    }


    string sName ,sPayPhone;
    if( iType == enum_kvalue_type_push){
        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(Id,userEntity);
        sName = userEntity.sNickName;
        sPayPhone = userEntity.sAccount;

        if( userEntity.iState == enum_user_state_colse){
            RetJsonMsg::SetRetMsg(-1,pResponse,err1);
            return -1;
        }
    }else{

        Json::Value jShop;
        GetShopEntity(Id,jShop);
        sName = mtConvert.Json2String(  jShop["entity"]["sShopName"] );
        sPayPhone = mtConvert.Json2String(  jShop["entity"]["sShopPhone"] );
        int iFansTrad = mtConvert.Json2Int(  jShop["entity"]["iFansTrad"] );
        if( 2 == iFansTrad ){
            RetJsonMsg::SetRetMsg(-1,pResponse,"这个商家平台禁止交易");
            return -1;
        }

        UserDataEntity userEntity;
        mUserDataDAL.GetEntityByAccount(sPayPhone,userEntity);

        if( userEntity.iState == enum_user_state_colse){
            RetJsonMsg::SetRetMsg(-1,pResponse,err1);
            return -1;
        }
        if( userEntity.iState == enum_user_state_disablefans){
            RetJsonMsg::SetRetMsg(-1,pResponse,"这个用户禁止交易");
            return -1;
        }

    }

    //to check pay password from zhou bin
    if( CheckPayPassWord(sPayPhone, sPassWord,pResponse) != 0){
        return -1;
    }


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        RetJsonMsg::SetRetMsg(-1,pResponse);
        return -1;
    }
    //事务处理
    //start
    con->AutoCommit(false);
    int iCanLots = GetCanSellLots(Id,iType);//判断是否能挂卖
    if( iLots > iCanLots){
        con->AutoCommit(true);

        RetJsonMsg::SetRetMsg(-1,pResponse,"剩余手数不足");
        return -1;
    }



    if( mFansBankOrderDAL.IsExistSelling(Id,iType) == 0){
        iRet = mFansBankOrderDAL.InreemLots(Id,iType,iLots);
    }else{
        FansBankOrderEntity orderEntity;
        orderEntity.dPrice = 0.0;
        orderEntity.iLots = iLots;
        orderEntity.lIdSell = Id;
        orderEntity.iType = iType;
        orderEntity.lAddTime = mtConvert.GetCurrentTime();
        orderEntity.lEndTime = orderEntity.lAddTime;
        orderEntity.iState = enum_order_salling;
        orderEntity.sNameSell = sName;
        orderEntity.sOrderNo = MakeAnOrderNum(Id);
        iRet = mFansBankOrderDAL.Add(orderEntity);
    }

    con->AutoCommit(true);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/*
Description:获取市值
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetMarketValue( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["Id"].isNull()  || pJson["iType"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    long Id = tConvert.Json2Long(pJson["Id"]);
    int iType = tConvert.Json2Int( pJson["iType"]);

    double dPushMoney = 0.0;

    LongStringMap longMap;

    string sName,sHeadImage;

    if( iType == enum_kvalue_type_platform ){

        sName = GetShopName(Id);
        long ManId = GetShopManId(Id);

        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(ManId,userEntity);
        sHeadImage = userEntity.sHeadImage;

        Json::Value jRes;
        if( 0 == GetPlatformIdIncome(Id,jRes) ){
            dPushMoney = mtConvert.Json2Double(jRes["dMerPushMoney"]);
        }

    }else {

        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(Id,userEntity);
        sName = userEntity.sNickName;
        sHeadImage = userEntity.sHeadImage;

        Json::Value jRes;

        if( 0 == GetPushIncome(userEntity.sAccount,jRes)){
            dPushMoney = mtConvert.Json2Double( jRes["dUserPushMoney"]);
        }
    }


    int iFansNum = 0;
    double dFansAsset = 0.0;
    double dPrice = 0.0;
    double dTotalSell = 0.0,dTotalBuy = 0.0;


    //    if( iType == enum_kvalue_type_push){
    //        long iTmp = 0;
    //        mUserDataDAL.GetMyPushCount(tConvert.LongToStr(Id),iTmp);
    //        iFansNum = (int)iTmp;

    //    }else{
    //        mUserDataDAL.GetFansByPlatformId(Id,iFansNum);
    //    }

    mUserDataDAL.GetWeixinBindNum(Id,iType,iFansNum);



    MakeK(Id,iType ,tConvert.GetCurrentDay());

    KValueEntity kvalue;
    mKValueDAL.GetEntity(Id,iType ,tConvert.GetCurrentDay() ,kvalue);
    dFansAsset = iFansNum * kvalue.price;
    dPrice = kvalue.price;

    mFansBankOrderDAL.GetTotalSell(Id,iType, dTotalSell);
    mFansBankOrderDAL.GetTotalBuy(Id,iType, dTotalBuy);



    pResponse["Id"] = tConvert.LongToStr(Id);
    pResponse["sName"] = sName;
    pResponse["sHeadImage"] = sHeadImage;

    pResponse["dFansAsset"] = dFansAsset;
    pResponse["dFansCAsset"] = dFansAsset;

    pResponse["iFansNum"] = iFansNum;

    pResponse["iPlatformNum"] = (int)longMap.size();

    pResponse["dPrice"] = dPrice;
    pResponse["dCPrice"] = dPrice;
    pResponse["dUpPrice"] = kvalue.price - kvalue.yclose;


    pResponse["dTotalSell"] = dTotalSell;
    pResponse["dCTotalSell"] = dTotalSell;

    pResponse["dTotalBuy"] = dTotalBuy;
    pResponse["dCTotalBuy"] = dTotalBuy;

    pResponse["dPushIncome"] = dPushMoney;
    pResponse["dYiyeIncome"] = dPushMoney;
    pResponse["dAdvIncome"] = 0.0;


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:搜索行情
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::SearchMarket( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["sCondition"].isNull()  || pJson["iType"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    string sCondition = tConvert.Json2String( pJson["sCondition"]);
    int iType = tConvert.Json2Int( pJson["iType"]);

    KValueLst Ks;
    mKValueDAL.GetPage( iType, 1,100 ,sCondition, Ks);

    for( int i = 0 ; i < Ks.size() ; i++){

        KValueEntity & kvalue = Ks[i];

        double dUplist = 0.0;
        if( kvalue.price != 0){
            dUplist =  (kvalue.price - kvalue.yclose)/kvalue.yclose*100;
        }


        pResponse["list"][i]["dPrice"] = kvalue.price;
        pResponse["list"][i]["dCPrice"] = kvalue.price;
        pResponse["list"][i]["dUpPrice"] = kvalue.price - kvalue.price;
        pResponse["list"][i]["dUplift"] = dUplist;

        pResponse["list"][i]["Id"] = tConvert.LongToStr( kvalue.Id );
        pResponse["list"][i]["sName"] = kvalue.sName;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:搜索
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::Search( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["sCondition"].isNull()  || pJson["iType"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    string sCondition = tConvert.Json2String( pJson["sCondition"]);
    int iType = tConvert.Json2Int( pJson["iType"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);

    KValueLst Ks;
    mKValueDAL.GetPage( iType, iPage,iCount ,sCondition, Ks);

    for( int i = 0 ; i < Ks.size() ; i++){

        KValueEntity & kvalue = Ks[i];

        pResponse["list"][i]["Id"] = tConvert.LongToStr( kvalue.Id );
        pResponse["list"][i]["sName"] = kvalue.sName;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:获取交易中的可买入列表
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetSelling( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if(  pJson["lUserId"].isNull() || pJson["iType"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iType = tConvert.Json2Int( pJson["iType"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);





    string baodanMember;
    int isvip = 1;
    if( iType == enum_kvalue_type_push){

        long timestamp = tConvert.GetCurrentTime()/1000;

        Json::Value jFee,jFeeR;

        jFee["ttdid"] = tConvert.LongToStr( lUserId );
        string sunweiUrl = "http://" + TBLL::GetInstance()->GetSunWeiIP() + "/apittd/bdorder/bdorderapi.ashx?do=isvip";
        //        Request::SunWeiHttpPost(sunweiUrl,
        //                                "83f41be38e65411f",tConvert.LongToStr(timestamp),jFee,jFeeR);

        //        isvip  = tConvert.Json2Int( jFeeR["state"] );



        jFee.clear();jFeeR.clear();
        sunweiUrl = "http://" + TBLL::GetInstance()->GetSunWeiIP() + "/apittd/bdorder/bdorderapi.ashx?do=getdatabd";
        Request::SunWeiHttpPost(sunweiUrl,
                                "83f41be38e65411f",tConvert.LongToStr(timestamp),jFee,jFeeR);

        if( tConvert.Json2Int( jFeeR["state"] ) == 0){
            baodanMember = tConvert.JsonArray2String(jFeeR["list"]);
        }

        sunweiUrl = "http://" + TBLL::GetInstance()->GetKonhaiIP()  + "/Api/AuthorityService/getMatchIds?appKey=ca84d1343b96baa8137c943ed1860e522cacb238&timestamp=" + tConvert.LongToStr(timestamp);
        jFee.clear();jFeeR.clear();
        Request::WalletHttpPost2(sunweiUrl,"96baa8137c943ed1",tConvert.LongToStr(timestamp),jFee,jFeeR);

        if( tConvert.Json2String( jFeeR["state"] ) != "Error"){
            string tmp = tConvert.JsonArray2String(jFeeR["result"]);
            if( baodanMember.empty()){
                baodanMember = tmp;
            }else{

                baodanMember += "," + tmp;
            }
        }
        if (baodanMember.find(tConvert.LongToStr(lUserId)) != string::npos){
            isvip = 0;
        }

    }

    FansBankOrderLst lst;
    mFansBankOrderDAL.GetSellingByBaodanState(  iType, iPage, "",isvip,baodanMember, iCount, lst);

    for( int i = 0,j = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];

        MakeK(entity.lIdSell,iType ,tConvert.GetCurrentDay());

        KValueEntity kvalue;
        mKValueDAL.GetEntity(entity.lIdSell,iType ,tConvert.GetCurrentDay() ,kvalue);

        double dUplist = 0.0;
        if( kvalue.yclose != 0){
            dUplist =  (kvalue.price - kvalue.yclose)/kvalue.yclose*100;
        }
        double dPrice = kvalue.price;

        if( iType == enum_kvalue_type_platform){
            long lManId = GetShopManId( entity.lIdSell);
            if( lManId == lUserId){
                continue;
            }
        }else{
            if( entity.lIdSell == lUserId){
                continue;
            }
        }



        pResponse["list"][j]["iLots"] = entity.iLots;
        pResponse["list"][j]["dPrice"] = dPrice;
        pResponse["list"][j]["dCPrice"] = dPrice;
        pResponse["list"][j]["dUpPrice"] = kvalue.price - kvalue.yclose;
        pResponse["list"][j]["dUplift"] = dUplist;

        pResponse["list"][j]["Id"] = tConvert.LongToStr( entity.lIdSell );
        pResponse["list"][j]["sName"] = entity.sNameSell;

        j++;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:搜索交易中的可买入列表
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::SearchSelling( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["sCondition"].isNull() ||  pJson["iType"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iType = tConvert.Json2Int( pJson["iType"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    string sCondition = tConvert.Json2String( pJson["sCondition"]);

    FansBankOrderLst lst;
    mFansBankOrderDAL.GetSelling(  iType, iPage, sCondition, iCount, lst);
    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];

        MakeK(entity.lIdSell,iType ,tConvert.GetCurrentDay());


        KValueEntity kvalue;
        mKValueDAL.GetEntity(entity.lIdSell,iType ,tConvert.GetCurrentDay() ,kvalue);
        double dUplist = 0.0;
        if( kvalue.yclose != 0){
            dUplist =  (kvalue.price - kvalue.yclose)/kvalue.yclose*100;
        }
        double dPrice = kvalue.price;


        pResponse["list"][i]["iLots"] = entity.iLots;
        pResponse["list"][i]["dPrice"] = dPrice;
        pResponse["list"][i]["dCPrice"] = dPrice;
        pResponse["list"][i]["dUpPrice"] = kvalue.price - kvalue.yclose;
        pResponse["list"][i]["dUplift"] = dUplist;

        pResponse["list"][i]["Id"] = tConvert.LongToStr( entity.lIdSell );
        pResponse["list"][i]["sName"] = entity.sNameSell;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:买入操作
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::Buy( const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    if( pJson["sOrderNo"].isNull() ||  pJson["iType"].isNull() || pJson["lBuyId"].isNull() || pJson["lSellId"].isNull()
            || pJson["iLots"].isNull() || pJson["iPayType"].isNull() || pJson["sPassWord"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iRet = -1;
    int iType = tConvert.Json2Int( pJson["iType"]);
    int iPayType = tConvert.Json2Int( pJson["iPayType"]);
    int iLots = tConvert.Json2Int( pJson["iLots"]);
    long lBuyId = tConvert.Json2Long( pJson["lBuyId"]);
    long lSellId = tConvert.Json2Long( pJson["lSellId"]);
    string sOrderNo = tConvert.Json2String( pJson["sOrderNo"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);

    if( iPayType <20 || iPayType >22){
      RetJsonMsg::SetRetMsg(-1,pResponse,"支付方式枚举值有误");
      return -1;
    }


    double dPrice = 0.0;

    KValueEntity kforprice;
    iRet = mKValueDAL.GetEntity(lSellId,iType,mtConvert.GetCurrentDay(),kforprice);
    dPrice = kforprice.price;

    if( iRet != 0 || kforprice.price < 1.0){
        RetJsonMsg::SetRetMsg(-1,pResponse,"获取均价出错");
        return -1;
    }



    long lSellAddTime = 0;
    mFansBankOrderDAL.GetLotsSellAddTime(lSellId,iType,lSellAddTime);


    string sPayPhone,sMerPhone,sNameBuy, sNameSell;
    double dTotalMoney;string sRemark;
    if( iType == enum_kvalue_type_push){

        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(lBuyId,userEntity);
        sPayPhone = userEntity.sAccount;
        sNameBuy = userEntity.sNickName;
        mUserDataDAL.GetEntityById(lSellId,userEntity);
        sMerPhone = userEntity.sAccount;
        sNameSell = userEntity.sNickName;
        sRemark = "C端";

    }else{

        Json::Value jShop;
        GetShopEntity(lBuyId,jShop);
        sNameBuy = mtConvert.Json2String(  jShop["entity"]["sShopName"] );
        sPayPhone = mtConvert.Json2String(  jShop["entity"]["sShopPhone"] );
        int iFansTrad = mtConvert.Json2Int(  jShop["entity"]["iFansTrad"] );
        if( 2 == iFansTrad ){
            RetJsonMsg::SetRetMsg(-1,pResponse,"这个商家平台禁止交易");
            return -1;
        }

        jShop.clear();
        GetShopEntity(lSellId,jShop);
        sNameSell = mtConvert.Json2String(  jShop["entity"]["sShopName"] );
        sMerPhone = mtConvert.Json2String(  jShop["entity"]["sShopPhone"] );

        sRemark = "B端";

    }

    dTotalMoney = dPrice*iLots*Param::GetFansPerLots();


    string sSelledUser;//要被卖的粉丝，格式a,b,c



    vector<string>   queryLst;

    bool comeFromDaSaiBaodan   = ( IsDaSaiMember(lBuyId) || IsBadodanMember(lBuyId) )
                            && (IsDaSaiMember(lSellId) || IsBadodanMember(lSellId));

    FansBankOrderEntity orderEntity;
    orderEntity.dPrice = dPrice;
    orderEntity.dTurnOver = dTotalMoney;
    orderEntity.iLots = iLots;
    orderEntity.iState = enum_order_ok;
    orderEntity.iType = iType;
    orderEntity.lAddTime = lSellAddTime;
    orderEntity.lEndTime = tConvert.GetCurrentTime();
    orderEntity.lIdBuy = lBuyId;
    orderEntity.lIdSell = lSellId;
    orderEntity.sNameBuy = sNameBuy;
    orderEntity.sNameSell = sNameSell;
    orderEntity.sOrderNo = sOrderNo;
    comeFromDaSaiBaodan ? orderEntity.comeFrom = 1:orderEntity.comeFrom = 0;




    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        RetJsonMsg::SetRetMsg(-1,pResponse);
        return -1;
    }

    //事务处理
    //start
    con->AutoCommit(false);


    int iCanSellLots = 0;
    mFansBankOrderDAL.GetLotsSell(lSellId,iType,iCanSellLots);//FOR UPDATE加锁
    if( iCanSellLots < iLots){

        con->AutoCommit(true);

        RetJsonMsg::SetRetMsg(-201,pResponse,"剩余手数不足");
        return -201;

    }

    GetSellUserString4(sMerPhone,lSellId,iType,iLots*Param::GetFansPerLots(),sSelledUser);
    if( sSelledUser.empty()){
        con->AutoCommit(true);
        RetJsonMsg::SetRetMsg(-1,pResponse,"剩余手数不足");
        return -1;
    }
    queryLst.push_back(mFansBankOrderDAL.GetSqlAdd(orderEntity));//添加一条已完成订单
    queryLst.push_back(mFansBankOrderDAL.GetSqlDeInreemLots(lSellId,iType,iLots));//减少挂卖中的手数
    if( iType == enum_kvalue_type_platform){
        //转移关注关系
        queryLst.push_back(mUserDataDAL.GetSqlMoveAttention(lSellId,lBuyId,sSelledUser));
    }else{
        //转移推广关系
        queryLst.push_back(mUserDataDAL.GetSqlMovePushMan(lSellId,lBuyId,sSelledUser));
    }
    queryLst.push_back(mFansBankOrderDAL.GetSqlInreemLotsOver(lSellId,iType,iLots,dPrice*iLots*Param::GetFansPerLots()));
    queryLst.push_back(mFansBankOrderDAL.GetSqlDel(lSellId,iType));
    queryLst.push_back(mFansBankFansmoveDAL.GetSqlAdd(sOrderNo,sSelledUser));

    int iIndex = 0;
    iRet = -1;
    while( iIndex < queryLst.size()){
        iRet =  con->Query(queryLst[iIndex++]);
        if(iRet != 0){
            break;
        }
    }


    int iWalletRet = -1;
    if( iRet == 0){

        if( iPayType == 10 || iPayType == 11){
//            double dPay1 = dTotalMoney,dPay2 = dTotalMoney;
//            if( iPayType == 10){
//                dPay1 = dTotalMoney*2;
//                dPay2 = 0.0;
//            }

            //创建订单(生成分润列表)
//            iWalletRet = WalletCreateOrder(sOrderNo,sPayPhone,lBuyId,sMerPhone, lSellId,sRemark,dPay1,AccountTypeC_FANYUORBINDYUORYU,dPay2,AccountTypeC_TONORLE,"",pResponse);
//            if( iWalletRet != 0){
//                Replacev2(pResponse);
//            }else{
//                pResponse.clear();
//                //支付
//                iWalletRet = Payv4( sOrderNo,sPassWord,pResponse);
//                if( iWalletRet != 0){
//                    Replacev2(pResponse);
//                }
//            }
        }else if(iPayType >= 20 && iPayType <=22){

            if( comeFromDaSaiBaodan  ){
                //孙伟那边支付

                iWalletRet = BaodanPay(sOrderNo,lBuyId,lSellId,iPayType,dTotalMoney,pResponse);

            }else{
                //香港创建订单支付
                HongkongCurrency h1,h2;
                if(iPayType == 20){
                    h1 = HKC_FANSXIAN;
                    h2 = HKC_FANSYU;
                }
                if(iPayType == 21){
                    h1 = HKC_FANSXIAN;
                    h2 = HKC_FANCOIN;
                }
                if(iPayType == 22){
                    h1 = HKC_FANSYU;
                    h2 = HKC_FANCOIN;
                }
                //创建订单(生成分润列表)
                iWalletRet = CreateOrderHongkong(sOrderNo,sPayPhone,lBuyId,sMerPhone, lSellId,sRemark,dTotalMoney,h1,dTotalMoney,h2,"",pResponse);
                if( iWalletRet != 0){
                    Replacev2(pResponse);
                }else{
                    pResponse.clear();
                    //支付
                    iWalletRet = PayByHongkong( sOrderNo,sPassWord,pResponse);
                    if( iWalletRet != 0){
                        Replacev2(pResponse);
                    }
                }
            }

        }
        else{
            //调用桌斌
            //iWalletRet = Pay( sOrderNo,sPayPhone,sMerPhone,sRemark,sPassWord,iPayType,dTotalMoney,dTotalMoney,pResponse);
        }


    }

    if( iRet != 0 || iWalletRet != 0){
        con->RollBack();
    }else{
        con->Commit();
    }
    con->AutoCommit(true);

    //end

    if( iRet == 0 && iWalletRet != 0){//返回钱包服务器的错误
        return  -1;
    }




    UpdatePrice(lSellId,iType);
    UpdatePrice(lBuyId,iType);


    pResponse.clear();
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


/*
Description:报单购买
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::InnerBuy( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    RetJsonMsg::SetRetMsg(-1,pResponse,"disable this call");
    return -1;

    if( pJson["sOrderNo"].isNull() ||  pJson["lBuyId"].isNull() ||  pJson["dPrice"].isNull() || pJson["dCoin"].isNull()
            || pJson["iLots"].isNull()  || pJson["sPassWord"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    int iLots = tConvert.Json2Int( pJson["iLots"]);
    long lBuyId = tConvert.Json2Long( pJson["lBuyId"]);
    string sOrderNo = tConvert.Json2String( pJson["sOrderNo"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);
    double dPrice = tConvert.Json2Double(pJson["dPrice"]);
    double dCoin = tConvert.Json2Double(pJson["dCoin"]);
    long lSellId = 181;//系统号，名下的粉丝内部转卖，得到的钱打入这个号
    string sSellPhone = "11111111231";//181对应手机号

    if( !pJson["lSellId"].isNull()){//如果有指定出售者
        lSellId = tConvert.Json2Long( pJson["lSellId"]);
        UserDataEntity userEntity;
        if( mUserDataDAL.GetEntityById(lSellId,userEntity) != 0){
            RetJsonMsg::SetRetMsg(-1,pResponse,"获取lSellId手机号出错");
            return -1;
        }
        sSellPhone = userEntity.sAccount;

    }


    string sPayPhone,sNameBuy;
    double dTotalMoney = 0.0,dTotalCoin = 0.0;

    UserDataEntity userEntity;
    mUserDataDAL.GetEntityById(lBuyId,userEntity);
    sPayPhone = userEntity.sAccount;
    sNameBuy = userEntity.sNickName;

    dTotalMoney = dPrice*iLots*Param::GetFansPerLots();
    dTotalCoin = dCoin*iLots*Param::GetFansPerLots();


    string sSelledUser;//要被卖的粉丝，格式a,b,c



    vector<string>   queryLst;

    FansBankOrderEntity orderEntity;
    orderEntity.dPrice = dPrice;
    orderEntity.dTurnOver = dTotalMoney;
    orderEntity.iLots = iLots;
    orderEntity.iState = enum_order_ok;
    orderEntity.iType = enum_kvalue_type_push;
    orderEntity.lAddTime = mtConvert.GetCurrentTime();
    orderEntity.lEndTime = orderEntity.lAddTime;
    orderEntity.lIdBuy = lBuyId;
    orderEntity.lIdSell = lSellId;
    orderEntity.sNameBuy = sNameBuy;
    orderEntity.sNameSell = "内部帐号";
    orderEntity.sOrderNo = sOrderNo;




    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        RetJsonMsg::SetRetMsg(-1,pResponse);
        return -1;
    }

    //事务处理
    //start
    con->AutoCommit(false);


    //    int iCanSellLots = 0;
    //    int iFansCount = 0;
    //    mUserDataDAL.GetHotFansCountByPeragent(lSellId,iFansCount);
    //   // string count = "0";
    //   // mFansBankCountDAL.GetValueForLock( mtConvert.LongToStr(lSellId), mtConvert.LongToStr(enum_kvalue_type_push),count);
    //   // iFansCount = mtConvert.StrToInt(count);
    //    iCanSellLots = iFansCount/Param::GetFansPerLots();
    //    if( iCanSellLots < iLots){

    //        con->AutoCommit(true);

    //        RetJsonMsg::SetRetMsg(-1,pResponse,"剩余手数不足");
    //        return -1;

    //    }

    GetSellUserString5(sSellPhone,lSellId,enum_kvalue_type_push,iLots*Param::GetFansPerLots(),sSelledUser);
    std::vector<std::string> resultVector;
    mtConvert.Split(sSelledUser,",",resultVector);
    if( sSelledUser.empty() || resultVector.size() < iLots*Param::GetFansPerLots()){
        con->AutoCommit(true);
        RetJsonMsg::SetRetMsg(-1,pResponse,"粉丝不足");
        return -1;
    }
    queryLst.push_back(mFansBankOrderDAL.GetSqlAdd(orderEntity));//添加一条已完成订单
    //转移推广关系
    queryLst.push_back(mUserDataDAL.GetSqlMovePushMan(lSellId,lBuyId,sSelledUser));
    queryLst.push_back(mFansBankFansmoveDAL.GetSqlAdd(sOrderNo,sSelledUser));

    int iIndex = 0;
    iRet = -1;
    while( iIndex < queryLst.size()){
        iRet =  con->Query(queryLst[iIndex++]);
        if(iRet != 0){
            break;
        }
    }


    int iWalletRet = -1;
    if( iRet == 0){
        iWalletRet = Payv4( sOrderNo,sPassWord,pResponse);
        if( iWalletRet != 0){
            Replacev2(pResponse);
        }

    }

    if( iRet != 0 || iWalletRet != 0){
        con->RollBack();
    }else{
        con->Commit();
    }
    con->AutoCommit(true);

    //end

    if( iRet == 0 && iWalletRet != 0){//返回钱包服务器的错误
        return  -1;
    }



    if( iRet == 0 && iWalletRet == 0 ){
        //把卖掉的粉丝价值改为指定的价格
        mUserValueDAL.UpValuelots(sSelledUser,dPrice,enum_kvalue_type_push);

        UpdatePrice(lBuyId,enum_kvalue_type_push);
    }


    pResponse.clear();
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


/*
Description:赠送
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::FansGive( const  Json::Value  & pJson  ,Json::Value & pResponse)
{

    if(  pJson["lBuyId"].isNull() || pJson["iLots"].isNull() || pJson["lSellId"].isNull()
         || pJson["dPrice"].isNull() || pJson["sOrderNo"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    int iLots = tConvert.Json2Int( pJson["iLots"]);
    long lBuyId = tConvert.Json2Long( pJson["lBuyId"]);
    double dPrice = tConvert.Json2Double( pJson["dPrice"]);
    string sSellPhone ;


    long lSellId = 181;//系统号，名下的粉丝内部转卖
    if(  !pJson["lSellId"].isNull() ){
        string tmp = tConvert.Json2String( pJson["lSellId"]);
        if( !tmp.empty()){
            lSellId = tConvert.Json2Long( pJson["lSellId"]);
        }
    }

    UserDataEntity userEntity;

    if( mUserDataDAL.GetEntityById(lSellId,userEntity) != 0){
        RetJsonMsg::SetRetMsg(-1,pResponse,"获取lSellId手机号出错");
        return -1;
    }
    sSellPhone = userEntity.sAccount;




    string sSelledUser;//要被卖的粉丝，格式a,b,c



    vector<string>   queryLst;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        RetJsonMsg::SetRetMsg(-1,pResponse);
        return -1;
    }

    //事务处理
    //事务start
    con->AutoCommit(false);



    GetSellUserString5(sSellPhone,lSellId,enum_kvalue_type_push,iLots*Param::GetFansPerLots(),sSelledUser);
    std::vector<std::string> resultVector;
    mtConvert.Split(sSelledUser,",",resultVector);
    if( sSelledUser.empty() || resultVector.size() < iLots*Param::GetFansPerLots()){
        con->AutoCommit(true);
        RetJsonMsg::SetRetMsg(-1,pResponse,"粉丝不足");
        return -1;
    }

    //转移推广关系
    queryLst.push_back(mUserDataDAL.GetSqlMovePushMan(lSellId,lBuyId,sSelledUser));

    int iIndex = 0;
    iRet = -1;
    while( iIndex < queryLst.size()){
        iRet =  con->Query(queryLst[iIndex++]);
        if(iRet != 0){
            break;
        }
    }


    if( iRet != 0 ){
        con->RollBack();
    }else{
        con->Commit();
    }
    con->AutoCommit(true);

    //事务end

    if( iRet == 0  ){

        //把卖掉的粉丝价值改为指定的价格
        mUserValueDAL.UpValuelots(sSelledUser,dPrice,enum_kvalue_type_push);
        //更新均价
        UpdatePrice(lBuyId,enum_kvalue_type_push);
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


/*
Description:报单报单下单
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::InnerCreate( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    RetJsonMsg::SetRetMsg(-1,pResponse,"disable this call");
    return -1;

    if( pJson["sOrderNo"].isNull() ||  pJson["lBuyId"].isNull() ||  pJson["dPrice"].isNull() || pJson["dCoin"].isNull()
            || pJson["iLots"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    int iLots = tConvert.Json2Int( pJson["iLots"]);
    long lBuyId = tConvert.Json2Long( pJson["lBuyId"]);
    string sOrderNo = tConvert.Json2String( pJson["sOrderNo"]);
    double dPrice = tConvert.Json2Double(pJson["dPrice"]);
    double dCoin = tConvert.Json2Double(pJson["dCoin"]);
    long lSellId = 181;//系统号，名下的粉丝内部转卖，得到的钱打入这个号
    string sSellPhone = "11111111231";//181对应手机号


    if( !pJson["lSellId"].isNull()){//如果有指定出售者
        lSellId = tConvert.Json2Long( pJson["lSellId"]);
        UserDataEntity userEntity;
        if( mUserDataDAL.GetEntityById(lSellId,userEntity) != 0){
            RetJsonMsg::SetRetMsg(-1,pResponse,"获取lSellId手机号出错");
            return -1;
        }
        sSellPhone = userEntity.sAccount;

    }


    string sPayPhone;
    double dTotalMoney = 0.0,dTotalCoin = 0.0;

    UserDataEntity userEntity;
    if( mUserDataDAL.GetEntityById(lBuyId,userEntity) != 0){
        RetJsonMsg::SetRetMsg(-1,pResponse,"获取lBuyId手机号出错");
        return -1;
    }
    sPayPhone = userEntity.sAccount;

    dTotalMoney = dPrice*iLots*Param::GetFansPerLots();
    dTotalCoin = dCoin*iLots*Param::GetFansPerLots();

    iRet = WalletCreateOrder(sOrderNo,sPayPhone, lBuyId,sSellPhone,lSellId,"C端",dTotalMoney,AccountTypeC_FANSYUE,dTotalCoin,
                             AccountTypeC_TONORLE,"",pResponse);
    if( iRet != 0){
        Replacev2(pResponse);
        return -1;
    }


    pResponse.clear();
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}

//更新实时价格 最高价 最低价 收盘价
void FansBankBLL::UpdatePrice(long Id,int iType)
{
    double dAvg = 0.0;
    if( iType == enum_kvalue_type_platform){

        Json::Value  jRes;
        GetShopEntity(Id,jRes);
        string sShopPhone = mtConvert.Json2String(  jRes["entity"]["sShopPhone"] );
        mUserValueDAL.GetPlatformAvg(sShopPhone,Id,enum_kvalue_type_platform,dAvg);
    }
    else{
        mUserValueDAL.GetPushAvg( Id,enum_kvalue_type_push,dAvg);
    }
    if( dAvg <= 0.0){
        return;
    }
    string sCurDay = mtConvert.GetCurrentDay();
    KValueEntity kvalueEntity;
    mKValueDAL.GetEntity(Id,iType,sCurDay,kvalueEntity);
    //更新实时价格
    mKValueDAL.UpdatePrice(Id,iType,sCurDay ,dAvg);
    //让收盘价等于实时价格
    mKValueDAL.UpdateClose(Id,iType,sCurDay ,dAvg);
    if( kvalueEntity.hight < dAvg){
        //更新最高价
        mKValueDAL.UpdateHigh(Id,iType,sCurDay ,dAvg);
    }
    if( kvalueEntity.low > dAvg){
        //更新最低价
        mKValueDAL.UpdateLow(Id,iType,sCurDay ,dAvg);
    }

}


/*
Description:分页获取订单
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetPageOrder( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["lUserId"].isNull() ||  pJson["iType"].isNull() ||  pJson["iState"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iType = tConvert.Json2Int( pJson["iType"]);
    int iState = tConvert.Json2Int( pJson["iState"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);


    string Ids;
    if( iType == enum_kvalue_type_platform){
        Json::Value jOpen = UserDataCache::GetInstance()->GetMyAllShops(lUserId);
        for(int j= 0; j < jOpen.size() ; j++){

            Ids += tConvert.Json2String( jOpen[j]["lPlatformId"]) + ",";

        }
        Ids = Ids.substr(0,Ids.length() -1);
    }else{
        Ids = mtConvert.LongToStr(lUserId);
    }


    FansBankOrderLst lst;

    if( iState == enum_order_salling){
        mFansBankOrderDAL.GetPageMySalling(iType,Ids,iPage,iCount,lst);
    }else if( iState == enum_order_close){
        mFansBankOrderDAL.GetPageMyCancel(iType,Ids,iPage,iCount,lst);
    }else if( iState == enum_order_ok){
        mFansBankOrderDAL.GetPageMyOk(iType,Ids,iPage,iCount,lst);
    }else if( iState == enum_order_my){
        mFansBankOrderDAL.GetPageMyBuy(iType,Ids,iPage,iCount,lst);
    }


    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];

        MakeK(entity.lIdSell,iType ,tConvert.GetCurrentDay());
        MakeK(entity.lIdSell,iType ,tConvert.GetYesterDay());

        KValueEntity kvalue,ykvalue;
        mKValueDAL.GetEntity(entity.lIdSell, iType,mtConvert.GetCurrentDay(),kvalue);
        mKValueDAL.GetEntity(entity.lIdSell, iType,mtConvert.GetYesterDay(),ykvalue);

        double dTurnOver = 0.0,dPrice = 0.0;
        if( (enum_order_ok == iState) || (enum_order_my == iState) ){
            dTurnOver = entity.dTurnOver;//entity.dPrice*entity.iLots*Param::GetFansPerLots();
            dPrice = entity.dPrice;
        }else{
            dTurnOver = entity.dTurnOver;
            dPrice = kvalue.price;
        }

        int iCanSellLots = 0;
        if( iState == enum_order_salling)
            iCanSellLots = GetCanSellLots(entity.lIdSell,iType);


        pResponse["list"][i]["sOrderNo"] = entity.sOrderNo;
        pResponse["list"][i]["sNameBuy"] = entity.sNameBuy;
        pResponse["list"][i]["sNameSell"] = entity.sNameSell;
        pResponse["list"][i]["lIdBuy"] = mtConvert.LongToStr( entity.lIdBuy );
        pResponse["list"][i]["lIdSell"] = mtConvert.LongToStr( entity.lIdSell );
        pResponse["list"][i]["sAddTime"] = mtConvert.TimeToStr(entity.lAddTime/1000000);
        pResponse["list"][i]["sEndTime"] = mtConvert.TimeToStr(entity.lEndTime/1000000);
        pResponse["list"][i]["iLots"] = entity.iLots;
        pResponse["list"][i]["iType"] = entity.iType;
        pResponse["list"][i]["dPrice"] = dPrice;
        pResponse["list"][i]["dCPrice"] = dPrice;
        pResponse["list"][i]["dUpPrice"] = kvalue.price -ykvalue.close;
        pResponse["list"][i]["dTurnOver"] = dTurnOver;
        pResponse["list"][i]["dCTurnOver"] = dTurnOver;

        pResponse["list"][i]["iLotsOver"] = entity.iLotsOver;

        pResponse["list"][i]["iState"] = iState;

        pResponse["list"][i]["iCanSellLots"] = iCanSellLots;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:分页获取DasaiBaodan订单
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetDasaiBaodanOrder( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    string sOrderNo = tConvert.Json2String( pJson["sOrderNo"]);

    FansBankOrderLst lst;int iTotal = 0;

    mFansBankOrderDAL.GetPageDasaiBaodanOk(iPage,iCount,sOrderNo,lst,iTotal);


    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];



        double dTurnOver = 0.0,dPrice = 0.0;

        dTurnOver = entity.dTurnOver;//entity.dPrice*entity.iLots*Param::GetFansPerLots();
        dPrice = entity.dPrice;



        pResponse["list"][i]["sOrderNo"] = entity.sOrderNo;
        pResponse["list"][i]["sNameBuy"] = entity.sNameBuy;
        pResponse["list"][i]["sNameSell"] = entity.sNameSell;
        pResponse["list"][i]["lIdBuy"] = mtConvert.LongToStr( entity.lIdBuy );
        pResponse["list"][i]["lIdSell"] = mtConvert.LongToStr( entity.lIdSell );
        pResponse["list"][i]["sAddTime"] = mtConvert.TimeToStr(entity.lAddTime/1000000);
        pResponse["list"][i]["sEndTime"] = mtConvert.TimeToStr(entity.lEndTime/1000000);
        pResponse["list"][i]["iLots"] = entity.iLots;
        pResponse["list"][i]["iType"] = entity.iType;
        pResponse["list"][i]["dPrice"] = dPrice;
        pResponse["list"][i]["dCPrice"] = dPrice;
        pResponse["list"][i]["dTurnOver"] = dTurnOver;
        pResponse["list"][i]["dCTurnOver"] = dTurnOver;

        pResponse["list"][i]["iLotsOver"] = entity.iLotsOver;


    }
    pResponse["total"] = iTotal;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:分页获取订单(后台管理)
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetOrderByManage( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    if(   pJson["page"].isNull() || pJson["count"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;

    int iPage = tConvert.Json2Int( pJson["page"]);
    int iCount = tConvert.Json2Int( pJson["count"]);
    int iType = -1;
    int iState = -1;
    long idSell = -1;
    long idBuy = -1;
    long startTime = -1;
    long endTime = -1;

    if( !pJson["type"].isNull() ){
        iType = tConvert.Json2Int( pJson["type"]);
    }
    if( !pJson["state"].isNull() ){
        iState = tConvert.Json2Int( pJson["state"]);
    }
    if( !pJson["idSell"].isNull() ){
        idSell = tConvert.Json2Long( pJson["idSell"]);
    }
    if( !pJson["idBuy"].isNull() ){
        idBuy = tConvert.Json2Long( pJson["idBuy"]);
    }
    if( !pJson["startTime"].isNull() ){
        startTime =  tConvert.StrToTime( tConvert.Json2String( pJson["startTime"]).c_str())*1000000;
    }
    if( !pJson["endTime"].isNull() ){
        endTime = tConvert.StrToTime( tConvert.Json2String( pJson["endTime"]).c_str())*1000000;
    }



    FansBankOrderLst lst;int iTotal = 0;

    mFansBankOrderDAL.GetOrderByManage(iType,iState,idSell,idBuy,startTime,endTime,iPage,iCount,lst,iTotal);



    for( int i = 0; i < lst.size(); i++){
        FansBankOrderEntity & entity = lst[i];

        MakeK(entity.lIdSell,iType ,tConvert.GetCurrentDay());
        MakeK(entity.lIdSell,iType ,tConvert.GetYesterDay());

        KValueEntity kvalue,ykvalue;
        mKValueDAL.GetEntity(entity.lIdSell, iType,mtConvert.GetCurrentDay(),kvalue);
        mKValueDAL.GetEntity(entity.lIdSell, iType,mtConvert.GetYesterDay(),ykvalue);

        double dTurnOver = 0.0,dPrice = 0.0;
        if( (enum_order_ok == iState) || (enum_order_my == iState) ){
            dTurnOver = entity.dTurnOver;//entity.dPrice*entity.iLots*Param::GetFansPerLots();
            dPrice = entity.dPrice;
        }else{
            dTurnOver = entity.dTurnOver;
            dPrice = kvalue.price;
        }


        pResponse["list"][i]["orderNo"] = entity.sOrderNo;
        pResponse["list"][i]["nameBuy"] = entity.sNameBuy;
        pResponse["list"][i]["nameSell"] = entity.sNameSell;
        pResponse["list"][i]["idBuy"] = mtConvert.LongToStr( entity.lIdBuy );
        pResponse["list"][i]["idSell"] = mtConvert.LongToStr( entity.lIdSell );
        pResponse["list"][i]["addTime"] = mtConvert.TimeToStr(entity.lAddTime/1000000);
        pResponse["list"][i]["endTime"] = mtConvert.TimeToStr(entity.lEndTime/1000000);
        pResponse["list"][i]["lots"] = entity.iLots;
        pResponse["list"][i]["type"] = entity.iType;
        pResponse["list"][i]["price"] = dPrice;
        pResponse["list"][i]["cprice"] = dPrice;
        pResponse["list"][i]["upPrice"] = kvalue.price -ykvalue.close;
        pResponse["list"][i]["turnOver"] = dTurnOver;
        pResponse["list"][i]["cturnOver"] = dTurnOver;

        pResponse["list"][i]["lotsOver"] = entity.iLotsOver;

        pResponse["list"][i]["state"] = iState;



    }
    pResponse["total"] = iTotal;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:取消订单
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::CancelOrder( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if( pJson["Id"].isNull()  || pJson["iType"].isNull()|| pJson["sPassWord"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    int iType = tConvert.Json2Int(pJson["iType"]);
    long Id = tConvert.Json2Int(pJson["Id"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);

    string sPayPhone;

    if( iType == enum_kvalue_type_push){

        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(Id,userEntity);
        sPayPhone = userEntity.sAccount;

    }else{
        sPayPhone = GetShopPhone(Id);

    }


    //to check pay password from zhou bin
    if( CheckPayPassWord(sPayPhone, sPassWord,pResponse) != 0){
        return -1;
    }

    iRet = mFansBankOrderDAL.CancelOrder(Id,iType);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:红点提示
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::RedPoin( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if( pJson["lLasts"].isNull()  || !pJson["lLasts"].isArray()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"].resize(0);
    Json::Value lLasts = pJson["lLasts"];

    long lUserId = 0;
    if( !pJson["lUserId"].isNull()){
        lUserId = tConvert.Json2Long( pJson["lUserId"]);
    }
    for( int i=0 ; i < lLasts.size(); i++){

        int iType = tConvert.Json2Int(lLasts[i]["iType"]);
        long lLastTime = tConvert.Json2Long( lLasts[i]["lLastTime"]);
        int iCount = 0;
        iRet = mFansBankOrderDAL.GetNewCount(lUserId,lLastTime,iType,iCount);
        pResponse["list"][i]["iCount"] = iCount;
        pResponse["list"][i]["iType"] = iType;

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/*
Description:
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::GetConfig( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    string id =  mtConvert.Json2String( pJson["id"] );
    CommonStructVector all;
    mFansBankKeyvalueDAL.GetAll(all);

    for( int i=0 ; i < all.size(); i++){

        CommonStruct & en = all[i];
        pResponse[en.v1] = en.v2;

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::SetConfig( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    Json::Value::Members  members = pJson.getMemberNames();
    for( int i = 0 ; i < members.size() ; i++){
        string key =  members[i];
        string value =  mtConvert.Json2String( pJson[key] );
        mFansBankKeyvalueDAL.SetValue(key,value);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

/*
Description:
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::OrderState( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if( pJson["orderNo"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    string  orderNo = tConvert.Json2String( pJson["orderNo"] );

    FansBankOrderEntity en;
    iRet = mFansBankOrderDAL.GetEntity(orderNo,en);
    if( iRet == 0){
        RetJsonMsg::SetRetMsg(0,pResponse);
    }else if( iRet == -5){
        RetJsonMsg::SetRetMsg(-5,pResponse,"订单不存在");
    }else{
        RetJsonMsg::SetRetMsg(-1,pResponse,"");
    }
    return iRet;

}


/*
Description:增加均价（可以是负数，表示减少）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FansBankBLL::ChangePrice( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if( pJson["value"].isNull() || pJson["type"].isNull() || pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    double value = tConvert.Json2Double( pJson["value"] );
    int type = tConvert.Json2Int( pJson["type"] );
    long id = tConvert.Json2Long( pJson["id"] );

    if( abs(value) > 1.0){
        RetJsonMsg::SetRetMsg(-1,pResponse,"增长值不能大于或小于1");
        return -1;
    }

    iRet = mUserValueDAL.IncreaseValue(value,id,type);
    iRet = mKValueDAL.IncreaseValue(value,id,type);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


//检验支付密码,调用桌斌
int FansBankBLL::CheckPayPassWord( string sPhone , string sPassWord,Json::Value & jResponse)
{

    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +"/GetDataInterface/FansTrade/VerifyPayPassWord.aspx";

    Json::Value jPost;
    jPost["sPayPassWord"] = sPassWord;
    jPost["sPhone"] = sPhone;

    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);

    Replace(jResponse);

    return iRet;
}


//获取推广收益,调用桌斌
int FansBankBLL::GetPushIncome( string sPhone ,Json::Value & jResponse)
{
    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +"/GetDataInterface/Consume/ReportForm/GetPushMoneyByPhone.aspx";

    Json::Value jPost;
    jPost["sPhone"] = sPhone;

    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);

    Replace(jResponse);

    return iRet;
}

//获取PlatformId推广收益,调用桌斌
int FansBankBLL::GetPlatformIdIncome( long lPlatformId ,Json::Value & jResponse)
{
    int iRet = -1;
    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/ReportForm/GetPushMoneyByPlatformId.aspx";
    Json::Value jPost;
    jPost["lPlatformId"] = mtConvert.LongToStr(lPlatformId);
    iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);

    Replace(jResponse);

    return iRet;
}

string FansBankBLL::GetShopName(long Id)
{
    Json::Value jPost,jRes;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return mtConvert.Json2String(  jRes["entity"]["sShopName"] );
}

int FansBankBLL::GetShopEntity(long Id,Json::Value & jRes)
{
    Json::Value jPost;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return 0;
}

string FansBankBLL::GetShopPhone(long Id)
{
    Json::Value jPost,jRes;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return mtConvert.Json2String(  jRes["entity"]["sShopPhone"] );
}

long FansBankBLL::GetShopManId(long Id)
{
    Json::Value jPost,jRes;
    jPost["lPlatformId"] = mtConvert.LongToStr(Id);
    Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(), jPost, jRes);
    return mtConvert.Json2Long(  jRes["entity"]["lUserId"] );
}


string FansBankBLL::GetPhoneByUserId(long Id)
{
    UserDataEntity entity;
    mUserDataDAL.GetEntityById(Id,entity);
    return entity.sAccount;
}

string FansBankBLL::MakeAnOrderNum(long Id)
{
    return mtConvert.LongToStr(Id) + mtConvert.LongToStr( (mtConvert.GetCurrentTime()/1000) );
}


//得到可以挂卖的手数
int FansBankBLL::GetCanSellLots(long Id, int iType)
{
    //    int iLotsSelling = 0;
    //    mFansBankOrderDAL.GetLotsSell(Id,iType, iLotsSelling);
    //    int iCount = 0;
    //    mUserDataDAL.GetFansNum(Id,iType,iCount);
    //    return (iCount-1)/Param::GetFansPerLots() - iLotsSelling;

    //////////////////////////
    //    int iLotsSelling = 0;
    //    mFansBankOrderDAL.GetLotsSell(Id,iType, iLotsSelling);

    //    int iCount = 0;
    //    mUserDataDAL.GetConsumFansNum(Id,iType,iCount);
    //    return (iCount-1)/Param::GetFansPerLots() - iLotsSelling;
    //////////////////

    int iLotsSelling = 0;
    mFansBankOrderDAL.GetLotsSell(Id,iType, iLotsSelling);
    int iCount = 0;
    mUserDataDAL.GetWeixinBindNum(Id,iType,iCount);

    return (iCount-1)/Param::GetFansPerLots() - iLotsSelling;


}



//支付，调用桌斌
int FansBankBLL::Pay(string sOrderNo,string sPayPhone, string sMerPhone, string sDetail,string sPayPassWord,
                     int iPayType, double dMoney,double dShopCoin, Json::Value & jResponse)
{
    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +"/GetDataInterface/FansTrade/CreateOrder.aspx";

    Json::Value jPost;
    jPost["sOrderNo"] = sOrderNo;
    jPost["sPayPhone"] = sPayPhone;
    jPost["sMerPhone"] = sMerPhone;
    jPost["sDetail"] = sDetail;
    jPost["iPayType"] = iPayType;
    jPost["dMoney"] = dMoney;
    jPost["dShopCoin"] = dShopCoin;
    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);
    if( iRet == 0){
        string iPayId =  mtConvert.Json2String( jResponse["iPayId"] );

        sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/FansTrade/PayById.aspx";
        jPost.clear();jResponse.clear();
        jPost["iPayId"] = iPayId;
        jPost["sPayPassWord"] = sPayPassWord;
        iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);

        Replace(jResponse);

    }else{
        Replace(jResponse);
    }
    return iRet;
}


//支付，调用yu
//int FansBankBLL::Payv2(string sOrderNo,string sPayPhone, string sMerPhone, string sDetail,string sPayPassWord,
//                       double dMoney,double dShopCoin, Json::Value & jResponse)
//{

//    TConvert tConvert;
//    long timestamp = tConvert.GetCurrentTime()/1000;
//    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
//    string appKey = TBLL::GetInstance()->GetAppKey();
//    string sUrl = "http://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/CreateBuyOrder"
//                                                                      "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);

//    Json::Value jPost;
//    jPost["orderNo"] = sOrderNo;
//    jPost["payerPhone"] = sPayPhone;
//    jPost["payeePhone"] = sMerPhone;
//    jPost["detail"] = sDetail;
//    jPost["money"] = dMoney;
//    jPost["coinMoney"] = dShopCoin;

//    int iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);
//    if( iRet == 0){
//        Json::Value jResult = jResponse["result"];
//        string iPayId =  mtConvert.Json2String( jResult["payId"] );

//        sUrl = "http://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/PayBuyOrder"
//                                                                   "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
//        jPost.clear();jResponse.clear();
//        jPost["payId"] = iPayId;
//        jPost["payPassWord"] = sPayPassWord;
//        iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

//        Replacev2(jResponse);

//    }else{
//        Replacev2(jResponse);
//    }
//    return iRet;
//}

//支付，调用yu
int FansBankBLL::Payv3(string orderNo,string payPhone, string recPhone, string remark,string payPassWord,
                       double payAmount,int payCurrency,double payAmountB,int payCurrencyB,
                       Json::Value & jResponse)
{
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;
    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    string sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/CreateOrder"
                                                                       "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);

    Json::Value jPost;
    jPost["orderNo"] = orderNo;
    jPost["payPhone"] = payPhone;
    jPost["recPhone"] = recPhone;
    jPost["remark"] = remark;
    jPost["payAmount"] = payAmount;
    jPost["payCurrency"] = payCurrency;
    jPost["payAmountB"] = payAmountB;
    jPost["payCurrencyB"] = payCurrencyB;

    int iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);
    if( iRet == 0){
        //Json::Value jResult = jResponse["result"];
        // string iPayId =  mtConvert.Json2String( jResult["payId"] );

        sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/PayOrder"
                                                                    "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
        jPost.clear();jResponse.clear();
        jPost["orderNo"] = orderNo;
        jPost["payPassWord"] = payPassWord;
        iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

        Replacev2(jResponse);

    }else{
        Replacev2(jResponse);
    }
    return iRet;

}

//支付，调用yu
int FansBankBLL::Payv4(string orderNo,string payPassWord,Json::Value & jResponse)
{
    TConvert tConvert;int iRet = -1;
    long timestamp = tConvert.GetCurrentTime()/1000;
    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    Json::Value jPost;

    string sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/PayOrder"
                                                                       "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
    jPost.clear();jResponse.clear();
    jPost["orderNo"] = orderNo;
    jPost["payPassWord"] = payPassWord;
    iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    return iRet;

}


//支付，调用Hongkong
int FansBankBLL::PayByHongkong(string orderNo,string payPassWord,Json::Value & jResponse)
{
    TConvert tConvert;int iRet = -1;
    long timestamp = tConvert.GetCurrentTime()/1000;
    Json::Value jPost;

    string sUrl = "http://" + TBLL::GetInstance()->GetHongkongIP()  +  "/Api/FansBankService/payOrder"
                                                                       "?appKey=" + mHongkongAppKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
    jPost.clear();jResponse.clear();
    jPost["orderNo"] = orderNo;
    jPost["payPassWord"] = payPassWord;
    iRet = Request::WalletHttpPost2(sUrl,mHongkongAPP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    return iRet;

}

//获取粉丝余额信息，调用yu
int FansBankBLL::BalanceGetModel(string phone,int currency,Json::Value & jResponse)
{
    TConvert tConvert;int iRet = -1;
    long timestamp = tConvert.GetCurrentTime()/1000;
    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    Json::Value jPost;

    string sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/trade/TradeBalanceService/getModel"
                                                                       "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);
    jPost.clear();jResponse.clear();
    jPost["phone"] = phone;
    jPost["currency"] = currency;
    iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    return iRet;

}

//创建订单(生成分润列表)，调用yu
int FansBankBLL::WalletCreateOrder(string orderNo,string payPhone,long payId, string recPhone,long recId, string remark,
                                   double payAmount,int payCurrency,double payAmountB,int payCurrencyB,string scene,
                                   Json::Value & jResponse)
{
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;
    string APP_SECRET = TBLL::GetInstance()->GetAppSecret();
    string appKey = TBLL::GetInstance()->GetAppKey();
    string sUrl = "https://" + TBLL::GetInstance()->GetFinanceIP()  +  "/Api/Payment/FansTradeService/CreateOrder"
                                                                       "?appKey=" + appKey  + "&timestamp=" + tConvert.LongToStr(timestamp);

    double payFansMoney = 0.0;//粉丝余额
    if( BalanceGetModel(payPhone,AccountTypeC_FANSYUE,jResponse) != 0){
        // return -1;
    }else{
        payFansMoney = tConvert.Json2Double( jResponse["result"]["amount"] );
    }



    double dTempMoney  = payAmount*(1-COUNTER_FEE_PROP);

    double recPoint3 = dTempMoney*0.3;
    double recPoint7 = dTempMoney*0.7;

    //计算手续费
    double feeAmount  = payAmount*TAXATION_PROP;
    double feeAmountB = 0.0;
    double dFeeProp = payAmount*COUNTER_FEE_PROP - feeAmount;
    //付款人-送通用购物卡
    double givPayCoinMoney  = 0.0;
    //收款人-送乐兑购物卡
    double givRecMallCoinMoney  = payAmountB*(1-COUNTER_FEE_PROP);

    //纯余额支付
    if( payAmountB == 0.0){
        dTempMoney = payAmount*0.5*(1-COUNTER_FEE_PROP);
        feeAmount = payAmount*0.5*TAXATION_PROP;
        dFeeProp = payAmount*0.5*COUNTER_FEE_PROP - feeAmount;
        recPoint3 = dTempMoney*0.3;
        recPoint7 = dTempMoney*0.7;
        if(payFansMoney == 0.0){
            givPayCoinMoney  = payAmount*0.5;
        }
        givRecMallCoinMoney  = dTempMoney;
    }
    //扣收款人-税费百分之3
    double recFreeMoney  = recPoint3*0.03;
    Json::Value jPost;
    jPost["orderNo"] = orderNo;
    jPost["remark"] = remark;
    jPost["payPhone"] = payPhone;
    jPost["payAmount"] = payAmount;
    jPost["payCurrency"] = payCurrency;
    jPost["payAmountB"] = payAmountB;
    jPost["payCurrencyB"] = payCurrencyB;

    jPost["recPhone"] = recPhone;
    jPost["recAmount"] = recPoint3;
    jPost["recCurrency"] = AccountTypeC_YUE;
    jPost["recAmountB"] = recPoint7;
    jPost["recCurrencyB"] = AccountTypeC_FANSYUE;

    Json::Value jDivideList;
    int index = 0;
    if( givPayCoinMoney != 0.0){
        //付款人-送通用积分
        jDivideList[index]["d"] = AccountTypeD_IN;
        jDivideList[index]["t"] = AccountTypeT_GIVECOIN;
        jDivideList[index]["p"] = payPhone;
        jDivideList[index]["a"] = givPayCoinMoney;
        jDivideList[index]["c"] = AccountTypeC_TONYONCOIN;
        index++;
    }
    if( givRecMallCoinMoney != 0.0){
        //收款人-乐兑积分
        jDivideList[index]["d"] = AccountTypeD_IN;
        jDivideList[index]["t"] = AccountTypeT_SALESER;
        jDivideList[index]["p"] = recPhone;
        jDivideList[index]["a"] = givRecMallCoinMoney;
        jDivideList[index]["c"] = AccountTypeC_LEDUICOIN;
        index++;
    }
    if( recFreeMoney != 0.0){
        //扣收款人-税费
        jDivideList[index]["d"] = AccountTypeD_OUT;
        jDivideList[index]["t"] = AccountTypeT_TAX;
        jDivideList[index]["p"] = recPhone;
        jDivideList[index]["a"] = recFreeMoney;
        jDivideList[index]["c"] = AccountTypeC_YUE;
        index++;
    }

    if( recFreeMoney != 0.0){
        //收收款人-税费
        jDivideList[index]["d"] = AccountTypeD_IN;
        jDivideList[index]["t"] = AccountTypeT_TAX;
        jDivideList[index]["p"] = "11111111243";
        jDivideList[index]["a"] = recFreeMoney;
        jDivideList[index]["c"] = AccountTypeC_YUE;
        index++;
    }

    Json::Value jFee,jFeeR;
    jFee["ttdid"] = tConvert.LongToStr( recId );
    jFee["ordernumber"] = orderNo;
    jFee["totaomoney"] = dFeeProp;
    string sunweiUrl = "http://" + TBLL::GetInstance()->GetSunWeiIP() + "/apittd/bdorder/bdorderapi.ashx?do=getprofit";
    Request::SunWeiHttpPost(sunweiUrl,
                            "83f41be38e65411f",tConvert.LongToStr(timestamp),jFee,jFeeR);
    int feeState = tConvert.Json2Int(jFeeR["state"]);
    if( feeState == 1000){//1000 成功
        Json::Value jDlist  = jFeeR["dlist"];
        for( int i = 0; i < jDlist.size(); i++){
            Json::Value e = jDlist[i];
            jDivideList[index]["d"] = e["d"];
            jDivideList[index]["t"] = e["t"];
            jDivideList[index]["p"] = e["p"];
            jDivideList[index]["a"] = e["a"];
            jDivideList[index]["c"] = e["c"];
            index++;
        }
    }



    jPost["divideList"] = jDivideList;

    int iRet = Request::WalletHttpPost2(sUrl,APP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    if( 0 == iRet){
        FenZhangEntiy en;
        en.addTime = tConvert.GetCurrentDayTime();
        en.orderNo = orderNo;
        en.info = jPost.toStyledString();
        mFenZhangDAL.Insert(en);
    }

    return iRet;

}



//创建订单(生成分润列表)，调用hongkongfans
int FansBankBLL::CreateOrderHongkong(string orderNo,string payPhone,long payId, string recPhone,long recId, string remark,
                                     double payAmount,int payCurrency,double payAmountB,int payCurrencyB,string scene,
                                     Json::Value & jResponse)
{
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;

    string sUrl = "http://" + TBLL::GetInstance()->GetHongkongIP()  +  "/Api/FansBankService/createTradeOrder"
                                                                       "?appKey=" + mHongkongAppKey  + "&timestamp=" + tConvert.LongToStr(timestamp);


    double dTempMoney  = payAmount*(1-COUNTER_FEE_PROP);
    double recPoint3 = dTempMoney*0.3;
    double recPoint7 = dTempMoney*0.7;

    //计算手续费
    double dFeeProp = payAmount*COUNTER_FEE_PROP;
    //收款人-送乐兑购物卡
    double givRecMallCoinMoney  = payAmountB*(1-COUNTER_FEE_PROP);



    Json::Value jPost;
    jPost["orderNo"] = orderNo;
    jPost["remark"] = remark;
    jPost["payPhone"] = payPhone;
    jPost["payAmount"] = payAmount;
    jPost["payCurrency"] = payCurrency;
    jPost["payAmountB"] = payAmountB;
    jPost["payCurrencyB"] = payCurrencyB;
    jPost["payAccountId"] = tConvert.LongToStr( payId );

    jPost["recPhone"] = recPhone;
    jPost["recAmount"] = recPoint3;
    jPost["recCurrency"] = HKC_FANSXIAN;
    jPost["recAmountB"] = recPoint7;
    jPost["recCurrencyB"] = HKC_FANSYU;
    jPost["recAccountId"] =tConvert.LongToStr( recId );

    Json::Value jDivideList;
    int index = 0;

    if( givRecMallCoinMoney != 0.0){
        //收款人-送通用积分
        jDivideList[index]["d"] = AccountTypeD_IN;
        jDivideList[index]["t"] = HKT_GIVECOIN;
        jDivideList[index]["p"] = recPhone;
        jDivideList[index]["a"] = givRecMallCoinMoney;
        jDivideList[index]["c"] = HKC_FANCOIN;
        jDivideList[index]["i"] = tConvert.LongToStr(recId);
        index++;
    }
    if( dFeeProp != 0.0){
        //手续费
        jDivideList[index]["d"] = AccountTypeD_IN;
        jDivideList[index]["t"] = HKT_FUWUFEI;
        jDivideList[index]["p"] = mHongkongFuwuFeiPhone;
        jDivideList[index]["a"] = dFeeProp;
        jDivideList[index]["c"] = HKC_FANSYU;
        jDivideList[index]["i"] = tConvert.LongToStr(mHongkongFuwuFeiId);
        index++;
    }




    jPost["divideList"] = jDivideList;

    int iRet = Request::WalletHttpPost2(sUrl,mHongkongAPP_SECRET,tConvert.LongToStr(timestamp),jPost,jResponse);

    if( 0 == iRet){
        FenZhangEntiy en;
        en.addTime = tConvert.GetCurrentDayTime();
        en.orderNo = orderNo;
        en.info = jPost.toStyledString();
        mFenZhangDAL.Insert(en);
    }

    return iRet;

}


//如果不存在某天的K柱，则补充
void FansBankBLL::MakeK(long Id,int iType, const string & sDay)
{
    if( mKValueDAL.IsExist(Id,iType,sDay) == 1){
        double dAvg = 1.0;

        string sName,sShopPhone;

        if( iType == enum_kvalue_type_platform){

            Json::Value  jRes;
            GetShopEntity(Id,jRes);
            sName = mtConvert.Json2String(  jRes["entity"]["sShopName"] );
            sShopPhone = mtConvert.Json2String(  jRes["entity"]["sShopPhone"] );


            mUserValueDAL.GetPlatformAvg(sShopPhone,Id,enum_kvalue_type_platform,dAvg);

        }
        else{
            mUserValueDAL.GetPushAvg( Id,enum_kvalue_type_push,dAvg);
            UserDataEntity userData;
            mUserDataDAL.GetEntityById(Id,userData);
            sName = userData.sNickName;

        }

        KValueEntity yk;
        mKValueDAL.GetEntity(Id,iType,mtConvert.GetYesterDay(),yk);
        if( yk.close <= 0){
            yk.close = GetRandByPrice(dAvg);
        }

        KValueEntity kvalueEntity;

        kvalueEntity.price = dAvg;
        GetRandByPrice(dAvg,kvalueEntity.open,kvalueEntity.close,kvalueEntity.hight,kvalueEntity.low);
        kvalueEntity.turnOver = 0.0;
        kvalueEntity.turnRate = 0.0;
        kvalueEntity.Id = Id;
        kvalueEntity.iType = iType;
        kvalueEntity.sDay = sDay;
        kvalueEntity.sName = sName;
        kvalueEntity.yclose = yk.close;
        mKValueDAL.Add(kvalueEntity);

    }
}

void FansBankBLL::Make20K(long Id,int iType)
{
    long lCurTime = mtConvert.GetCurrentTime()/1000000;
    long lSecondOneDay = 3600*24;
    for( int i=0 ; i<20;i++){
        string sDay = mtConvert.TimeToStr(lCurTime - lSecondOneDay*i).substr(0,10);
        MakeK(Id,iType,sDay);
    }
}

//获取一批要卖的粉丝(按粉丝价值排序后分段，从每段抽出粉丝)
int FansBankBLL::GetSellUserString(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser)
{
    int iCount = 0;
    mUserValueDAL.GetCount(IdSell,iType,iCount);
    //iCount-1:先减去自己
    if( iCount-1 < iNum || (iNum%10 != 0)){
        return -1;
    }
    int iPer = iCount/10;
    int iDuan = iNum/10;

    UserValueLst sellLst;
    for(int j = 0; j < 10 ; j++){
        mUserValueDAL.GetValueUser(sSellPhone,IdSell,iType,true,iPer*j,iDuan,sellLst);
        for( int i = 0; i < sellLst.size(); i++){
            UserValueEntity & entity = sellLst[i];
            sSellUser += mtConvert.LongToStr( entity.lUserId ) + ",";
        }
    }


    if( sellLst.size() > 0 ){
        sSellUser = sSellUser.substr(0, sSellUser.length() -1);
    }
    return 0;

}

//获取一批要卖的粉丝(高价值的粉丝先被卖掉)
int FansBankBLL::GetSellUserString3(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser)
{
    int iCount = 0;
    mUserValueDAL.GetCount(IdSell,iType,iCount);
    //iCount-1:先减去自己
    if( iCount-1 < iNum || (iNum%10 != 0)){
        return -1;
    }

    UserValueLst sellLst;

    mUserValueDAL.GetValueUser(sSellPhone,IdSell,iType,false,0,iNum,sellLst);
    for( int i = 0; i < sellLst.size(); i++){
        UserValueEntity & entity = sellLst[i];
        sSellUser += mtConvert.LongToStr( entity.lUserId ) + ",";
    }

    if( sellLst.size() > 0 ){
        sSellUser = sSellUser.substr(0, sSellUser.length() -1);
    }
    return 0;

}

//获取一批要卖的粉丝(随机)
int FansBankBLL::GetSellUserString4(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser)
{
    int iCount = 0;
    mUserValueDAL.GetCount(IdSell,iType,iCount);
    //iCount-1:先减去自己
    if( iCount-1 < iNum || (iNum%10 != 0)){
        return -1;
    }

    longs sellLst;

    mUserDataDAL.GetRandFans(sSellPhone,IdSell,iType,iNum,sellLst);
    for( int i = 0; i < sellLst.size(); i++){
        long lUserId = sellLst[i];
        sSellUser += mtConvert.LongToStr( lUserId ) + ",";
    }

    if( sellLst.size() > 0 ){
        sSellUser = sSellUser.substr(0, sSellUser.length() -1);
    }
    return 0;

}


//获取一批要卖的粉丝
int FansBankBLL::GetSellUserString5(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser)
{

    longs sellLst;

    mUserDataDAL.GetFansv2(sSellPhone,IdSell,iType,iNum,sellLst);
    for( int i = 0; i < sellLst.size(); i++){
        long lUserId = sellLst[i];
        sSellUser += mtConvert.LongToStr( lUserId ) + ",";
    }

    if( sellLst.size() > 0 ){
        sSellUser = sSellUser.substr(0, sSellUser.length() -1);
    }
    return 0;

}


//获取一批要卖的粉丝(只有线下消费过的)
int FansBankBLL::GetSellUserString2(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser)
{

    longs sellLst;
    mUserDataDAL.GetConsumFans(sSellPhone,IdSell,iType,0,iNum,sellLst);
    if( iNum != sellLst.size()){
        return -1;
    }

    for( int i = 0; i < sellLst.size(); i++){
        long lUserId = sellLst[i];
        sSellUser += mtConvert.LongToStr( lUserId ) + ",";
    }

    if( sellLst.size() > 0 ){
        sSellUser = sSellUser.substr(0, sSellUser.length() -1);
    }
    return 0;

}


double FansBankBLL::CaluSellAvg(UserValueLst & lows)
{
    double dTotal = 0.0;

    for( int i = 0 ; i < lows.size() ; i++){
        UserValueEntity & e = lows[i];
        dTotal += e.dValue;
    }

    return dTotal/(lows.size());

}

void FansBankBLL::Append(UserValueLst & lows, UserValueLst &  hights)
{

    for( int i = 0 ; i < hights.size() ; i++){
        UserValueEntity & e = hights[i];
        lows.push_back(e);
    }


}


double FansBankBLL::GetRandByPrice(double dPrice)
{
    return (MyUtil::GetRand(101) + 50)*dPrice/100 + 1.0;
}

void FansBankBLL::GetRandByPrice(double dPrice, double & dOpen, double & dClose, double & dHight, double & dLow)
{
    double d[5];
    d[0] = dPrice;
    for( int i = 1; i < 5;i++){
        d[i] = GetRandByPrice(dPrice);
    }
    dOpen = d[1];
    dClose = d[2];
    dHight = dPrice;
    dLow = dPrice;

    for( int i = 0; i < 5; i++){
        if( d[i] > dHight){
            dHight = d[i];
        }
        if( d[i] < dLow){
            dLow = d[i];
        }
    }

}


void FansBankBLL::GetTodayYesterdayKvalue(long Id, int iType ,KValueEntity & kvalue, KValueEntity & ykvalue, double & dUpPrice, double & dUplist)
{

    MakeK(Id,iType ,mtConvert.GetCurrentDay());
    MakeK(Id,iType ,mtConvert.GetYesterDay());

    mKValueDAL.GetEntity(Id,iType ,mtConvert.GetCurrentDay() ,kvalue);
    mKValueDAL.GetEntity(Id,iType ,mtConvert.GetYesterDay() ,ykvalue);
    dUplist = 0.0;
    dUpPrice = kvalue.price - ykvalue.close;
    if( ykvalue.close != 0){
        dUplist =  dUpPrice/ykvalue.close*100;
    }

}


void FansBankBLL::Replace(Json::Value & jsonRespons)
{
    TConvert tConvert;
    string err =  tConvert.Json2String( jsonRespons["err"]);
    if( err.empty()){
        err = "服务器异常";
    }
    jsonRespons.removeMember("err");
    jsonRespons["sMsg"] = err;
    if(jsonRespons["iRet"].isNull() ){
        jsonRespons["iRet"] = -1000000;
    }
}

void FansBankBLL::Replacev2(Json::Value & jsonRespons)
{
    TConvert tConvert;
    string err =  tConvert.Json2String( jsonRespons["msg"]);
    if( err.empty()){
        err = "服务器异常";
    }
    jsonRespons.removeMember("msg");
    jsonRespons["sMsg"] = err;
    string state =  tConvert.Json2String( jsonRespons["state"]);
    if(state == "Error" || state.empty()){
        jsonRespons["iRet"] = -1;
    }else{
        jsonRespons["iRet"] = 0;
    }

    jsonRespons.removeMember("state");


}
//是否vip[报单员]
bool  FansBankBLL::IsBadodanMember(long lUserId){
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;

    Json::Value jFee,jFeeR;

    jFee["ttdid"] = tConvert.LongToStr( lUserId );
    string sunweiUrl = "http://" + TBLL::GetInstance()->GetSunWeiIP() + "/apittd/bdorder/bdorderapi.ashx?do=isvip";
    Request::SunWeiHttpPost(sunweiUrl,
                            "83f41be38e65411f",tConvert.LongToStr(timestamp),jFee,jFeeR);

    int isvip  = tConvert.Json2Int( jFeeR["state"] );
    bool bIsVip;
    ( 0== isvip)?bIsVip = true:bIsVip = false;
    return bIsVip;
}

//孙伟那边的交易支付
int FansBankBLL::BaodanPay(string orderNo,long buy, long sell, int type,double amount,Json::Value & jResponse){
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;

    Json::Value jFee;

    jFee["orderNo"] = orderNo;
    jFee["ttdid"] = tConvert.LongToStr( buy );
    jFee["refttdid"] = tConvert.LongToStr( sell );
    jFee["type"] = type;
    jFee["totalmoney"] =MyUtil::Floor( amount,2);
    string sunweiUrl = "http://" + TBLL::GetInstance()->GetSunWeiIP() + "/apittd/bdorder/bdorderapi.ashx?do=payfansbank";
    Request::SunWeiHttpPost(sunweiUrl,
                            "83f41be38e65411f",tConvert.LongToStr(timestamp),jFee,jResponse);

    int state  = tConvert.Json2Int( jResponse["state"] );

    if( state != 1000){
        jResponse["iRet"] = -1;
        string msg = tConvert.Json2String(jResponse["msg"]);
        jResponse["sMsg"] = msg;
        jResponse.removeMember("msg");
        jResponse.removeMember("state");
    }else{
        state = 0;
    }
    return state;
}

//是否是否大赛会员
bool FansBankBLL::IsDaSaiMember(long lUserId)
{
    TConvert tConvert;
    long timestamp = tConvert.GetCurrentTime()/1000;

    Json::Value jFee,jFeeR;

    //171.17.1.110:10011
   // string sunweiUrl = "http://" + "admin.myfensbank.com:10011/Api/AuthorityService/isMatchUser?appKey=ca84d1343b96baa8137c943ed1860e522cacb238&timestamp=" + tConvert.LongToStr(timestamp);
    string sunweiUrl = "http://" + TBLL::GetInstance()->GetKonhaiIP() + "/Api/AuthorityService/isMatchUser?appKey=ca84d1343b96baa8137c943ed1860e522cacb238&timestamp=" + tConvert.LongToStr(timestamp);

    jFee.clear();jFeeR.clear();
    jFee["userId"] = tConvert.LongToStr( lUserId );
    Request::WalletHttpPost2(sunweiUrl,"96baa8137c943ed1",tConvert.LongToStr(timestamp),jFee,jFeeR);

    bool bIsDasai = false;
    if( tConvert.Json2String( jFeeR["state"] ) == "Success"){
        if( tConvert.Json2Bool( jFeeR["result"]) ){
            bIsDasai = true;
        }
    }
    return bIsDasai;

}
