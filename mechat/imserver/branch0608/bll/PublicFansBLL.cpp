

#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "TBLL.h"
#include "MyAES.h"
#include "TMd5.h"
#include "CmdValue.h"
#include "com/PostsAes.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "com/PushDataCOM.h"
#include "dal/UserSetDAL.h"
#include "dal/UserDataDAL.h"
#include "PublicFansBLL.h"
#include "bll/LukyMoneyBLL.h"
#include "cache/UserDataCache.h"
#include "com/Request.h"

using namespace std;

/**
 *Copyright(C),TTD
 * 公众号粉丝接口
 * @author suyp
 * @modify suyp
 */






/**
 * 外部入口
 * @param psMethod
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Tick" == psMethod){
        iRet = Tick(pJson,pResponse);
    }else if( "MyPublic" == psMethod){
        iRet = MyPublic(pJson,pResponse);
    }else if( "UnAttention" == psMethod){
        iRet = UnAttention(pJson,pResponse);
    }else if( "Detail" == psMethod){
        iRet = Detail(pJson,pResponse);
    }else if( "ConsumerPage" == psMethod){
        iRet = ConsumerPage(pJson,pResponse);
    }else if( "Luky" == psMethod){
        iRet = Luky(pJson,pResponse);
    }else if( "UnPackLk" == psMethod){
        iRet = UnPackLk(pJson,pResponse);
    }else if( "Chat" == psMethod){
        iRet = Chat(pJson,pResponse);
    }else if( "GetInfo" == psMethod){
        iRet = GetInfo(pJson,pResponse);
    }else if( "PublicData" == psMethod){
        iRet = PublicData(pJson,pResponse);
    }
    else {
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}



/**
 * 初始化
 * @param
 * @return 0 sucess
 */
int PublicFansBLL::Init()
{

    return 0;
}

