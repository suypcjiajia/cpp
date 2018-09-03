/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装私聊业务接口
 *Others:
*/


#include "PrivateChatBLL.h"
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
#include "com/PushDataCOM.h"
#include "dal/OrderNumDAL.h"
#include "dal/UserRelationDAL.h"
#include "inc/CmdValue.h"
#include "PushBLL.h"
#include "bll/LukyMoneyBLL.h"
#include "bll/UserDataBLL.h"
#include "cache/UserDataCache.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Send" == psMethod){
        iRet = Send(pJson,pResponse);
    }else if( "CancelChat" == psMethod){
        iRet = CancelChat(pJson,pResponse);
    }
    else if ("MyChat" == psMethod ){
        iRet =MyChat(pJson,pResponse);
    }else if( "UnPackLkMoney" == psMethod){
        iRet = UnPackLkMoney(pJson,pResponse);
    }else if( "QueryLkByOrder" == psMethod){
        iRet = QueryLkByOrder(pJson,pResponse);
    }//
    else if("UpLoadPayRecord" == psMethod){
        iRet = UpLoadPayRecord(pJson,pResponse);
    }
    else {
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
int PrivateChatBLL::Init()
{

    return 0;
}
/*
Description:发送一条私聊
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::Send( const  Json::Value  & pJson, Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  || pJson["iType"].isNull() || pJson["sContent"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);

    if( lUserId == lToUserId ){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }
    if( lToUserId < 50000){
        RetJsonMsg::SetRetMsg(-124,pResponse);
        return -1;
    }

    int iType = tConvert.Json2Int(pJson["iType"]);


    UserRelationDAL userRelDal;
    UserRelationEntity userRelEntity;
    iRet = userRelDal.GetEntity(lUserId, lToUserId,userRelEntity);

    if( iRet == -5){
        //添加陌生人的关系
        userRelEntity.iState = 0;
        userRelEntity.lUserId = lUserId;
        userRelEntity.lToUserId = lToUserId;
        userRelEntity.lAddTime = tConvert.GetCurrentTime();
        userRelEntity.iType = enum_rel_stranger;
        userRelEntity.lUpTime = userRelEntity.lAddTime;
        mUserRelationDal.AddData(userRelEntity);

    }


    if( PowerCheck(lUserId,lToUserId, userRelEntity)){//跳过好友关系的判断

    }else{

        //好友关系的判断

        if( iType != enum_chattype_transfer){
            if( userRelEntity.iType == enum_rel_bbcacka || userRelEntity.iType == enum_rel_ablackb
                    || userRelEntity.iType == enum_rel_adelb_bblacka  ){
                RetJsonMsg::SetRetMsg(-137,pResponse);
                return -1;
            }
            if( userRelEntity.iType == enum_rel_bdela || userRelEntity.iType == enum_rel_adelb
                    || userRelEntity.iType == enum_rel_ablackb_bdela  || userRelEntity.iType == enum_rel_delab ){
                RetJsonMsg::SetRetMsg(-136,pResponse);
                return -1;
            }


        }
    }




    UserDataDAL userDataDal;UserDataEntity userDataEntity;
    userDataDal.GetEntity(tConvert.LongToStr(lUserId),"-1nochange", "-1nochange",userDataEntity);

    PrivateChatEntity entity;

    entity.lUserId = lUserId;
    entity.lToUserId = lToUserId;
    entity.iType = iType;
    entity.sContent = tConvert.Json2String(pJson["sContent"]);
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.iState = 0;
    entity.sKey = tConvert.LongToStr(entity.lUserId) + tConvert.LongToStr(entity.lAddTime/1000);

    if(  userDataEntity.iState == enum_user_state_colse){
        RetJsonMsg::SetRetMsg(-1,pResponse,err1);
        return -1;
    }

    if(enum_chattype_lkmoney == entity.iType || enum_chattype_transfer == entity.iType){//红包,转帐
        string sPassWord;
        if( pJson["sPassWord"].isNull() || pJson["sOrderNo"].isNull()  ){//数据合法性
            RetJsonMsg::SetRetMsg(-15,pResponse);
            return -1;
        }
        sPassWord = tConvert.Json2String(pJson["sPassWord"]);
        string sOrderNo = tConvert.Json2String(pJson["sOrderNo"]);
        int iCoinType = 0;
        if( !pJson["iCoinType"].isNull()){
          iCoinType = tConvert.Json2Int(pJson["iCoinType"]);
        }
        entity.sKey = sOrderNo;

        LongLst lRevLst;
        lRevLst.push_back(lToUserId);

        int iCurrency = 3;//购物币
        if( !pJson["iCurrency"].isNull()){//币种：1现金，2绑定现金，3购物币，4积分
            iCurrency = tConvert.Json2Int(pJson["iCurrency"]);
            if( iCurrency < 1 || iCurrency > 4){
                iCurrency = 3;
            }
        }
        int iTransfer = 1;//1红包 转换成钱包服务的类型
        if( enum_chattype_transfer == entity.iType){
            iTransfer = 2;//2转账 转换成钱包服务的类型
        }

        if( !pJson["bRand"].isNull()){
            bool bRand = tConvert.Json2Bool(pJson["bRand"]);

            if( bRand)
                iTransfer = 1;
            else
                iTransfer = 3;
        }

        //call wallet interface
        LukyMoneyBLL LuckyMoney;
        iRet = LuckyMoney.LukyTransfer(lUserId,lRevLst,sPassWord,1,pJson["sContent"],iCurrency,iCoinType,iTransfer,entity.sKey,pResponse);

        if( iRet == 0 && enum_chattype_transfer == entity.iType){
            //帮对方接收转账
            iRet = LuckyMoney.TransferConfirm(lToUserId,entity.sKey,pResponse);
        }
        if( iRet != 0){
            return -1;
        }

    }

    if( enum_chattype_transfer != entity.iType){
        //数据库,添加一条私聊
        iRet = this->mFriendChatDAL.AddData(entity);
        if ( 0 != iRet && enum_chattype_lkmoney != entity.iType) {
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return -1;
        }
    }

    if( iRet == 0){
        //推送私聊
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_chat;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["sKey"] =  entity.sKey;
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["iType"] = entity.iType;
        jsonPush["Entity"]["sContent"] = entity.sContent;
        jsonPush["Entity"]["sNickName"]  = userDataEntity.sNickName;
        if( userRelEntity.lUserId == lUserId && !userRelEntity.sUserMark.empty()){
            jsonPush["Entity"]["sNickName"]  = userRelEntity.sUserMark;
        }else if(userRelEntity.lToUserId == lUserId && !userRelEntity.sToUserMark.empty()){
            jsonPush["Entity"]["sNickName"]  = userRelEntity.sToUserMark;
        }
        jsonPush["Entity"]["sHeadImage"]  = userDataEntity.sHeadImage;

        Json::Value jShopIds  = UserDataCache::GetInstance()->GetMyShops(entity.lUserId);
        if( jShopIds.size() > 0){
            jsonPush["Entity"]["bShoper"] = true;
        }else{
            jsonPush["Entity"]["bShoper"] = false;
        }

        PushBLL::GetInstance()->Push(jsonPush,Sucess,Fail);


    }


    //返回结果
    pResponse["sKey"] = entity.sKey;
    pResponse["lAddTime"] = tConvert.LongToStr( entity.lAddTime );
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/*
Description:撤销私聊
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::CancelChat(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);

    PrivateChatEntity entity;
    iRet = mFriendChatDAL.GetEntity(sKey,entity);
    if( iRet != 0){

        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }

    if( entity.iType == 5){//不能撤销红包
        RetJsonMsg::SetRetMsg(-135,pResponse);
        return -1;
    }

    if( (tConvert.GetCurrentTime() - entity.lAddTime)/1000000 > 60*2){//超过2分钟了不能被撤回
        RetJsonMsg::SetRetMsg(-131,pResponse);
        return -1;
    }
    iRet = mFriendChatDAL.UpdateState(sKey,333);

    if( iRet == 0){
        //推送撤销私聊(推送给接收者)
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_cancelchat;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["sKey"] =  entity.sKey;
        jsonPush["Entity"]["lUserId"] =  tConvert.LongToStr(entity.lUserId);
        jsonPush["Entity"]["lToUserId"] =  tConvert.LongToStr(entity.lToUserId);


        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

/*
Description:获取我的聊天
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::MyChat( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lAddTime"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = -1;
    long lAddTime = tConvert.Json2Long(pJson["lAddTime"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);

    if( (lUserId == lToUserId) || iCount <=0){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    if( lAddTime == 0){//0表示最新
        lAddTime = 9480400983864318;//未来的时间
    }

    if( !pJson["lToUserId"].isNull()){
        lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    }


    //获取
    PrivateChatList chatList;
    if ( 0 != this->mFriendChatDAL.GetMyChat( lUserId,lToUserId,lAddTime ,iCount,chatList) ){
        appendlog(TTDLogger::LOG_ERROR,"PrivateChatBLL::MyChat mFriendChatDAL.GetMyChat error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    for(int iRow = 0; iRow < chatList.size(); ++iRow ){

        PrivateChatEntity& entity = chatList[chatList.size() - iRow - 1];
        pResponse["list"][iRow]["sKey"] = entity.sKey;
        pResponse["list"][iRow]["lUserId"] = tConvert.LongToStr(entity.lUserId);
        pResponse["list"][iRow]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iRow]["lToUserId"] = tConvert.LongToStr(entity.lToUserId);
        pResponse["list"][iRow]["iType"] = entity.iType;
        pResponse["list"][iRow]["sContent"] = entity.sContent;

        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(entity.lUserId),"-1nochange", "-1nochange",userDataEntity);

        pResponse["list"][iRow]["sNickName"] = userDataEntity.sNickName;
        pResponse["list"][iRow]["sHeadImage"] = userDataEntity.sHeadImage;


        //pResponse["list"][iRow]["ShopIds"]  = UserDataCache::GetInstance()->GetMyShops(entity.lUserId);

    }

    if( chatList.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    mFriendChatDAL.UpdateState(chatList,1);

    //返回成功结果
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:拆红包
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::UnPackLkMoney(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet;
    //数据合法性
    if(  pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);

    PrivateChatEntity entity;
    iRet = mFriendChatDAL.GetEntity(sKey,entity);

    if( iRet != 0 ||  entity.iType !=5 ){//找不到对应的红包
        RetJsonMsg::SetRetMsg(-133,pResponse);
        return -1;
    }

    if( entity.iLkstat == enum_lkstat_packed){//红包已经被拆过了
        RetJsonMsg::SetRetMsg(-132,pResponse);
        return -1;
    }

    if( (tConvert.GetCurrentTime() - entity.lAddTime)/1000000 > 60*60*24 ){//红包已过期
        RetJsonMsg::SetRetMsg(-134,pResponse);
        return -1;
    }

    //call wallet interface
    LukyMoneyBLL LuckyMoney;
    iRet = LuckyMoney.TransferConfirm(entity.lToUserId, sKey,pResponse);
    if( 0 != iRet){
        return -1;
    }


    iRet = mFriendChatDAL.UpdateLkStat(sKey,enum_lkstat_packed);//更改红包转态:拆

    if( iRet == 0){
        //推送拆红包(推送给发送者)
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_unpacklkmoney;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["sKey"] =  entity.sKey;
        jsonPush["Entity"]["lUserId"] =  tConvert.LongToStr(entity.lUserId);
        jsonPush["Entity"]["lToUserId"] =  tConvert.LongToStr(entity.lToUserId);


        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}



/*
Description:红包信息查询 by订单号
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::QueryLkByOrder(const Json::Value& pJson, Json::Value& pResponse)
{

    TConvert tConvert;
    int iRet;
    //数据合法性
    if(  pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);
    int iType = 0;long lAddTime = 0;
    if( !pJson["iType"].isNull() ){
        iType = tConvert.Json2Int( pJson["iType"]);
    }

    if( iType == 0){
        PrivateChatEntity entity;
        mFriendChatDAL.GetEntity(sKey, entity);
        lAddTime = entity.lAddTime;
    }else{
        PublicInfoEntity entity;
        mPublicInfoDAL.GetEntity(sKey, entity);
        lAddTime = entity.lAddTime;
    }

    //call wallet interface
    LukyMoneyBLL LuckyMoney;
    iRet = LuckyMoney.TransferQuery(sKey,pResponse);



    if( pResponse["bTimeOut"].isNull()){
        if( (tConvert.GetCurrentTime() - lAddTime)/1000000 > 60*60*24 ){
            pResponse["bTimeOut"] = true;
        }else{
            pResponse["bTimeOut"] = false;
        }
    }


    //
    if(!pResponse["sReceiveRecord"].isNull()){
        if( pResponse["sReceiveRecord"].size() > 0){
            pResponse["bTimeOut"] = false;
        }
    }

    return iRet;

}


/*
Description:上传付款记录
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int PrivateChatBLL::UpLoadPayRecord(const Json::Value& pJson, Json::Value& pResponse)
{
    TConvert tConvert;
    int iRet;

    if(  pJson["sPayPhone"].isNull() || pJson["lRevPhone"].isNull() || pJson["sPayTime"].isNull()
         || pJson["dMoney"].isNull() || pJson["dCoin"].isNull() || pJson["sPayType"].isNull()
         || pJson["sOrder"].isNull() || pJson["sState"].isNull() || pJson["sRevType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    string sPayPhone = tConvert.Json2String (pJson["sPayPhone"] );
    //string lRevPhone = tConvert.Json2String(pJson["lRevPhone"]);


    UserDataDAL userDataDal;UserDataEntity userDataEntity;
    userDataDal.GetEntity("-1nochange", sPayPhone,"-1nochange",userDataEntity);
    long lPayUserId = userDataEntity.lUserId;

    // userDataDal.GetEntity("-1nochange", lRevPhone,"-1nochange",userDataEntity);
    // long lRevUserId = userDataEntity.lUserId;

    PrivateChatEntity entity;
    entity.iState = 0;
    entity.iType = enum_chattype_payrecord;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lToUserId = lPayUserId;
    entity.lUserId = 29;//天天兑支付系统号
    entity.lUpTime = entity.lAddTime;
    entity.sKey = tConvert.LongToStr(lPayUserId) + tConvert.LongToStr(entity.lAddTime/1000);

    Json::Value jContent = pJson;
    jContent.removeMember("lUserId");
    entity.sContent = jContent.toStyledString();

    iRet = this->mFriendChatDAL.AddData(entity);
    if ( 0 != iRet) {
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return -1;
    }

    if( iRet == 0){
        //推送chat记录
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_chat;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["sKey"] =  entity.sKey;
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["iType"] = entity.iType;

        jsonPush["Entity"]["sContent"] = entity.sContent;


        jsonPush["Entity"]["sNickName"]  = "天天兑支付";
        jsonPush["Entity"]["sHeadImage"]  = "http://tymuserhead.oss-cn-shenzhen.aliyuncs.com/7A95A5D6-A8EB-4EE1-878A-97F86AD076AF_iPicHeight=638.00_iPicWide=638.00_fPicSize=103465.jpg";


        PushBLL::GetInstance()->Push(jsonPush,Sucess,Fail);


    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}

/*
Description:
Input: @lUserId @lToUserId
Output: @userRelEntity
Return: true -跳过好友关系的判断
*/
bool PrivateChatBLL::PowerCheck(long lUserId,long lToUserId,const UserRelationEntity & userRelEntity)
{
    Json::Value jToUserShopIds  = UserDataCache::GetInstance()->GetMyShops(lToUserId);



    Json::Value jUserShopIds  = UserDataCache::GetInstance()->GetMyShops(lUserId);
    if( jUserShopIds.size() == 0 && jToUserShopIds.size() == 0 ){
        return false;
    }



    if( lUserId == userRelEntity.lUserId){
        if( (userRelEntity.iType == enum_rel_bbcacka) ){
            return false;
        }
    }else if( lUserId == userRelEntity.lToUserId){
        if( (userRelEntity.iType == enum_rel_ablackb) ){
            return false;
        }
    }


    return true;

}

//设置默认值
void PrivateChatBLL::SetDefaultValue( PrivateChatEntity& pEntity )
{
    pEntity.lToUserId = -1;
    pEntity.iType = -1;
    pEntity.sContent = "-1nochange";
}

/*
Description:推送成功回调
Input:  @value 回调数据
Output: null
Return: null
*/
void PrivateChatBLL::Sucess(const Json::Value & value)
{
    PrivateChatDAL FriendChatDAL;
    TConvert tConvert;

    if(tConvert.Json2Int( value["iCmd"] ) == enum_cmd_chat){

        // FriendChatDAL.UpdateState(tConvert.Json2String( value["Entity"]["sKey"]),1);

    }

    return;
}
/*
Description:推送失败回调
Input:  @value 回调数据
Output: null
Return: null
*/
void PrivateChatBLL::Fail(const Json::Value & value)
{
    return;
}
