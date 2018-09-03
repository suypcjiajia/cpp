/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装好友请求业务
 *Others:
*/

#include "FriendResquestBLL.h"
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

#include "dal/UserRelationDAL.h"
#include "dal/UserDataDAL.h"
#include "inc/CmdValue.h"
using namespace std;

/*
Description:分配业务接口我是谁
Input: @psMethod 方法名 @pJson Post数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FriendResquestBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Resquest" == psMethod){
        iRet = Resquest(pJson,pResponse);
    } else if ("DisAgree" == psMethod ){
        iRet = DisAgree(pJson,pResponse);
    } else if ("Agree" == psMethod ){
        iRet = Agree(pJson,pResponse);
    }  else if ("ReqToMe" == psMethod ){
        iRet =ReqToMe(pJson,pResponse);
    } else {
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
int FriendResquestBLL::Init()
{

    return 0;
}

/*
Description:申请好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FriendResquestBLL::Resquest( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull() || pJson["sRemark"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;

    //参数转换
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }


    FriendResquestEntity entity;
    entity.lUserId = lUserId;
    entity.lToUserId = lToUserId;
    entity.iAction = 1;
    entity.sRemark = tConvert.Json2String(pJson["sRemark"]);
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;
    entity.iState = 0;

    UserRelationDAL userRelDal;
    UserRelationEntity userRelEntity;
    userRelDal.GetEntity(entity.lUserId,entity.lToUserId,userRelEntity);
    if( userRelEntity.lUserId == entity.lUserId){
        if( userRelEntity.iType == 1  ){
            RetJsonMsg::SetRetMsg(-120,pResponse);
            return -1;
        }
        if( userRelEntity.iType == enum_rel_bbcacka || userRelEntity.iType == enum_rel_blackab || userRelEntity.iType == enum_rel_adelb_bblacka){
            RetJsonMsg::SetRetMsg(-121,pResponse);
            return -1;
        }
    }else if( userRelEntity.lToUserId == entity.lUserId){
        if( userRelEntity.iType == 1  ){
            RetJsonMsg::SetRetMsg(-120,pResponse);
            return -1;
        }
        if( userRelEntity.iType == enum_rel_ablackb || userRelEntity.iType == enum_rel_blackab || userRelEntity.iType == enum_rel_ablackb_bdela){
            RetJsonMsg::SetRetMsg(-121,pResponse);
            return -1;
        }
    }

    //添加申请(数据库)
    //iRet = mFriendResquestDAL.IsExist(entity.lUserId, entity.lToUserId);
    //if( iRet == 1){
        iRet = mFriendResquestDAL.AddData(entity);
    //}else if( iRet == 0){
      //  iRet = mFriendResquestDAL.UpdateAction( entity.lUserId, entity.lToUserId, 1);
    //}
    //else{
    //    RetJsonMsg::SetRetMsg(iRet,pResponse);
    //    return iRet;
    //}

    if( iRet == 0){
        UserDataDAL userDal;
        UserDataEntity userEntity,touserEntity;
        userDal.GetEntity(tConvert.LongToStr(entity.lUserId),"-1nochange","-1nochange",userEntity);
        //userDal.GetEntity(tConvert.LongToStr(entity.lToUserId),"-1nochange","-1nochange",touserEntity);
        //推送申请
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_resqfri;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["sRemark"] = entity.sRemark;
        jsonPush["Entity"]["sHeadImage"] = userEntity.sHeadImage;
        jsonPush["Entity"]["sNickName"] = userEntity.sNickName;
        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }

    //返回结果
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

/*
Description:不同意为好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FriendResquestBLL::DisAgree( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if(  pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    //更新为不同意
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);

    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }

    if ( 0 != (iRet = this->mFriendResquestDAL.UpdateAction(lToUserId,lUserId,enum_friresq_disagree))) {
        appendlog(TTDLogger::LOG_ERROR,"FriendResquestBLL::DisAgree FriendResquestDAL.UpdateAction  error");
        RetJsonMsg::SetRetMsg( iRet,pResponse);
        return -1;
    }

    if( iRet == 0){
        UserDataDAL userDal;
        UserDataEntity userEntity,touserEntity;
        //userDal.GetEntity(tConvert.LongToStr(entity.lUserId),"-1nochange","-1nochange",userEntity);
        userDal.GetEntity(tConvert.LongToStr(lUserId),"-1nochange","-1nochange",touserEntity);
        //推送不同意
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_disagreefri;
        jsonPush["lUserId"] = pJson["lToUserId"];
        jsonPush["CmdEntity"]["lUserId"] = tConvert.LongToStr( lToUserId );
        jsonPush["CmdEntity"]["lToUserId"] = tConvert.LongToStr( lUserId );

        jsonPush["Entity"]["sHeadImage"] = touserEntity.sHeadImage;
        jsonPush["Entity"]["sNickName"] = touserEntity.sNickName;

        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }

    //返回结果
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;
}

/*
Description:获取对方向我的申请
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FriendResquestBLL::ReqToMe( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    FriendResquestQueryEntity query;
    query.lToUserId = tConvert.Json2Long(pJson["lUserId"]);;
    query.lUserId = -1;
    query.lUpdateId =1;//tConvert.GetCurrentTime() - 60*60*24*3*1000000;//保留3天
    query.iAction = 1;


    if( query.lToUserId <= 0){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    //获取对方向我的申请
    FriendResquestList pLst;
    int iTotalCount = 0;
    if ( 0 != this->mFriendResquestDAL.GetData( query,0,99999999,pLst , iTotalCount) ){
        appendlog(TTDLogger::LOG_ERROR,"FriendResquestBLL::ReqToMe mFriendResquestDAL.GetData error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    int iCount = 0;

    //获取申请者昵称头像
    vector<long> useridLst;
    while(iCount <  pLst.size()){
        FriendResquestEntity & entity = pLst[ iCount ];
        useridLst.push_back(entity.lUserId);
        iCount++;
    }
    UserDataDAL userDal;
    UserDataList userdataLst;
    userDal.GetUserInfoList(useridLst,userdataLst);

    iCount = 0;


    //返回成功结果

    while(iCount <  pLst.size()){

        FriendResquestEntity & entity = pLst[ iCount ];
        pResponse["list"][iCount ]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iCount ]["lUserId"] =  tConvert.LongToStr( entity.lUserId);
        pResponse["list"][iCount ]["lToUserId"] =  tConvert.LongToStr( entity.lToUserId);
        pResponse["list"][iCount ]["sRemark"] = entity.sRemark;


        if(  userdataLst.size() == pLst.size()){
            UserDataEntity & userdataEntity = userdataLst[iCount];
            pResponse["list"][iCount ]["sHeadImage"] = userdataEntity.sHeadImage;
            pResponse["list"][iCount ]["sNickName"] = userdataEntity.sNickName;
        }else{
            pResponse["list"][iCount ]["sHeadImage"] = "";
            pResponse["list"][iCount ]["sNickName"] = "";
        }


        iCount++;
    }
    if( pLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    pResponse["iTotal"]  = iTotalCount;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


/*
Description:同意申请好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int FriendResquestBLL::Agree( const Json::Value& pJson, Json::Value& pResponse )
{

    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if(  pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);


    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }


    UserRelationDAL userRelDal;
    UserRelationEntity userRelEntity;
    userRelDal.GetEntity(lUserId, lToUserId,userRelEntity);
    if( userRelEntity.iType == enum_rel_friend  ){
        RetJsonMsg::SetRetMsg(-120,pResponse);
        return -1;
    }
    if( userRelEntity.lUserId == lUserId){
//        if( userRelEntity.iType == enum_rel_bbcacka || userRelEntity.iType == enum_rel_blackab
//              || userRelEntity.iType == enum_rel_adelb_bblacka){
//            RetJsonMsg::SetRetMsg(-122,pResponse);
//            return -1;
//        }
    }else if( userRelEntity.lToUserId == lUserId){
//        if( userRelEntity.iType == enum_rel_ablackb || userRelEntity.iType == enum_rel_blackab
//              || userRelEntity.iType == enum_rel_ablackb_bdela){
//            RetJsonMsg::SetRetMsg(-122,pResponse);
//            return -1;
//        }

    }



    if ( 0 != (iRet = this->mFriendResquestDAL.UpdateAction(lToUserId, lUserId,enum_friresq_agree))) {
        appendlog(TTDLogger::LOG_ERROR,"FriendResquestBLL::Agree UpdateAction  error");
        RetJsonMsg::SetRetMsg( iRet,pResponse);
        return -1;
    }
    mFriendResquestDAL.UpdateAction(lUserId,lToUserId,enum_friresq_default);

    //成为好友    
    iRet = userRelDal.IsExist(lUserId, lToUserId);
    if( iRet == -4){
        appendlog(TTDLogger::LOG_ERROR,"FriendResquestBLL::Agree userRelDal.IsExist  error");
        RetJsonMsg::SetRetMsg( iRet,pResponse);
        return -1;
    }
    if( iRet == 0){
        iRet = userRelDal.UpdateType(lUserId,lToUserId,1);
        iRet = userRelDal.UpdateType(lToUserId,lUserId,1);
    }else if( iRet == 1){
        UserRelationEntity userRelEntity;
        userRelEntity.iState = 0;
        userRelEntity.lUserId = lUserId;
        userRelEntity.lToUserId = lToUserId;
        userRelEntity.iType = 1;
        userRelEntity.lAddTime = tConvert.GetCurrentTime();
        userRelEntity.lUpTime = userRelEntity.lAddTime;
        iRet = userRelDal.AddData(userRelEntity);
    }

    //推送同意
    if( iRet == 0){
        UserDataDAL userDal;
        UserDataEntity userEntity,touserEntity;
        //userDal.GetEntity(tConvert.LongToStr(entity.lUserId),"-1nochange","-1nochange",userEntity);
        userDal.GetEntity(tConvert.LongToStr(lUserId),"-1nochange","-1nochange",touserEntity);
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_agreefri;
        jsonPush["lUserId"] = tConvert.LongToStr( lToUserId );
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( lToUserId );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( lUserId );
        //jsonPush["Entity"]["sUserHeadImage"] = userEntity.sHeadImage;
        //jsonPush["Entity"]["sUserNickName"] = userEntity.sNickName;
        jsonPush["Entity"]["sHeadImage"] = touserEntity.sHeadImage;
        jsonPush["Entity"]["sNickName"] = touserEntity.sNickName;

        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }

    //返回结果
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}

//设置默认值
void FriendResquestBLL::SetDefaultValue( FriendResquestEntity& pEntity )
{
    pEntity.lUserId = -1;
    pEntity.lToUserId = -1;
    pEntity.iAction = -1;
    pEntity.sRemark = "-1nochange";
}