/**
 * 天天兑用户在附近商家消费(钱包调用)
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Tick( const  Json::Value  & pJson, Json::Value & pResponse )
{

    if( pJson["sPayPhone"].isNull() || pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    string sPayPhone = tConvert.Json2String(pJson["sPayPhone"]);
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    long lUserId = 0;

    iRet = mUserDataDAL.GetUserIdByPhone(sPayPhone,lUserId);
    if( iRet == 0){
        if( mPublicFansDAL.IsExist(lPlatformId, lUserId) != 0){
            PublicFansEntity publicFans;
            publicFans.iCount = 1;
            publicFans.Id = lPlatformId;
            publicFans.iType = 0;
            publicFans.lAddTime = tConvert.GetCurrentTime();
            publicFans.lUpTime = publicFans.lAddTime;
            publicFans.lFansId = lUserId;

            iRet = mPublicFansDAL.AddData(publicFans);
        }else
            iRet = mPublicFansDAL.UpdateCount(lPlatformId,lUserId,1);
    }



    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/**
 * 获取我加入的公众号
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::MyPublic( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    if( pJson["lUserId"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"];
    pResponse["list"].resize(0);

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    LongStringMap longMap;

    PublicFansList publics;
    mPublicFansDAL.GetMyPublic(lUserId,publics);


    for(PublicFansList::iterator it = publics.begin(); it != publics.end() ; it++){
        PublicFansEntity & entity = *it;
        longMap[entity.Id] = "";
    }


    Json::Value jOpen = UserDataCache::GetInstance()->GetMyNearShops(lUserId);
    for(int j= 0; j < jOpen.size() ; j++){

        longMap[ tConvert.Json2Long( jOpen[j]["lPlatformId"])] = "";

    }

    UserDataEntity myAtten;
    mUserDataDAL.GetEntityById(lUserId,myAtten);

    longMap[  myAtten.lCurrentPlatformId] = "";

    int i = 0;
    for( LongStringMap::iterator it = longMap.begin(); it != longMap.end(); it ++){
        long Id = it->first;

        Json::Value jPost, jResponse;
        jPost["lPlatformId" ] = tConvert.LongToStr( Id );
        if( 0 == Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jPost,jResponse)){
            pResponse["list"][i]["sPublicName"] = jResponse["entity"]["sShopName"];
            pResponse["list"][i]["lMasterId"] = jResponse["entity"]["lUserId"];
            pResponse["list"][i]["sServerPhone"] = jResponse["entity"]["sSevvicePhone"] ;

            if( tConvert.Json2String( jResponse["entity"]["sSevvicePhone"]).empty()){
                pResponse["list"][i]["sServerPhone"] = "4000555162";//显示公司客服电话

            }
        }else{
            pResponse["list"][i]["sPublicName"] = "";
            pResponse["list"][i]["lMasterId"] = "";
            pResponse["list"][i]["sServerPhone"] = "" ;
        }

        pResponse["list"][i]["Id"] = tConvert.LongToStr( Id );
        pResponse["list"][i]["sHead"] = "http://tymuserhead.oss-cn-shenzhen.aliyuncs.com/7A95A5D6-A8EB-4EE1-878A-97F86AD076AF_iPicHeight=638.00_iPicWide=638.00_fPicSize=103465.jpg";

        i++;
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}


/**
 * 获取公众号上的消费等详细
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Detail( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;int iRet = -1;
    if( pJson["Id"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["Consumer"];
    pResponse["Consumer"].resize(0);
    pResponse["Attentions"];
    pResponse["Attentions"].resize(0);

    long Id = tConvert.Json2Long(pJson["Id"]);
    int iAttentionCount = 0, iAttentionMale=0;
    //得到一个平台上的关注粉丝数
    mUserDataDAL.GetFansByPlatformId(Id, iAttentionCount);
    //获取平台的关注男性粉丝数
    mUserDataDAL.GetMaleFansByPlatformId(Id, iAttentionMale);

    int iConsumerCount = 0, iConsumerMale;
    //得到一个平台上的消费粉丝数
    mPublicFansDAL.GetConsumer(Id, iConsumerCount);
    //得到一个平台上的消费男性粉丝数
    mPublicFansDAL.GetMaleConsumer(Id,iConsumerMale);

    UserDataList userDataLst;
    //获取10个消费粉丝
    mPublicFansDAL.GetConsumer(Id,0,10,userDataLst);
    UserDataList userAttenLst;int iTmp;
    mUserDataDAL.GetFans(Id,1,1,10,"",-1,"null","null","null","null","null","null",userAttenLst,iTmp);


    pResponse["iAttent"] =  iAttentionCount;
    pResponse["iConsum"] =  iConsumerCount;

    pResponse["iMaleAttent"] =  GetPersent(iAttentionMale,iAttentionCount);
    pResponse["iFemaleAttent"] =   GetPersent(iAttentionCount - iAttentionMale,iAttentionCount) ;
    pResponse["iMaleConsum"] =  GetPersent( iConsumerMale ,iConsumerCount );
    pResponse["iFemaleConsum"] = GetPersent( iConsumerCount - iConsumerMale ,iConsumerCount ) ;
    pResponse["bSexAttent"] = true;
    if(iAttentionCount == 0){
        pResponse["bSexAttent"] = false;
    }
    pResponse["bSexConsum"] = true;
    if(iConsumerCount == 0){
        pResponse["bSexConsum"] = false;
    }





    int i = 0;
    for( UserDataList::iterator it = userDataLst.begin(); it != userDataLst.end(); it++){
        UserDataEntity & en =*it;
        pResponse["Consumer"][i]["lUserId"] = tConvert.LongToStr( en.lUserId );
        pResponse["Consumer"][i]["sNickName"] = en.sNickName;
        pResponse["Consumer"][i]["sHeadImage"] = en.sHeadImage;
        i++;
    }

    i = 0;
    for( UserDataList::iterator it = userAttenLst.begin(); it != userAttenLst.end(); it++){
        UserDataEntity & en =*it;
        pResponse["Attentions"][i]["lUserId"] = tConvert.LongToStr( en.lUserId );
        pResponse["Attentions"][i]["sNickName"] = en.sNickName;
        pResponse["Attentions"][i]["sHeadImage"] = en.sHeadImage;
        i++;
    }


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/ReportForm/GetReportFormToUser.aspx";
    Json::Value jPost,jResponse;
    jPost["lPlatformId"] = tConvert.LongToStr(Id);
    iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);
    appendlog(TTDLogger::LOG_DEBUG,"GetReportFormToUser.aspx from wallet:%s",jResponse.toStyledString().c_str());
    if( 0 == iRet){
        pResponse["dMemberTotalMoney"] = jResponse["dMemberTotalMoney"];
        pResponse["dPlusTotalMoney"] = jResponse["dPlusTotalMoney"];
        pResponse["DayRecInfo"] = jResponse["listDayRecInfo"];
        pResponse["listMemberHourRecInfo"] = jResponse["listMemberHourRecInfo"];
        pResponse["listFansHourRecInfo"] = jResponse["listFansHourRecInfo"];
        pResponse["listFansHourRecInfo"].resize(0);
        pResponse["listMemberHourRecInfo"].resize(0);

        bool bClear = true;
        for( int k = 0; k < jResponse["listMemberHourRecInfo"].size() ; k++){
            Json::Value jValue = jResponse["listMemberHourRecInfo"][k];
            double dPer = tConvert.Json2Double( jValue["iProp"] );
            if( dPer > 0){
                bClear = false;
                break;
            }
        }
        if( bClear){
            jResponse["listMemberHourRecInfo"].resize(0);
        }
        bClear = true;
        for( int k = 0; k < jResponse["listFansHourRecInfo"].size() ; k++){
            Json::Value jValue = jResponse["listFansHourRecInfo"][k];
            double dPer = tConvert.Json2Double( jValue["iProp"] );
            if( dPer > 0){
                bClear = false;
                break;
            }
        }
        if( bClear){
            jResponse["listFansHourRecInfo"].resize(0);
        }

        int i100 = 100;
        for( int k = 0,i = 0; k < jResponse["listMemberHourRecInfo"].size() ; k++){
            Json::Value jValue = jResponse["listMemberHourRecInfo"][k];
            double dPer = tConvert.Json2Double( jValue["iProp"] );
            //if( dPer > 0){

                pResponse["listMemberHourRecInfo"][i] = jResponse["listMemberHourRecInfo"][k];
                if(dPer < 1){
                    pResponse["listMemberHourRecInfo"][i]["iProp"] = 1;
                    i100 -= 1;
                }else{
                    if( k ==jResponse["listMemberHourRecInfo"].size() -1 ){
                        pResponse["listMemberHourRecInfo"][i]["iProp"] = i100;
                    }else{
                        pResponse["listMemberHourRecInfo"][i]["iProp"] = (int)dPer;
                    }
                    i100 -= (int)dPer;
                }
                i++;

          //  }
        }



        i100 = 100;
        for( int k = 0,i = 0; k < jResponse["listFansHourRecInfo"].size() ; k++){
            Json::Value jValue = jResponse["listFansHourRecInfo"][k];
            double dPer = tConvert.Json2Double( jValue["iProp"] );
           // if( dPer > 0){

                pResponse["listFansHourRecInfo"][i] = jResponse["listFansHourRecInfo"][k];
                if(dPer < 1){
                    pResponse["listFansHourRecInfo"][i]["iProp"] = 1;
                    i100 -= 1;
                }else{
                    if( k ==jResponse["listFansHourRecInfo"].size() -1 ){
                        pResponse["listFansHourRecInfo"][i]["iProp"] = i100;
                    }else{
                        pResponse["listFansHourRecInfo"][i]["iProp"] = (int)dPer;
                    }
                    i100 -= (int)dPer;
                }
                i++;

           // }
        }


    }else{
        pResponse["dMemberTotalMoney"] = 0.0;
        pResponse["dPlusTotalMoney"] = 0.0;
        pResponse["DayRecInfo"];
        pResponse["DayRecInfo"].resize(0);
        pResponse["listFansHourRecInfo"].resize(0);
        pResponse["listMemberHourRecInfo"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}



/**
 * 获取公众号的消费粉丝（包括头像，昵称）（按分页批量）
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::ConsumerPage( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;int iRet = -1;
    if( pJson["Id"].isNull()  || pJson["iPage"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["list"];
    pResponse["list"].resize(0);
    pResponse["iTotal"] = 0;

    long Id = tConvert.Json2Long(pJson["Id"]);
    int iPage = tConvert.Json2Int(pJson["iPage"]);
    int iCount = 100;

    UserDataList userDataLst;
    iRet = mPublicFansDAL.GetConsumer(Id,(iPage -1)*iCount,iCount,userDataLst);

    int i = 0;
    for( UserDataList::iterator it = userDataLst.begin(); it != userDataLst.end(); it++){
        UserDataEntity & en =*it;
        pResponse["list"][i]["lUserId"] = tConvert.LongToStr( en.lUserId );
        pResponse["list"][i]["sNickName"] = en.sNickName;
        pResponse["list"][i]["sHeadImage"] = en.sHeadImage;
        i++;
    }

    //获取近一个月时间内的消费粉丝数
    int iTotal = 0;long lTime  =tConvert.GetCurrentTime();
    long lDis = 2592000000000;//一个月时间内
    iRet = mPublicFansDAL.GetCountsConsumer(Id,lTime -lDis,iTotal);
    pResponse["iTotal"] = iTotal;

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/**
 * 取消关注公众号
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::UnAttention( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;int iRet = -1;
    if( pJson["lUserId"].isNull()  || pJson["Id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long Id = tConvert.Json2Long(pJson["Id"]);

    iRet = mPublicFansDAL.ResetCount(Id,lUserId);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

/**
 * 在公众号发布一个红包
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Luky( const  Json::Value  & pJson, Json::Value & pResponse )
{

    if( pJson["lUserId"].isNull() || pJson["Id"].isNull() || pJson["dMoney"].isNull()  || pJson["iNum"].isNull() || pJson["sPassWord"].isNull()
            || pJson["sOrderNo"].isNull() || pJson["bRand"].isNull() || pJson["iDest"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long Id = tConvert.Json2Long(pJson["Id"]);
    double dMoney = tConvert.Json2Double(pJson["dMoney"]);
    int iNum = tConvert.Json2Int(pJson["iNum"]);
    string sNum  = tConvert.Json2String(pJson["iNum"]);
    int iPos = sNum.find("个");//为了兼容IOS的错误（多了个‘个’字）
    if( iPos != string::npos){
        sNum = sNum.substr(0,iPos);
        iNum = atoi(sNum.c_str());
    }
    string sPassWord = tConvert.Json2String(pJson["sPassWord"]);
    string sOrderNo = tConvert.Json2String(pJson["sOrderNo"]);
    bool bRand = tConvert.Json2Bool(pJson["bRand"]);
    int iRand = 3;
    if( bRand)
        iRand = 1;

    int iDest = tConvert.Json2Int(pJson["iDest"]);


    UserDataList userDataLst;string sRev;

    if( iDest == 2 ){
        //获取xx个会员
        mPublicFansDAL.GetConsumer(Id,0,1000,userDataLst);
    }else{
        mUserDataDAL.GetAttentionByPlatformId(Id,0,1000,userDataLst);
    }

    LongLst lRevUserIdLst;
    for( UserDataList::const_iterator it = userDataLst.begin(); it != userDataLst.end(); it++)
    {
        const UserDataEntity & en = *it;
        lRevUserIdLst.push_back(en.lUserId);
        sRev += tConvert.LongToStr(en.lUserId) + ",";
    }

    //call wallet interface
    LukyMoneyBLL LuckyMoney;
    iRet = LuckyMoney.LukyTransfer(lUserId,lRevUserIdLst,sPassWord,iNum,dMoney,3,0,iRand,sOrderNo,pResponse);

    if( iRet != 0){//pResponse已经被设置错信息
        return -1;
    }


    PublicInfoEntity pubInfo;
    if( 0 == iRet){


        pubInfo.Id = Id;
        pubInfo.iType = enum_chattype_lkmoney;
        pubInfo.lAddTime = tConvert.GetCurrentTime();
        pubInfo.lUpTime = pubInfo.lAddTime;
        pubInfo.lSendId = lUserId;
        pubInfo.sContent = tConvert.Json2String(pJson["dMoney"]) + "," + tConvert.Json2String(pJson["iNum"]);
        pubInfo.sKey = sOrderNo;
        pubInfo.sReaded = "";
        pubInfo.sRev = sRev;
        if( pubInfo.sRev.empty())
            pubInfo.sRev = "-";

        iRet = mPublicInfoDAL.AddData(pubInfo);


        if( 0 == iRet){
            UserDataEntity userDataEntity;
            mUserDataDAL.GetEntity(tConvert.LongToStr(lUserId),"-1nochange", "-1nochange",userDataEntity);

            long lMasterId = 0;
            string sPublicName;
            Json::Value jPost, jResponse;
            jPost["lPlatformId" ] = tConvert.LongToStr( Id );
            if( 0 == Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jPost,jResponse)){
                sPublicName = tConvert.Json2String( jResponse["entity"]["sShopName"] );
                lMasterId = tConvert.Json2Long( jResponse["entity"]["lUserId"] );
            }


            for( UserDataList::const_iterator it = userDataLst.begin(); it != userDataLst.end(); it++)
            {
                const UserDataEntity & en = *it;

                if( en.lUserId == lUserId){//不能推送给自己
                    continue;
                }

                PushNewChatCmd(  en.lUserId, lUserId,userDataEntity.sNickName, userDataEntity.sHeadImage,
                                 Id,pubInfo.sKey, pubInfo.lAddTime,pubInfo.sContent,pubInfo.iType,sPublicName,lMasterId);

            }


        }

    }

    pResponse["sKey"] = sOrderNo;
    pResponse["lAddTime"] =  tConvert.LongToStr(pubInfo.lAddTime);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/**
 * 在公众号抢一个红包
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::UnPackLk(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet;

    if( pJson["lUserId"].isNull() ||  pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);

    PublicInfoEntity entity;
    iRet = mPublicInfoDAL.GetEntity(sKey,entity);

    if( iRet != 0 ||  entity.iType !=enum_chattype_lkmoney ){//找不到对应的红包
        RetJsonMsg::SetRetMsg(-133,pResponse);
        return -1;
    }


    if( (tConvert.GetCurrentTime() - entity.lAddTime)/1000000 > 60*60*24 ){//红包已过期
        RetJsonMsg::SetRetMsg(-134,pResponse);
        return -1;
    }

    //call wallet interface
    LukyMoneyBLL LuckyMoney;
    iRet = LuckyMoney.TransferConfirm(lUserId, sKey,pResponse);
    if( iRet != 0){
        return iRet;//pResponse已经被设置错信息
    }


    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


/**
 * 在公众号聊天
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Chat(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lUserId"].isNull() ||  pJson["Id"].isNull() ||  pJson["lToUserId"].isNull()
            || pJson["iType"].isNull() || pJson["sContent"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sContent = tConvert.Json2String(pJson["sContent"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
    long Id = tConvert.Json2Long( pJson["Id"]);
    long lToUserId = tConvert.Json2Long( pJson["lToUserId"]);
    int iType = tConvert.Json2Int( pJson["iType"]);


    PublicInfoEntity entity;
    entity.Id = Id;
    entity.iType = iType;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.lSendId = lUserId;
    entity.sContent = sContent;
    entity.sKey = tConvert.LongToStr(lUserId) + tConvert.LongToStr(entity.lAddTime);


    entity.sRev = tConvert.LongToStr(lToUserId);



    iRet = mPublicInfoDAL.AddData(entity);


    if( 0 == iRet ){

        UserDataEntity userDataEntity;
        mUserDataDAL.GetEntity(tConvert.LongToStr(lUserId),"-1nochange", "-1nochange",userDataEntity);

        long lMasterId = 0;
        string sPublicName;
        Json::Value jPost, jResponse;
        jPost["lPlatformId" ] = tConvert.LongToStr( entity.Id );
        if( 0 == Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jPost,jResponse)){
            sPublicName = tConvert.Json2String( jResponse["entity"]["sShopName"] );
            lMasterId = tConvert.Json2Long( jResponse["entity"]["lUserId"] );
        }

        PushNewChatCmd(  lToUserId, lUserId,userDataEntity.sNickName, userDataEntity.sHeadImage,
                         Id,entity.sKey, entity.lAddTime,sContent,iType,sPublicName,lMasterId);
    }

    pResponse["sKey"] = entity.sKey;
    pResponse["lAddTime"] = tConvert.LongToStr( entity.lAddTime );

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


//推送一条聊天
int PublicFansBLL::PushNewChatCmd( long lUserId, long lSendId,const string sNickName, const string sHeadImage,long Id,const string & sKey, long lAddTime,
                                  const string & sContent, int iType,const string & sPublicName, long lMasterId)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_public_chat;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["lSendId"] =  tConvert.LongToStr( lSendId);
    jsonPush["Entity"]["sKey"] =  sKey;
    jsonPush["Entity"]["sContent"] =  sContent;
    jsonPush["Entity"]["iType"] =  iType;
    jsonPush["Entity"]["Id"] =  tConvert.LongToStr( Id);
    jsonPush["Entity"]["lAddTime"] =  tConvert.LongToStr( lAddTime);
    jsonPush["Entity"]["sNickName"] =  sNickName;
    jsonPush["Entity"]["sHeadImage"] =  sHeadImage;
    jsonPush["Entity"]["sPublicName"] =  sPublicName;
    jsonPush["Entity"]["lMasterId"] = tConvert.LongToStr( lMasterId );

    PushBLL::GetInstance()->Push(jsonPush,NULL,NULL);
}


/**
 * 获取公众号信息（包括发布的，聊天的）
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::GetInfo(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["lUserId"].isNull()  || pJson["lAddTime"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    pResponse["list"] ;
    pResponse["list"].resize(0);

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lAddTime = tConvert.Json2Long(pJson["lAddTime"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);
    longs Ids;


    if( lAddTime == 0){//0表示最新
        lAddTime = 9480400983864318;//未来的时间
    }

    if( !pJson["Id"].isNull()){
        long Id = tConvert.Json2Long(pJson["Id"]);
        Ids.push_back(Id);

    }else{

        PublicFansList publics;
        mPublicFansDAL.GetMyPublic(lUserId,publics);

        int i = 0;
        for(PublicFansList::iterator it = publics.begin(); it != publics.end() ; it++){
            PublicFansEntity & entity = *it;

            Ids.push_back(entity.Id );
            i++;
        }


        UserDataEntity userEntity;
        mUserDataDAL.GetEntityById(lUserId,userEntity);
        Ids.push_back(userEntity.lCurrentPlatformId);

        Json::Value jOpen = UserDataCache::GetInstance()->GetMyNearShops(lUserId);
        for(int j= 0; j < jOpen.size() ; j++){

            Ids.push_back( tConvert.Json2Long( jOpen[j]["lPlatformId"]));

        }

    }

    PublicInfoList pubInfolst;
    if ( 0 != mPublicInfoDAL.GetMyInfo( lUserId,Ids,lAddTime ,iCount,pubInfolst) ){
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    long lNextTime = 9480400983864318;//未来的时间

    for(int iRow = 0; iRow < pubInfolst.size(); ++iRow ){

        PublicInfoEntity & entity = pubInfolst[pubInfolst.size() - iRow - 1];
        pResponse["list"][iRow]["sKey"] = entity.sKey;
        pResponse["list"][iRow]["lSendId"] = tConvert.LongToStr(entity.lSendId);
        pResponse["list"][iRow]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iRow]["Id"] = tConvert.LongToStr(entity.Id);
        pResponse["list"][iRow]["iType"] = entity.iType;
        pResponse["list"][iRow]["sContent"] = entity.sContent;

        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(entity.lSendId),"-1nochange", "-1nochange",userDataEntity);

        pResponse["list"][iRow]["sNickName"] = userDataEntity.sNickName;
        pResponse["list"][iRow]["sHeadImage"] = userDataEntity.sHeadImage;


        Json::Value jPost, jResponse;
        jPost["lPlatformId" ] = tConvert.LongToStr( entity.Id );
        if( 0 == Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jPost,jResponse)){
            pResponse["list"][iRow]["sPublicName"] = jResponse["entity"]["sShopName"];
            pResponse["list"][iRow]["lMasterId"] = jResponse["entity"]["lUserId"];
        }else{
            pResponse["list"][iRow]["sPublicName"] = "";
            pResponse["list"][iRow]["lMasterId"] = "";
        }

        if( entity.lAddTime < lNextTime)
            lNextTime = entity.lAddTime;
    }

    mPublicInfoDAL.UpdateReaded( lUserId ,pubInfolst);


    pResponse["lNextTime"] = tConvert.LongToStr(lNextTime);
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/**
 * 获取公众号资料
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::PublicData(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if(  pJson["Id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long Id = tConvert.Json2Long( pJson["Id"]);

    Json::Value jPost, jResponse;
    jPost["lPlatformId" ] = tConvert.LongToStr( Id );
    if( 0 == Request::ShopGetEntity(TBLL::GetInstance()->GetMepayIP(),jPost,jResponse)){
        pResponse["entity"]["sPublicName"] = jResponse["entity"]["sShopName"];
        pResponse["entity"]["lMasterId"] = jResponse["entity"]["lUserId"];
    }else{
        pResponse["entity"]["sPublicName"] = "";
        pResponse["entity"]["lMasterId"] = "";
    }

    pResponse["entity"]["Id"] = tConvert.LongToStr( Id );
    pResponse["entity"]["sHead"] = "http://tymuserhead.oss-cn-shenzhen.aliyuncs.com/7A95A5D6-A8EB-4EE1-878A-97F86AD076AF_iPicHeight=638.00_iPicWide=638.00_fPicSize=103465.jpg";
    pResponse["entity"]["sServerPhone"] = "400-xxxxxxxx" ;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}


/**
 * 公众号中发布一条信息（只能公众号所有者发布）
 * @param pJson
 * @param pResponse
 * @return 0 sucess
 */
