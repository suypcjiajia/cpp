/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户关系业务接口
 *Others:
*/



#include "UserRelationBLL.h"
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
#include "dal/UserDataDAL.h"
#include "dal/FriendResquestDAL.h"
#include "UserSetBLL.h"
#include "inc/CmdValue.h"
#include "common/userutil.h"
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if ("Black" == psMethod ){

        iRet =Black(pJson,pResponse);
    } else if ("MyFriends" == psMethod ){

        iRet =MyFriends(pJson,pResponse);
    }else if( "DelFri" == psMethod){
        iRet =DelFri(pJson,pResponse);
    }else if( "SetMark" == psMethod){
        iRet =SetMark(pJson,pResponse);
    }else if( "Contacts" == psMethod){
        iRet =Contacts(pJson,pResponse);
    }else {
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
int UserRelationBLL::Init()
{

    return 0;
}


/*
Description:拉黑好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::Black( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    UserRelationEntity entity;
    entity.lUserId = tConvert.Json2Long(pJson["lUserId"]);
    entity.lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    entity.iState = 0;
    entity.iType = enum_rel_ablackb;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;


    if( entity.lUserId == entity.lToUserId){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }

    //更新好友关系
    iRet = mUserRelationDAL.IsExist( entity.lUserId,entity.lToUserId);
    if( iRet == 0){
        UserRelationEntity userRelEntity;
        mUserRelationDAL.GetEntity(entity.lUserId, entity.lToUserId,userRelEntity);
        if( userRelEntity.lUserId == entity.lUserId){
            if(enum_rel_no  == userRelEntity.iType || enum_rel_friend ==  userRelEntity.iType
                    || enum_rel_stranger == userRelEntity.iType || enum_rel_ablackb == userRelEntity.iType ){
                entity.iType = enum_rel_ablackb;
            }else if( userRelEntity.iType == enum_rel_bbcacka){
                entity.iType = enum_rel_blackab;
            }else if( userRelEntity.iType == enum_rel_adelb){
                entity.iType = enum_rel_ablackb;
            }else if( userRelEntity.iType == enum_rel_bdela){
                entity.iType = enum_rel_ablackb_bdela;
            }else if( userRelEntity.iType == enum_rel_blackab){
                entity.iType = enum_rel_blackab;
            }else if( userRelEntity.iType == enum_rel_delab){
                entity.iType = enum_rel_ablackb_bdela;
            }else if( userRelEntity.iType == enum_rel_ablackb_bdela){
                entity.iType = enum_rel_ablackb_bdela;
            }else if( userRelEntity.iType == enum_rel_adelb_bblacka){
                entity.iType = enum_rel_blackab;
            }
            iRet = mUserRelationDAL.UpdateType( entity.lUserId,entity.lToUserId,entity.iType);
        }else if(userRelEntity.lToUserId == entity.lUserId){
            if( enum_rel_no  == userRelEntity.iType  ||  enum_rel_friend == userRelEntity.iType
                    || enum_rel_stranger == userRelEntity.iType || enum_rel_bbcacka == userRelEntity.iType ) {
                entity.iType = enum_rel_bbcacka;
            }else if( userRelEntity.iType == enum_rel_ablackb){
                entity.iType = enum_rel_blackab;
            }else if( userRelEntity.iType == enum_rel_adelb){
                entity.iType = enum_rel_adelb_bblacka;
            }else if( userRelEntity.iType == enum_rel_bdela){
                entity.iType = enum_rel_bbcacka;
            }else if( userRelEntity.iType == enum_rel_blackab){
                entity.iType = enum_rel_blackab;
            }else if( userRelEntity.iType == enum_rel_delab){
                entity.iType = enum_rel_adelb_bblacka;
            }else if( userRelEntity.iType == enum_rel_ablackb_bdela){
                entity.iType = enum_rel_blackab;
            }else if( userRelEntity.iType == enum_rel_adelb_bblacka){
                entity.iType = enum_rel_adelb_bblacka;
            }
            iRet = mUserRelationDAL.UpdateType( entity.lToUserId,entity.lUserId,entity.iType);
        }

    }else if( iRet == 1)
    {
        iRet = mUserRelationDAL.AddData(entity);
    }else{
        appendlog(TTDLogger::LOG_ERROR,"UserRelationBLL::Black error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    FriendResquestDAL freResDal;
    freResDal.UpdateAction(entity.lUserId,entity.lToUserId,enum_friresq_disagree);
    freResDal.UpdateAction(entity.lToUserId,entity.lUserId,enum_friresq_disagree);

    if( iRet == 0){
        //推送拉黑
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_blackfri;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( entity.lToUserId );

        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }

    UserSetBLL setBll;
    setBll.DelTopChatFriend(entity.lUserId,entity.lToUserId);

    //返回
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}


/*
Description:删除好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::DelFri( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    UserRelationEntity entity;
    entity.lUserId = tConvert.Json2Long(pJson["lUserId"]);
    entity.lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    entity.iState = 0;
    entity.iType = enum_rel_adelb;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lUpTime = entity.lAddTime;


    if( entity.lUserId == entity.lToUserId){
        RetJsonMsg::SetRetMsg(-123,pResponse);
        return -1;
    }

    //更新好友关系
    iRet = mUserRelationDAL.IsExist( entity.lUserId,entity.lToUserId);
    if( iRet == 0){
        UserRelationEntity userRelEntity;
        mUserRelationDAL.GetEntity(entity.lUserId, entity.lToUserId,userRelEntity);

        if( userRelEntity.lUserId == entity.lUserId){
            if(enum_rel_no  <= userRelEntity.iType && userRelEntity.iType <= enum_rel_stranger){
                entity.iType = enum_rel_adelb;
            }else if( userRelEntity.iType == enum_rel_ablackb){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_bbcacka){
                entity.iType = enum_rel_adelb_bblacka;
            }else if( userRelEntity.iType == enum_rel_adelb){
                entity.iType = enum_rel_adelb;
            }else if( userRelEntity.iType == enum_rel_bdela){
                entity.iType = enum_rel_delab;
            }else if( userRelEntity.iType == enum_rel_blackab){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_delab){
                entity.iType = enum_rel_delab;
            }else if( userRelEntity.iType == enum_rel_ablackb_bdela){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_adelb_bblacka){
                entity.iType = enum_rel_adelb_bblacka;
            }else if( userRelEntity.iType == enum_rel_stranger){
                entity.iType = enum_rel_stranger;
            }
            iRet = mUserRelationDAL.UpdateType( entity.lUserId,entity.lToUserId,entity.iType);
        }else if(userRelEntity.lToUserId == entity.lUserId){
            if( enum_rel_no  == userRelEntity.iType  ||  enum_rel_friend == userRelEntity.iType
                    || enum_rel_stranger == userRelEntity.iType ) {
                entity.iType = enum_rel_bdela;
            }else if( userRelEntity.iType == enum_rel_ablackb){
                entity.iType = enum_rel_ablackb_bdela;
            }else if( userRelEntity.iType == enum_rel_bbcacka){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_adelb){
                entity.iType = enum_rel_delab;
            }else if( userRelEntity.iType == enum_rel_bdela){
                entity.iType = enum_rel_bdela;
            }else if( userRelEntity.iType == enum_rel_blackab){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_delab){
                entity.iType = enum_rel_delab;
            }else if( userRelEntity.iType == enum_rel_ablackb_bdela){
                entity.iType = enum_rel_ablackb_bdela;
            }else if( userRelEntity.iType == enum_rel_adelb_bblacka){
                RetJsonMsg::SetRetMsg(-122,pResponse);
                return -1;
            }else if( userRelEntity.iType == enum_rel_stranger){
                entity.iType = enum_rel_stranger;
            }
            //mUserRelationDAL.UpdateMark(entity.lUserId,entity.lToUserId,2,"");
            iRet = mUserRelationDAL.UpdateType( entity.lToUserId,entity.lUserId,entity.iType);
        }
    }else if( iRet == 1)
    {
        iRet = mUserRelationDAL.AddData(entity);
    }else{
        appendlog(TTDLogger::LOG_ERROR,"UserRelationBLL::Del error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    if( iRet == 0){
        //推送删除好友
        Json::Value jsonPush;
        jsonPush["iCmd"] = enum_cmd_delfri;
        jsonPush["lUserId"] = tConvert.LongToStr( entity.lToUserId );
        jsonPush["Entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        jsonPush["Entity"]["lToUserId"] = tConvert.LongToStr( entity.lToUserId );

        PushDataCOM pushDataCOM;
        pushDataCOM.PushData( TBLL::GetInstance()->GetPushIP(), jsonPush.toStyledString() );
    }

    UserSetBLL setBll;
    setBll.DelTopChatFriend(entity.lUserId,entity.lToUserId);

    //返回
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;

}

/*
Description:设置好友备注
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::SetMark( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull() ||  pJson["sMark"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    string sMark = tConvert.Json2String(pJson["sMark"]);


    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    //更新好友备注
    iRet = mUserRelationDAL.IsExist(lUserId,lToUserId);
    if( iRet == 0){
        iRet = mUserRelationDAL.UpdateMark( lUserId,lToUserId,0,sMark);
        iRet = mUserRelationDAL.UpdateMark( lToUserId,lUserId,1,sMark);
    }else{
        appendlog(TTDLogger::LOG_ERROR,"UserRelationBLL::SetMark error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    //返回
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return 0;
}

/*
Description:获取我的好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::MyFriends( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lUpTime"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    //获取列表内容
    UserRelationEntityList relationList;
    UserRelationQueryEntity query;
    query.lUserId = tConvert.Json2Long(pJson["lUserId"]);
    query.lToUserId = query.lUserId;
    query.iType = -1;
    int iTotalCount = 0;
    query.lUpTime = tConvert.Json2Long(pJson["lUpTime"]);


    if( query.lUserId <= 0){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    mUserRelationDAL.GetData( query,relationList,iTotalCount);


    UserRelationEntity uRelationSys;
    uRelationSys.iState = 0;
    uRelationSys.iType = 1;
    uRelationSys.lAddTime = tConvert.GetCurrentTime();
    uRelationSys.lUpTime = uRelationSys.lAddTime;
    uRelationSys.lToUserId = 28;// system id
    uRelationSys.lUserId = query.lUserId;
    relationList.push_back(uRelationSys);
    uRelationSys.lToUserId = 29;// system id
    uRelationSys.lUserId = query.lUserId;
    relationList.push_back(uRelationSys);


    int iFor = 0;
    //获取申请者昵称头像
    vector<long> useridLst;
    while(iFor <  relationList.size()){
        UserRelationEntity & entity = relationList[ iFor ];
        if( entity.lUserId == query.lUserId){
            useridLst.push_back(entity.lToUserId);
        }else{
            useridLst.push_back(entity.lUserId);
        }
        iFor++;
    }
    UserDataDAL userDal;
    UserDataList userdataLst;
    userDal.GetUserInfoList(useridLst,userdataLst);




    int iContinueCount = 0;
    int iLstIndex = 0;
    UserDataEntity userdataEntity;
    for(int iRow = 0; iRow < relationList.size(); ++iRow ){

        UserRelationEntity& entity = relationList[iRow];
        if( entity.iType == enum_rel_stranger){
            iContinueCount ++;
            continue;
        }
        if( entity.lUserId == query.lUserId){
            if( entity.iType == 5 || entity.iType ==3 || entity.iType ==7
                    || entity.iType ==8|| entity.iType ==9 || entity.iType ==10 ){
                iContinueCount++;
                continue;
            }
            pResponse["list"][iLstIndex]["lToUserId"] = tConvert.LongToStr(entity.lToUserId);
            pResponse["list"][iLstIndex]["iType"] = entity.iType;
            pResponse["list"][iLstIndex ]["sMark"] = entity.sToUserMark;

            UserUtil::GetUserData(entity.lToUserId,userdataLst,userdataEntity);


        }else{
            if( entity.iType == 6 || entity.iType ==4 || entity.iType ==7
                    || entity.iType ==8 || entity.iType ==9 || entity.iType ==10){
                iContinueCount++;
                continue;
            }
            pResponse["list"][iLstIndex]["lToUserId"] = tConvert.LongToStr(entity.lUserId);
            int iType = entity.iType;
            if( entity.iType == 3){
                iType = 4;
            }else if( entity.iType == 5){
                iType = 6;
            }
            pResponse["list"][iLstIndex]["iType"] = iType;
            pResponse["list"][iLstIndex ]["sMark"] = entity.sUserMark;

            UserUtil::GetUserData(entity.lUserId,userdataLst,userdataEntity);
        }
        pResponse["list"][iLstIndex ]["sHeadImage"] = userdataEntity.sHeadImage;
        pResponse["list"][iLstIndex ]["sNickName"] = userdataEntity.sNickName;
        pResponse["list"][iLstIndex ]["lUpTime"] = tConvert.LongToStr(entity.lUpTime);


        iLstIndex ++;

    }
    if( relationList.size()- iContinueCount <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:通信录用户状态
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserRelationBLL::Contacts( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["sContacts"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);

    Json::Value sContacts = pJson["sContacts"];
    for( int i = 0 ; i < sContacts.size(); i++){
        string sPhoneNum = tConvert.Json2String(sContacts[i]["sPhoneNum"]);

        if(sContacts[i]["sPhoneNum"].isNull()){
            RetJsonMsg::SetRetMsg(-15,pResponse);
            return -1;
        }
        UserDataDAL uDataDal;
        bool bExist = false;
        uDataDal.IsExistByAccont(sPhoneNum,bExist);


        pResponse["list"][i]["sPhoneNum"] = sPhoneNum;

        pResponse["list"][i]["bReg"] = bExist;

        if( bExist){
            UserDataEntity uEntity;
            uDataDal.GetEntityByAccount(sPhoneNum,uEntity);
            UserRelationEntity relEntity;
            relEntity.iType = enum_rel_no;
            mUserRelationDAL.GetEntity(lUserId,uEntity.lUserId,relEntity);
            if(relEntity.iType == enum_rel_friend){
                pResponse["list"][i]["bFriend"] = true;
            }else{
                pResponse["list"][i]["bFriend"] = false;
            }
            pResponse["list"][i]["sHeadImage"] = uEntity.sHeadImage;
            pResponse["list"][i]["sNickName"] = uEntity.sNickName;
            pResponse["list"][i]["lUserId"] = tConvert.LongToStr( uEntity.lUserId );
        }

        pResponse["list"][i]["sName"] = sContacts[i]["sName"];


    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;



}



//设置默认值
void UserRelationBLL::SetDefaultValue( UserRelationEntity& pEntity )
{
    pEntity.lToUserId = -1;
    pEntity.iType = -1;
    pEntity.lUserId = -1;
}
