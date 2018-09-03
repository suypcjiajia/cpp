/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装调系统通知业务接口
 *Others:
*/

#include "NoticeBLL.h"
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
#include "bll/GroupMemberBLL.h"
#include "bll/PushBLL.h"
#include "CmdValue.h"
#include "dal/UserDataDAL.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int NoticeBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Send" == psMethod){
        iRet = Send(pJson,pResponse);
    }else if("GetMyNotice" == psMethod){
        iRet = GetMyNotice(pJson,pResponse);
    }else if("Touch" == psMethod){
        iRet = Touch(pJson,pResponse);
    }else if("RedPoin" == psMethod){
        iRet = RedPoin(pJson,pResponse);
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
int NoticeBLL::Init()
{

    return 0;
}
/*
Description:发送一条通知
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int NoticeBLL::Send( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lSendId"].isNull() || pJson["iTarget"].isNull()  || pJson["sContent"].isNull() ||
            pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    TConvert tConvert;
    int iRet = -1;
    NoticeEntity nEntity;
    long lSendId = tConvert.Json2Long(pJson["lSendId"]);
    EnumTarget iTarget = (EnumTarget)tConvert.Json2Long(pJson["iTarget"]);
    string sContent = tConvert.Json2String(pJson["sContent"]);
    long iType = tConvert.Json2Int(pJson["iType"]);
    Json::Value jRevIds = pJson["sRevIds"];


    int iSubType = 0;
    if(iType == enum_notice_xiaoxi){


        int iSubType = tConvert.Json2Int( pJson["sContent"]["iSubType"] );//iSubType
        if( iSubType >=20)
            iSubType = 0;
    }


    if(iTarget == enum_broadcast){
        nEntity.sRevIds = "";
    }else if(iTarget == enum_multicast){
        if( pJson["sRevIds"].isNull()){
            RetJsonMsg::SetRetMsg(-15,pResponse);
            return -1;
        }


        if( jRevIds.size() > 0){
            string v = tConvert.Json2String( jRevIds[0] );
            int i = v.size();
            if( v.length() == 11){//手机号，要转成天天兑号

                for( int i = 0; i < jRevIds.size() ; i++){
                    UserDataDAL userDataDal;UserDataEntity userDataEntity;
                    userDataDal.GetEntity("-1nochange", tConvert.Json2String(jRevIds[i]),"-1nochange",userDataEntity);
                    jRevIds[i] = tConvert.LongToStr(userDataEntity.lUserId);

                }
            }
        }

        nEntity.sRevIds = jRevIds.toStyledString();

    }else{

    }




    long lCurTime = tConvert.GetCurrentTime();
    nEntity.sKey = tConvert.LongToStr(lSendId) + tConvert.LongToStr(lCurTime/1000);
    nEntity.lAddTime = lCurTime;
    nEntity.lUpTime = lCurTime;
    nEntity.lSendId = lSendId;
    nEntity.sContent = sContent;
    nEntity.iTarget = iTarget;
    nEntity.iType  =iType;
    nEntity.sReaded = "";



    iRet = mNoticeDAL.AddData(nEntity);


    if( 0 == iRet ){


        //        if( iType == enum_notice_xiaoxi){
        //            int iSubType =tConvert.Json2Int ( pJson["sContent"]["iSubType"] );
        //            if( iSubType == 8){
        //                Json::Value jTmp = pJson["sContent"];
        //                jTmp.removeMember("product_mark_id");
        //                nEntity.sContent = jTmp.toStyledString();
        //            }
        //        }

        //推送系统通知

        PushNotice(jRevIds,nEntity,mMsgColor[iSubType]);


    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


/*
Description:获取未读通知
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int NoticeBLL::GetMyNotice( const Json::Value& pJson, Json::Value& pResponse )
{

    //数据合法性
    if( pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    int iType = -1;
    if( !pJson["iType"].isNull()){
        iType = tConvert.Json2Int(pJson["iType"]);
    }

    if( lUserId <= 50000){
        RetJsonMsg::SetRetMsg(0,pResponse);
        pResponse["list"] ;
        pResponse["list"].resize(0);
        return 0;
    }

    NoticeList nLst;
    mNoticeDAL.GetMyNotice(lUserId,iType,nLst);


    NoticeList::iterator it = nLst.begin();
    for( int i = 0; it != nLst.end() ; it++){
        NoticeEntity & entity = *it;
        pResponse["list"][i ]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
        pResponse["list"][i ]["lSendId"] = tConvert.LongToStr( entity.lSendId);
        tConvert.Str2Json(entity.sContent,pResponse["list"][i ]["sContent"]);


        if( entity.iType == enum_notice_gonggao){
            if( pResponse["list"][i ]["sContent"]["iJumpType"].isNull() ){
                pResponse["list"][i ]["sContent"]["iJumpType"] = 0;
            }
        }

        pResponse["list"][i ]["sKey"] =  entity.sKey;
        pResponse["list"][i ]["iType"] =  entity.iType;

        int iSubType = 0;
        if( entity.iType == enum_notice_xiaoxi){
            iSubType = tConvert.Json2Int( pResponse["list"][i ]["sContent"]["iSubType"] );
            if( iSubType >=20)
                iSubType = 0;

        }
        pResponse["list"][i ]["sColor"] = mMsgColor[iSubType];

        i++;

    }

    if( nLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    //mNoticeDAL.UpdateReaded(lUserId, nLst);


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}


/*
Description:记录已读的时间点
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int NoticeBLL::Touch( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["iType"].isNull() || pJson["lLastTime"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    int iType = tConvert.Json2Int(pJson["iType"]);
    long lLastTime = tConvert.Json2Long(pJson["lLastTime"]);
    mRedPointDAL.Replace(lUserId,lLastTime,iType,"");

    //只是记录一下，永远返回成功
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}



/*
Description:获取红点情况
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int NoticeBLL::RedPoin( const Json::Value& pJson, Json::Value& pResponse )
{
    if( pJson["lUserId"].isNull() || pJson["iType"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;int iRet = -1;

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    int iType = tConvert.Json2Int(pJson["iType"]);

    long lLastTime = mRedPointDAL.GetLastTime(lUserId,iType,"");
    if( lLastTime < 0){
        lLastTime = 0;
    }

    if( iType == enum_red_sysxiaoxi){
        int iCount = 0;


        iRet = mNoticeDAL.GetCountByLastTime(lUserId,lLastTime,iCount);

        NoticeEntity entity;
        mNoticeDAL.GetLastRecord(lUserId,entity);

        Json::Value jContent;
        tConvert.Str2Json(entity.sContent,jContent);
        pResponse["sContent"] = jContent["sTitle"];
        pResponse["lAddTime"] = tConvert.LongToStr( entity.lAddTime );

        if( iCount <= 0){
            pResponse["bNew"] = false;
        }else{
            pResponse["bNew"] = true;
        }
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


/*
Description:推送系统通知
Input:  @jRevIds 推送的目标用户，多人
Output: @nEntity
Return: 0 -success
*/
int NoticeBLL::PushNotice( const Json::Value & jRevIds ,const NoticeEntity&  nEntity,string sColor )
{
    TConvert tConvert;

    //    Json::Value  jTram;

    //    if(!jRevIds.isArray() ){


    //        string sRev = jRevIds.toStyledString();
    //        sRev = sRev.substr(1);
    //        sRev = sRev.substr(0,sRev.length() -1);
    //        vector<string> vRev;
    //        tConvert.Split(sRev,",",vRev);

    //        for(int i = 0; i < vRev.size(); i++){
    //            jTram[i] = sRev;
    //        }

    //    }else{
    //        jTram = jRevIds;
    //    }


    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_notice;

    jsonPush["jUserIds"] = jRevIds;

    if( jRevIds.size() == 0)
        jsonPush["jUserIds"].resize(0);
    jsonPush["Entity"]["iTarget"] =   nEntity.iTarget;
    jsonPush["Entity"]["lAddTime"] =  tConvert.LongToStr(nEntity.lAddTime);
    jsonPush["Entity"]["lSendId"] =   tConvert.LongToStr( nEntity.lSendId );
    tConvert.Str2Json( nEntity.sContent , jsonPush["Entity"]["sContent"]);
    jsonPush["Entity"]["sKey"] =   nEntity.sKey;
    jsonPush["Entity"]["iType"] =   nEntity.iType;
    jsonPush["Entity"]["sColor"] = sColor;


    // PushDataCOM pushDataCOM;
    // pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );

    PushBLL::GetInstance()->Push(jsonPush,Sucess,Fail);
}


/*
Description:推送成功回调
Input:  @value 回调数据
Output: null
Return: null
*/
void NoticeBLL::Sucess(const Json::Value & value)
{


    return;
}
/*
Description:推送失败回调
Input:  @value 回调数据
Output: null
Return: null
*/
void NoticeBLL::Fail(const Json::Value & value)
{
    return;
}
