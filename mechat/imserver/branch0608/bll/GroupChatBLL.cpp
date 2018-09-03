/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群聊业务
 *Others:
*/


#include "GroupChatBLL.h"
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
#include "dal/UserSetDAL.h"
#include "dal/GroupMemberDAL.h"
#include "dal/UserDataDAL.h"
#include "bll/GroupMemberBLL.h"
#include "bll/LukyMoneyBLL.h"
#include "PrivateChatEntity.h"
#include "bll/PushBLL.h"
#include "CmdValue.h"

using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupChatBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Send" == psMethod){
        iRet = Send(pJson,pResponse);
    }else if( "MyChat" == psMethod){
        iRet = MyChat(pJson,pResponse);
    }else if( "UnPackLkMoney" == psMethod){
        iRet = UnPackLkMoney(pJson,pResponse);
    }else if( "QueryLkByOrder" == psMethod){
        iRet = QueryLkByOrder(pJson,pResponse);
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
int GroupChatBLL::Init()
{

    return 0;
}


/*
Description:发送一条群聊
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupChatBLL::Send( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lGroupId"].isNull() || pJson["iType"].isNull() || pJson["sContent"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
    long iType = tConvert.Json2Int(pJson["iType"]);
    string sContent = tConvert.Json2String(pJson["sContent"]);
    if( lUserId < 50000){
        RetJsonMsg::SetRetMsg(-2,pResponse);
        return -1;
    }

    GroupMemberDAL gMemberDal;

    GroupMemberEntity gSendInfo;
    gSendInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lUserId,gSendInfo);
    if( gSendInfo.iType != enum_membertype_member){
        RetJsonMsg::SetRetMsg(-143,pResponse);
        return -1;
    }


    GroupChatEntity gChatEntity;
    long lCurTime = tConvert.GetCurrentTime();
    gChatEntity.sKey = tConvert.LongToStr(lUserId) + tConvert.LongToStr(lCurTime/1000);
    gChatEntity.iLkstat = 0;
    gChatEntity.iType = iType;
    gChatEntity.lAddTime = lCurTime;
    gChatEntity.lUpTime = lCurTime;
    gChatEntity.lSendId = lUserId;
    gChatEntity.lGroupId = lGroupId;
    gChatEntity.sContent = sContent;



    GroupMemberList gMemberLst;
    gMemberDal.GetRealMemberById(lGroupId,gMemberLst);

    if(enum_chattype_lkmoney == gChatEntity.iType ){//红包
        string sPassWord;
        if( pJson["sPassWord"].isNull() || pJson["sOrderNo"].isNull()){//数据合法性
            RetJsonMsg::SetRetMsg(-15,pResponse);
            return -1;
        }
        sPassWord = tConvert.Json2String(pJson["sPassWord"]);
        string sOrderNo = tConvert.Json2String(pJson["sOrderNo"]);
        int iCoinType = 0;
        if( !pJson["iCoinType"].isNull()){
          iCoinType = tConvert.Json2Int(pJson["iCoinType"]);
        }
        gChatEntity.sKey = sOrderNo;


        int iCurrency = 3;//购物币
        if( !pJson["iCurrency"].isNull()){//币种：1现金，2绑定现金，3购物币，4积分
            iCurrency = tConvert.Json2Int(pJson["iCurrency"]);
            if( iCurrency < 1 || iCurrency > 4){
                iCurrency = 3;
            }
        }
        int iTransfer = 1;//1红包 转换成钱包服务的类型
        if( !pJson["bRand"].isNull()){
            bool bRand = tConvert.Json2Bool(pJson["bRand"]);

            if( bRand)
                iTransfer = 1;
            else
                iTransfer = 3;
        }

        std::vector<std::string>  resultVector;
        tConvert.Split(  sContent , ",",resultVector );


        if( resultVector.size() != 2   ){//数据合法性
            RetJsonMsg::SetRetMsg(-2,pResponse);
            return -1;
        }

        double dMoney = atof(resultVector[0].c_str());
        int iMun = atoi(resultVector[1].c_str());

        if(  dMoney <= 0 || iMun <= 0  ){//数据合法性
            RetJsonMsg::SetRetMsg(-2,pResponse);
            return -1;
        }

        LongLst lRevUserIdLst;
        for( GroupMemberList::const_iterator it = gMemberLst.begin(); it != gMemberLst.end(); it++)
        {
            const GroupMemberEntity & en = *it;
            lRevUserIdLst.push_back(en.lMemberId);
        }

        //call wallet interface
        LukyMoneyBLL LuckyMoney;
        iRet = LuckyMoney.LukyTransfer(lUserId,lRevUserIdLst,sPassWord,iMun,dMoney,iCurrency,iCoinType,iTransfer,gChatEntity.sKey,pResponse);

        if( iRet != 0){
            return -1;
        }

    }

    iRet = mGroupDataDAL.AddData(gChatEntity);
    if( 0 != iRet && enum_chattype_lkmoney != gChatEntity.iType){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    if( 0 == iRet ){
        GroupMemberEntity gMemberEntity;
        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(lUserId),"-1nochange", "-1nochange",userDataEntity);
        gMemberDal.GetEntity(lGroupId,lUserId,gMemberEntity);

        GroupMemberList::iterator it = gMemberLst.begin();
        //向其他成员推送一条群聊天
        for( ; it != gMemberLst.end(); it++){
            GroupMemberEntity & gMemberEntity = *it;
            if( gMemberEntity.lMemberId == lUserId){
                continue;
            }
            //appendlog(TTDLogger::LOG_DEBUG, "xxxxxx:%ld", gMemberEntity.lMemberId);
            PushNewChatCmd(  gMemberEntity.lMemberId, lUserId, gSendInfo.sNickName, userDataEntity.sHeadImage, lGroupId,gChatEntity.sKey, lCurTime,sContent,iType);
        }
    }

    pResponse["sKey"] = gChatEntity.sKey;
    pResponse["lAddTime"] =  tConvert.LongToStr(gChatEntity.lAddTime);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/*
Description:获取未读群聊
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupChatBLL::MyChat( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["lUserId"].isNull()  || pJson["lAddTime"].isNull() || pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    pResponse["lUserId"] = pJson["lUserId"];

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lAddTime = tConvert.Json2Long(pJson["lAddTime"]);
    int iCount = tConvert.Json2Int(pJson["iCount"]);


    if( lAddTime == 0){//0表示最新
        lAddTime = 9480400983864318;//未来的时间
    }

    GroupMemberDAL gMemberDal;GroupMemberList gMemberLst;

    if( !pJson["lGroupId"].isNull()){
        long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
        GroupMemberEntity gMemEntity;
        gMemberDal.GetEntity(lGroupId,lUserId,gMemEntity);
        gMemberLst.push_back(gMemEntity);

    }else{
        gMemberDal.GetMyGroup(lUserId,gMemberLst);
    }

    GroupChatList chatList;
    if ( 0 != mGroupDataDAL.GetMyChat( lUserId,gMemberLst,lAddTime ,iCount,chatList) ){
        appendlog(TTDLogger::LOG_ERROR,"GroupChatBLL::MyChat GroupDataDAL.GetMyChat error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    for(int iRow = 0; iRow < chatList.size(); ++iRow ){

        GroupChatEntity& entity = chatList[chatList.size() - iRow - 1];
        pResponse["list"][iRow]["sKey"] = entity.sKey;
        pResponse["list"][iRow]["lSendId"] = tConvert.LongToStr(entity.lSendId);
        pResponse["list"][iRow]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iRow]["lGroupId"] = tConvert.LongToStr(entity.lGroupId);
        pResponse["list"][iRow]["iType"] = entity.iType;
        pResponse["list"][iRow]["sContent"] = entity.sContent;

        GroupMemberDAL gMemberDal;GroupMemberEntity gMemberEntity;
        gMemberDal.GetEntity(entity.lGroupId,entity.lSendId,gMemberEntity);
        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(entity.lSendId),"-1nochange", "-1nochange",userDataEntity);

        pResponse["list"][iRow]["sNickName"] = gMemberEntity.sNickName;
        pResponse["list"][iRow]["sHeadImage"] = userDataEntity.sHeadImage;
    }

    if( chatList.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    mGroupDataDAL.UpdateReaded( lUserId ,chatList);

    //返回成功结果

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}



/*
Description:抢红包
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupChatBLL::UnPackLkMoney(const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet;
    //数据合法性
    if( pJson["lUserId"].isNull() ||  pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);
    long lUserId = tConvert.Json2Long( pJson["lUserId"]);

    GroupChatEntity entity;
    iRet = mGroupDataDAL.GetEntity(sKey,entity);

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
    if( 0 != iRet){
        return -1;
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
int GroupChatBLL::QueryLkByOrder(const Json::Value& pJson, Json::Value& pResponse)
{

    TConvert tConvert;
    int iRet;
    //数据合法性
    if(  pJson["sKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    string sKey = tConvert.Json2String(pJson["sKey"]);

    GroupChatEntity entity;
    mGroupDataDAL.GetEntity(sKey, entity);

    //call wallet interface
    LukyMoneyBLL LuckyMoney;
    iRet = LuckyMoney.TransferQuery(sKey,pResponse);

    if( (tConvert.GetCurrentTime() - entity.lAddTime)/1000000 > 60*60*24 ){
        pResponse["bTimeOut"] = true;
    }else{
        pResponse["bTimeOut"] = false;
    }

    return iRet;

}



//推送一条群聊天
int GroupChatBLL::PushNewChatCmd( long lUserId, long lSendId,const string sNickName, const string sHeadImage,long lGroupId,const string & sKey, long lAddTime,
                                  const string & sContent, int iType)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_chat;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["lSendId"] =  tConvert.LongToStr( lSendId);
    jsonPush["Entity"]["sKey"] =  sKey;
    jsonPush["Entity"]["sContent"] =  sContent;
    jsonPush["Entity"]["iType"] =  iType;
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);
    jsonPush["Entity"]["lAddTime"] =  tConvert.LongToStr( lAddTime);
    jsonPush["Entity"]["sNickName"] =  sNickName;
    jsonPush["Entity"]["sHeadImage"] =  sHeadImage;

   // PushDataCOM pushDataCOM;
    //pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );

    PushBLL::GetInstance()->Push(jsonPush,Sucess,Fail);
}



/*
Description:推送成功回调
Input:  @value 回调数据
Output: null
Return: null
*/
void GroupChatBLL::Sucess(const Json::Value & value)
{

    return;
}
/*
Description:推送失败回调
Input:  @value 回调数据
Output: null
Return: null
*/
void GroupChatBLL::Fail(const Json::Value & value)
{
    return;
}


