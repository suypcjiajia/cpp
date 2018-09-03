/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群成员业务
 *Others:
*/


#include "GroupMemberBLL.h"
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
#include "bll/GroupDataBLL.h"
#include "dal/GroupDataDAL.h"
#include "dal/UserDataDAL.h"
#include "CmdValue.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "MyGroup" == psMethod){
        iRet = MyGroup(pJson,pResponse);
    }else if("JoinIn" == psMethod){
        iRet = JoinIn(pJson,pResponse);
    }else if("JoinOut" == psMethod){
        iRet = JoinOut(pJson,pResponse);
    }else if("KickOut" == psMethod){
        iRet = KickOut(pJson,pResponse);
    }else if("MemberById" == psMethod){
        iRet = MemberById(pJson,pResponse);
    }else if("SetNickName" == psMethod){
        iRet = SetNickName(pJson,pResponse);
    }else if("MemberBylotsId" == psMethod){
        iRet = MemberBylotsId(pJson,pResponse);
    }else if( "ModifyMySetting" == psMethod){
        iRet = ModifyMySetting(pJson,pResponse);
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
int GroupMemberBLL::Init()
{

    return 0;
}


/*
Description:成为群的成员 加入方式 二维码或好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::JoinIn( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;
    //数据合法性
    if(  pJson["lGroupId"].isNull() || pJson["lInviteId"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);

    long lInviteId = -1;string sInviteNick;
    lInviteId = tConvert.Json2Long(pJson["lInviteId"]);


    GroupMemberDAL gMemberDal;
    GroupMemberEntity gMemberInfo;
    gMemberInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lInviteId,gMemberInfo);
    if( gMemberInfo.iType != enum_membertype_member){
        RetJsonMsg::SetRetMsg(-144,pResponse);
        return -1;
    }


    vector<long> lMemberLst;


    EnumMemberJoin iJoin = enum_memberjoin_unknow;
    if(! pJson["lMemberId"].isNull() ){
        iJoin = enum_memberjoin_qrcode;
        lMemberLst.push_back(tConvert.Json2Long(pJson["lMemberId"]));

    }else if(! pJson["lMemberIdLst"].isNull() ){



        iJoin = enum_memberjoin_friend;
        Json::Value jsonMemberIdLst = pJson["lMemberIdLst"];

        for( int i = 0 ; i <  jsonMemberIdLst.size() ; i++ ){

            lMemberLst.push_back( tConvert.Json2Long(jsonMemberIdLst[i]));
        }
    }else {
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }




    GroupMemberEntity gInviteinfo;
    mGroupMemberDAL.GetEntity(lGroupId,lInviteId,gInviteinfo);
    sInviteNick = gInviteinfo.sNickName;


    UserDataDAL uDataDal; UserDataEntity uDataEntity;
    GroupDataDAL gDataDal; GroupDataEntity gDataEntity;
    gDataDal.GetEntity(lGroupId,gDataEntity);


    for( int i = 0 ; i < lMemberLst.size() ; i++){
        long lMemberId = lMemberLst[i];
        GroupMemberEntity tmpEntity;
        iRet = mGroupMemberDAL.GetEntity(lGroupId,lMemberId,tmpEntity);
        if( (tmpEntity.lMemberId == lMemberId) && (tmpEntity.iType == enum_membertype_member)){//已经是群的成员
            continue;
        }

        if( (0 == iRet ) && ( tmpEntity.iType == enum_membertype_none ) ){
            iRet = mGroupMemberDAL.UpDataType( lGroupId, lMemberId, enum_membertype_member);
        }else{
            iRet = AddMember(lGroupId,lMemberId,iJoin);
        }

        if( 0 == iRet){


            uDataDal.GetEntity(tConvert.LongToStr(lMemberId), "-1nochange", "-1nochange",uDataEntity);

            GroupMemberList gMemberLst;
            mGroupMemberDAL.GetRealMemberById(lGroupId,gMemberLst);
            GroupMemberList::iterator it = gMemberLst.begin();
            //向成员推送lMemberId入群消息
            for( ; it != gMemberLst.end(); it++){
                GroupMemberEntity & gMemberEntity = *it;
                if( (iJoin == enum_memberjoin_qrcode) && (gMemberEntity.lMemberId == lMemberId) ){//如果是主动入群(二维码)，就不用推送给自己
                    continue;
                }
                if( (enum_memberjoin_friend ==  iJoin) && (gMemberEntity.lMemberId == lInviteId)){//如果是好友拉入群，就不用推送给邀请者
                    continue;
                }
                PushJoinInCmd(gMemberEntity.lMemberId,lMemberId,
                              uDataEntity.sNickName,lGroupId,gDataEntity.lMasterId,gDataEntity.sGroupName,sInviteNick);
            }




        }
    }


    if( iRet == 0){
        pResponse["sGroupName"] = gDataEntity.sGroupName;
        pResponse["sMixHead"] =  gDataEntity.sGroupHead;
        pResponse["sGroupHead"] = MakeGroupHead(gDataEntity.lGroupId);
        pResponse["lMasterId"] = tConvert.LongToStr( gDataEntity.lMasterId);
        pResponse["lGroupId"] = tConvert.LongToStr( gDataEntity.lGroupId);
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:主动退出群
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::JoinOut( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull()  || pJson["lGroupId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);

    GroupMemberDAL gMemberDal;

    GroupMemberEntity gMemberInfo;
    gMemberInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lUserId,gMemberInfo);
    if( gMemberInfo.iType != enum_membertype_member){
        RetJsonMsg::SetRetMsg(-143,pResponse);
        return -1;
    }

    iRet = mGroupMemberDAL.UpDataType(lGroupId,lUserId,enum_membertype_none);

    long lTmpTime = 9480400983864318; long lTmpMemberId = 0;
    if( 0 == iRet ){

        UserDataDAL uDataDal; UserDataEntity uDataEntity;
        uDataDal.GetEntity(tConvert.LongToStr(lUserId), "-1nochange", "-1nochange",uDataEntity);

        GroupMemberList gMemberLst;
        mGroupMemberDAL.GetRealMemberById(lGroupId,gMemberLst);
        GroupMemberList::iterator it = gMemberLst.begin();
        //向其他成员推送用户退群消息
        for( ; it != gMemberLst.end(); it++){
            GroupMemberEntity & gMemberEntity = *it;

            if( gMemberEntity.lMemberId == lUserId){
                continue;
            }
            PushJoinOutCmd(gMemberEntity.lMemberId, lUserId,uDataEntity.sNickName,lGroupId);


            if( gMemberEntity.lAddTime < lTmpTime){
                lTmpTime = gMemberEntity.lAddTime;
                lTmpMemberId = gMemberEntity.lMemberId;//群主退群后，新群主由加入群的成员第一次序来担任
            }
        }
    }

    GroupDataDAL gDataDal;GroupDataEntity gDataEntity;
    gDataDal.GetEntity(lGroupId,gDataEntity);
    if( gDataEntity.lMasterId == lUserId){//啊，群主退群

        //更换群主
        gDataDal.UpMaster(lGroupId,lTmpMemberId);
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:群主踢人
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::KickOut( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull()  || pJson["lGroupId"].isNull() || pJson["lMemberIds"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);


    GroupDataDAL gDataDal; GroupDataEntity gDataEntity;
    gDataDal.GetEntity(lGroupId,gDataEntity);
    if( gDataEntity.lMasterId != lUserId){//不是群主，没有权限
        RetJsonMsg::SetRetMsg(-141,pResponse);
        return -141;
    }

    GroupMemberList gMemberLst;
    mGroupMemberDAL.GetRealMemberById(lGroupId,gMemberLst);

    Json::Value jMemberIds = pJson["lMemberIds"];
    for( int i = 0; i < jMemberIds.size() ; i++){
        long lMemberId = tConvert.Json2Long(jMemberIds[i]);

        iRet = mGroupMemberDAL.UpDataType(lGroupId,lMemberId,enum_membertype_none);

        if( 0 == iRet ){

            UserDataDAL userDataDal;UserDataEntity userDataEntity;
            userDataDal.GetEntity(tConvert.LongToStr(lMemberId),"-1nochange", "-1nochange",userDataEntity);


            GroupMemberList::iterator it = gMemberLst.begin();
            //向其他成员推送群主踢人消息
            for( ; it != gMemberLst.end(); it++){
                GroupMemberEntity & gMemberEntity = *it;
                if( gMemberEntity.lMemberId == lUserId){
                    continue;
                }
                PushKickOutCmd(gMemberEntity.lMemberId, lMemberId, userDataEntity.sNickName,lGroupId);
            }
        }
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:获取我加入的群
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::MyGroup( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull()  || pJson["lUpTime"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lUpTime = tConvert.Json2Long(pJson["lUpTime"]);
    if( lUserId < 50000){
        RetJsonMsg::SetRetMsg(-2,pResponse);
        return -1;
    }
    GroupMemberList groupMemberLst;
    mGroupMemberDAL.GetMyGroup(lUserId,lUpTime,groupMemberLst);

    long lRetUpTime = lUpTime;
    GroupMemberList::iterator it = groupMemberLst.begin();
    for( int i = 0; it != groupMemberLst.end() ; it++){
        GroupMemberEntity & entity = *it;

        GroupDataDAL groupDataDal; GroupDataEntity groupDataEntity;
        groupDataDal.GetEntity(entity.lGroupId,groupDataEntity);

        pResponse["list"][i ]["lGroupId"] = tConvert.LongToStr( entity.lGroupId);
        pResponse["list"][i ]["lMasterId"] = tConvert.LongToStr( groupDataEntity.lMasterId);
        pResponse["list"][i ]["sNickName"] =  entity.sNickName;
        pResponse["list"][i ]["sGroupName"] =  groupDataEntity.sGroupName;
        pResponse["list"][i ]["sMixHead"] =  groupDataEntity.sGroupHead;
        pResponse["list"][i ]["sGroupHead"] =  MakeGroupHead(entity.lGroupId);
        pResponse["list"][i ]["iType"] =  entity.iType;//app need to use this iType to do add or delete member on it's groups
        if(entity.lUpTime > lRetUpTime)
            lRetUpTime = entity.lUpTime  ;


        i++;

    }
    pResponse["lUpTime"] = tConvert.LongToStr( lRetUpTime);

    if( groupMemberLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:一个群的所有成员
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::MemberById( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if(  pJson["lGroupId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;

    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
    GroupMemberList groupMemberLst;
    mGroupMemberDAL.GetMemberById(lGroupId,groupMemberLst);

    GroupMemberList::iterator it = groupMemberLst.begin();
    int i = 0;
    for( ; it != groupMemberLst.end() ; it++){
        GroupMemberEntity & entity = *it;

        if( enum_membertype_none ==  entity.iType ){
            continue;
        }
        pResponse["list"][i ]["lMemberId"] = tConvert.LongToStr( entity.lMemberId);
        pResponse["list"][i ]["sNickName"] =  entity.sNickName;

        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(entity.lMemberId),"-1nochange", "-1nochange",userDataEntity);
        pResponse["list"][i ]["sHeadImage"] =  userDataEntity.sHeadImage;
        i++;

    }

    if( i  <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:一批群的成员
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::MemberBylotsId( const Json::Value& pJson, Json::Value& pResponse )
{

    TConvert tConvert;

    //数据合法性
    if(  pJson["lGroupIdLst"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }



    vector<GroupUpTime> lGroupIdLst;

    Json::Value jsonGroupIdLst = pJson["lGroupIdLst"];
    for( int i = 0 ; i <  jsonGroupIdLst.size() ; i++ ){

        GroupUpTime gUpTime;
        gUpTime.lGroupId =  tConvert.Json2Long(jsonGroupIdLst[i]["lGroupId"]);
        gUpTime.lUpTime =  tConvert.Json2Long(jsonGroupIdLst[i]["lUpTime"]);
        lGroupIdLst.push_back( gUpTime);
    }

    GroupMemberList groupMemberLst,gMembertmp;
    mGroupMemberDAL.GetMemberBylotsId(lGroupIdLst,enum_membertype_member,groupMemberLst);
    mGroupMemberDAL.GetMemberBylotsId(lGroupIdLst,enum_membertype_none,gMembertmp);
    Append(groupMemberLst,gMembertmp);

    GroupMemberList::iterator it = groupMemberLst.begin();
    int i = 0;
    for( ; it != groupMemberLst.end() ; it++){
        GroupMemberEntity & entity = *it;

        pResponse["list"][i ]["lGroupId"] = tConvert.LongToStr( entity.lGroupId );
        pResponse["list"][i ]["lMemberId"] = tConvert.LongToStr( entity.lMemberId);
        pResponse["list"][i ]["sNickName"] =  entity.sNickName;
        pResponse["list"][i ]["iType"] =  entity.iType;
        pResponse["list"][i ]["lUpTime"] = tConvert.LongToStr( entity.lUpTime);

        UserDataDAL userDataDal;UserDataEntity userDataEntity;
        userDataDal.GetEntity(tConvert.LongToStr(entity.lMemberId),"-1nochange", "-1nochange",userDataEntity);
        pResponse["list"][i ]["sHeadImage"] =  userDataEntity.sHeadImage;
        i++;

    }

    if( i  <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:更改我在群中的名字
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::SetNickName( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull()  ||  pJson["lGroupId"].isNull() || pJson["sNickName"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);
    string sNickName = tConvert.Json2String(pJson["sNickName"]);


    GroupMemberDAL gMemberDal;

    GroupMemberEntity gMemberInfo;
    gMemberInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lUserId,gMemberInfo);
    if( gMemberInfo.iType != enum_membertype_member){
        RetJsonMsg::SetRetMsg(-143,pResponse);
        return -1;
    }

    iRet =  mGroupMemberDAL.SetNickName(lUserId, lGroupId, sNickName);

    if( 0 == iRet ){
        GroupMemberList gMemberLst;
        mGroupMemberDAL.GetRealMemberById(lGroupId,gMemberLst);
        GroupMemberList::iterator it = gMemberLst.begin();
        //向其他成员推送群成员变更昵称
        for( ; it != gMemberLst.end(); it++){
            GroupMemberEntity & gMemberEntity = *it;
            if( gMemberEntity.lMemberId == lUserId){
                continue;
            }
            PushMemberNameCmd(gMemberEntity.lMemberId, lUserId,lGroupId, sNickName);
        }

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


/*
Description:更改我的群配置
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int GroupMemberBLL::ModifyMySetting( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull()  ||  pJson["lGroupId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;int iRet = 0;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lGroupId = tConvert.Json2Long(pJson["lGroupId"]);


    if( !pJson["iMsgMode"].isNull() ){
        int iMsgMode = tConvert.Json2Int(pJson["iMsgMode"]);
        iRet =  mGroupMemberDAL.SetMsgMode(lUserId, lGroupId, iMsgMode);
    }

    if(! pJson["sNickName"].isNull()){
        iRet = SetNickName(pJson,pResponse);
    }


    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}



int GroupMemberBLL::AddMember(long lGroupId, long lMemberId, int iJoin)
{
    TConvert tConvert;
    GroupMemberEntity entity;
    entity.iJoin = iJoin;
    entity.iType = enum_membertype_member;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.lGroupId = lGroupId;
    entity.lMemberId = lMemberId;

    UserDataDAL userDataDal;UserDataEntity userDataEntity;
    userDataDal.GetEntity(tConvert.LongToStr(lMemberId),"-1nochange", "-1nochange",userDataEntity);
    entity.sNickName =userDataEntity.sNickName;

    return mGroupMemberDAL.AddData(entity);

}

//推送入群消息
int GroupMemberBLL::PushJoinInCmd( long lUserId, long lMemberId,string sNickName,long lGroupId,long lMasterId, string sGroupName,string sInviteNick)
{
    TConvert tConvert;

    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_join;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["lMemberId"] =  tConvert.LongToStr( lMemberId);
    jsonPush["Entity"]["sNickName"] =  sNickName;
    jsonPush["Entity"]["lMasterId"] =  tConvert.LongToStr( lMasterId);
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);
    jsonPush["Entity"]["sGroupName"] =  sGroupName;
    jsonPush["Entity"]["sInviteNick"] =  sInviteNick;

    PushDataCOM pushDataCOM;
    pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
}

//推送退群消息
int GroupMemberBLL::PushJoinOutCmd( long lUserId, long lMemberId, string sNickName,long lGroupId)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_out;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["lMemberId"] =  tConvert.LongToStr( lMemberId);
    jsonPush["Entity"]["sNickName"] =  sNickName;
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);

    PushDataCOM pushDataCOM;
    pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
}

//推送群主踢人消息
int GroupMemberBLL::PushKickOutCmd( long lUserId, long lMemberId,string sNickName,long lGroupId)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_kickout;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["lMemberId"] =  tConvert.LongToStr( lMemberId);
    jsonPush["Entity"]["sNickName"] =  sNickName;
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);

    PushDataCOM pushDataCOM;
    pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
}

//推送变更群信息
int GroupMemberBLL::PushSetGroupCmd( long lUserId,long lModifyId,string sNickName,long lGroupId ,string sGroupName)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_set;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["sGroupName"] =   sGroupName;
    jsonPush["Entity"]["lModifyId"] =  tConvert.LongToStr( lModifyId);
    jsonPush["Entity"]["sNickName"] =   sNickName;
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);

    PushDataCOM pushDataCOM;
    pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
}


//推送群成员变更昵称
//lUserId :推送到目的ID
int GroupMemberBLL::PushMemberNameCmd( long lUserId ,long lMemberId,long lGroupId ,string sNickName)
{
    TConvert tConvert;
    Json::Value jsonPush;
    jsonPush["iCmd"] = enum_cmd_group_membername;
    jsonPush["lUserId"] = tConvert.LongToStr( lUserId );
    jsonPush["Entity"]["sNickName"] =   sNickName;
    jsonPush["Entity"]["lGroupId"] =  tConvert.LongToStr( lGroupId);
    jsonPush["Entity"]["lMemberId"] =  tConvert.LongToStr( lMemberId);

    PushDataCOM pushDataCOM;
    pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );

}


void GroupMemberBLL::Append(GroupMemberList & L, const GroupMemberList & R)
{
    GroupMemberList::const_iterator it = R.begin();
    for( ; it != R.end() ; it++ ){
        L.push_back(*it);
    }
}


//生成群的头像(成员头像列表)
string GroupMemberBLL::MakeGroupHead(long lGroupId)
{

    string sHead;

    GroupMemberList groupMemberLst;
    mGroupMemberDAL.GetRealMemberById(lGroupId,groupMemberLst);

    GroupMemberList::iterator it = groupMemberLst.begin();
    vector<long> lUserIds;
    for( ; it != groupMemberLst.end() ; it++){
        GroupMemberEntity & entity = *it;
        lUserIds.push_back(entity.lMemberId);

    }
    UserDataDAL uDataDal;UserDataList uDataLst;
    uDataDal.GetUserInfoList(lUserIds,uDataLst);

    UserDataList::iterator iter = uDataLst.begin();
    int i = 0;
    for( ; iter != uDataLst.end(); iter++){
        UserDataEntity & entity = *iter;
        if( entity.sHeadImage.empty())
            continue;
        sHead += entity.sHeadImage + ",";
        i++;
        if( i > 9)
            break;
    }
    sHead = sHead.substr(0,sHead.size() -1);
    return sHead;

}


bool GroupMemberBLL::IsMember(long lGroupId, long lMemberId)
{

    GroupMemberDAL gMemberDal;

    GroupMemberEntity gInfo;
    gInfo.iType = enum_membertype_none;
    gMemberDal.GetEntity(lGroupId,lMemberId,gInfo);
    if( gInfo.iType != enum_membertype_member){
        return false;
    }else{
        return true;
    }

}