int PublicFansBLL::Release(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lUserId"].isNull() ||  pJson["Id"].isNull()
            || pJson["iType"].isNull() || pJson["sContent"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sContent = tConvert.Json2String(pJson["sContent"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
    long Id = tConvert.Json2Long( pJson["Id"]);
    int iType = tConvert.Json2Int( pJson["iType"]);


    PublicInfoEntity entity;
    entity.Id = Id;
    entity.iType = iType;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.lSendId = lUserId;
    entity.sContent = sContent;
    entity.sKey = tConvert.LongToStr(lUserId) + tConvert.LongToStr(entity.lAddTime);


    entity.sRev = "";



    iRet = mPublicInfoDAL.AddData(entity);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}



//void PublicFansBLL::SortByPin(const PublicDataList & publicDatalst,PingYingMap & pingMap)
//{
//    for( int i = 0; i < publicDatalst.size(); i++){
//       PublicDataEntity & entity =  publicDatalst[i];

//       char in_utf8[128] = {0};
//       char out_gb2312[255] = {0};
//       char out_ping[256] = {0};
//       strncpy(in_utf8,entity.sName.c_str(),128);
//       //unicode码转为gb2312码
//       //tConvert.u2g(in_utf8,strlen(in_utf8),out_gb2312,255);
//       tConvert.utf8togb2312(in_utf8,strlen(in_utf8), out_gb2312,255);
//       //得到地名拼音
//       pinyin_gb2312(out_gb2312, out_ping, true, false, false, false, false);

//       pingMap[""] = ;

//    }

//}


/*
Description:推送成功回调
Input:  @value 回调数据
Output: null
Return: null
*/
void PublicFansBLL::Sucess(const Json::Value & value)
{

    return;
}
/*
Description:推送失败回调
Input:  @value 回调数据
Output: null
Return: null
*/
void PublicFansBLL::Fail(const Json::Value & value)
{
    return;
}

//计算百份比
int PublicFansBLL::GetPersent(int iCount, int iTotal)
{
    if( iTotal <= 0)
        return 0;

    return ((double)iCount/(double)iTotal)*100;
}
