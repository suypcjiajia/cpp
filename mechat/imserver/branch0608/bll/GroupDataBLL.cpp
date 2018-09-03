/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群资料业务
 *Others:
*/

#include "GroupDataBLL.h"
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
#include "CmdValue.h"
#include "dal/UserDataDAL.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Creat" == psMethod){
        iRet = Creat(pJson,pResponse);
    }else if("Set" == psMethod){
        iRet = Set(pJson,pResponse);
    }else if("GetInfoById" == psMethod){
        iRet = GetInfoById(pJson,pResponse);
    }else if("SetHead" == psMethod){
        iRet = SetHead(pJson,pResponse);
    }else if("GetGroupIds" == psMethod){
        iRet = GetGroupIds(pJson,pResponse);
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
int GroupDataBLL::Init()
{

    return 0;
}

/*
Description:创建一个群
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::Creat( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["sUserLst"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    if( lUserId < 50000){
        RetJsonMsg::SetRetMsg(-2,pResponse);
        return -1;
    }

    GroupDataEntity groupDataEntity;

    vector<string> userVct;

    Json::Value jsonUserLst = pJson["sUserLst"];
    for( int i = 0 ; i <  jsonUserLst.size() ; i++ ){

        userVct.push_back( tConvert.Json2String(jsonUserLst[i]));

    }


    userVct.insert( userVct.begin(), tConvert.LongToStr(lUserId));
    if( userVct.size()  < 3){//三人才能成群
        RetJsonMsg::SetRetMsg(-142,pResponse);
        return -1;
    }


    groupDataEntity.lAddTime = tConvert.GetCurrentTime();
    groupDataEntity.lUpTime = groupDataEntity.lAddTime;
    groupDataEntity.lMasterId = lUserId;
    groupDataEntity.sGroupName = "多人聊天群组";
    groupDataEntity.lGroupId = mGroupDataDAL.GetMaxGroupId() + 1;

    iRet = mGroupDataDAL.AddData(groupDataEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    UserDataDAL uDataDal; UserDataEntity uDataEntity;

    uDataDal.GetEntity(tConvert.LongToStr(lUserId), "-1nochange", "-1nochange",uDataEntity);
    string sInviteNick = uDataEntity.sNickName;

    GroupMemberBLL gMemberBll;
    for( int i = 0; i < userVct.size(); i++){
        //want to do 这里应该改为批量插入
        iRet = gMemberBll.AddMember(groupDataEntity.lGroupId,tConvert.StrToLong(userVct[i]),enum_memberjoin_friend);

        if( iRet == 0){

            if(  tConvert.LongToStr(lUserId) ==  userVct[i] ){
                continue;
            }

            uDataDal.GetEntity(userVct[i], "-1nochange", "-1nochange",uDataEntity);

            //推送入群消息
            gMemberBll.PushJoinInCmd( tConvert.StrToLong(userVct[i]), tConvert.StrToLong(userVct[i]),uDataEntity.sNickName,
                                      groupDataEntity.lGroupId,groupDataEntity.lMasterId,groupDataEntity.sGroupName,sInviteNick);
        }
    }

    pResponse["lGroupId"] = tConvert.LongToStr(groupDataEntity.lGroupId);


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:查询群资料
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::GetInfoById( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;
    //数据合法性
    if( pJson["lGroupId"].isNull()  || pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    GroupDataEntity gDataEntity;
    iRet = mGroupDataDAL.GetEntity(lGroupId,gDataEntity);

    if( 0 == iRet){

        pResponse["entity"]["lGroupId"] = tConvert.LongToStr( gDataEntity.lGroupId );
        pResponse["entity"]["lMasterId"] = tConvert.LongToStr(gDataEntity.lMasterId);
        pResponse["entity"]["sGroupName"] = gDataEntity.sGroupName;
        GroupMemberBLL gMemberbll;
        pResponse["entity"]["sGroupHead"] = gMemberbll.MakeGroupHead(gDataEntity.lGroupId);
        pResponse["sMixHead"] =  gDataEntity.sGroupHead;

        GroupMemberDAL gMemDal;GroupMemberEntity gMemEntity;
        gMemDal.GetEntity(lGroupId, lUserId,gMemEntity);

        pResponse["entity"]["sNickName"] = gMemEntity.sNickName;
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:遍历群（后台维护使用）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::GetGroupIds( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["iIndex"].isNull()  || pJson["iCount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iIndex  = tConvert.Json2Int(pJson["iIndex"]);
    int iCount  = tConvert.Json2Int(pJson["iCount"]);

    longs lGroupIds;
    mGroupDataDAL.GetGroupIds( lGroupIds, iIndex, iCount);

    int i = 0;
    for( longs::iterator it = lGroupIds.begin() ; it != lGroupIds.end(); it++){
        pResponse["lGroupIds"][i++] = tConvert.LongToStr( *it);

    }
    if( lGroupIds.size() == 0){
        pResponse["lGroupIds"] ;
        pResponse["lGroupIds"].resize(0);
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:更改群信息
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::Set( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;int iRet = -1;
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lGroupId"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
    string sGroupName ;
    if( ! pJson["sGroupName"].isNull()){
        sGroupName = tConvert.Json2String(pJson["sGroupName"]);
    }


    GroupDataEntity gDataEntity;
    mGroupDataDAL.GetEntity( lGroupId,gDataEntity);

    if( gDataEntity.lGroupId != lGroupId){
        RetJsonMsg::SetRetMsg(-5,pResponse);
        return -5;
    }


    GroupMemberDAL gMemberDal;

    GroupMemberEntity gMemberInfo;
    gMemberInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lUserId,gMemberInfo);


    if( gMemberInfo.iType != enum_membertype_member){
        RetJsonMsg::SetRetMsg(-143,pResponse);
        return -1;
    }


    UserDataDAL uDataDal; UserDataEntity uDataEntity;
    uDataDal.GetEntity(tConvert.LongToStr(lUserId), "-1nochange", "-1nochange",uDataEntity);


    if( ! sGroupName.empty()){
       iRet =  mGroupDataDAL.UpGroupName(lGroupId,sGroupName);
    }
    if( 0 == iRet){
        GroupMemberList gMemberLst;GroupMemberDAL gMemberDal;
        gMemberDal.GetRealMemberById(lGroupId,gMemberLst);
        GroupMemberList::iterator it = gMemberLst.begin();
        //向其他成员推送变更消息
        for( ; it != gMemberLst.end(); it++){
            GroupMemberEntity & gMemberEntity = *it;GroupMemberBLL gMemberBll;
            if( gMemberEntity.lMemberId == lUserId){//不用推送给本人
                continue;
            }
            gMemberBll.PushSetGroupCmd(gMemberEntity.lMemberId,lUserId,uDataEntity.sNickName,lGroupId, sGroupName);
        }
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

/*
Description:群头像（合成头像）
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupDataBLL::SetHead( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;int iRet = -1;
    //数据合法性
    if(  pJson["lGroupId"].isNull() || pJson["sGroupHead"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);

    string sGroupHead = tConvert.Json2String(pJson["sGroupHead"]);

    iRet =  mGroupDataDAL.UpGroupHead(lGroupId,sGroupHead);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


