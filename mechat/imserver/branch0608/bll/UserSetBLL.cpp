/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户配置业务接口
 *Others:
*/


#include "UserSetBLL.h"
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
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserSetBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "SetTopChatFriend" == psMethod){
        iRet = SetTopChatFriend(pJson,pResponse);
    }else if( "MyTopChatFriends" == psMethod){
        iRet = MyTopChatFriends(pJson,pResponse);
    }else if( "DelTopChatFriend" == psMethod){
        iRet = DelTopChatFriend(pJson,pResponse);
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
int UserSetBLL::Init()
{

    return 0;
}
/*
Description:设置好友置顶
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserSetBLL::SetTopChatFriend( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    iRet = mUserSetDAL.IsExist(lUserId);
    if( iRet == 1){
        UserSetEntity entity;
        entity.lUserId = lUserId;
        entity.sTopChatFriends = tConvert.LongToStr(lToUserId);
        entity.lAddTime = tConvert.GetCurrentTime();
        entity.lUpTime = entity.lAddTime;
        iRet = mUserSetDAL.AddData(entity);
    }else if( iRet == 0){
        UserSetEntity setEntity;
        mUserSetDAL.GetEntity(lUserId,setEntity);
        iRet = 0;

        if(setEntity.sTopChatFriends.find( tConvert.LongToStr(lToUserId)) == string::npos){
            string sTmp;
            if( setEntity.sTopChatFriends.empty()){
                sTmp = tConvert.LongToStr(lToUserId);
            }else{
                sTmp = tConvert.LongToStr(lToUserId) + "," + setEntity.sTopChatFriends;
            }
            setEntity.sTopChatFriends = sTmp;
            iRet = mUserSetDAL.UpdateTopChatFriends( lUserId, setEntity.sTopChatFriends);
        }

    }
    //返回结果
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

/*
Description:取消好友置顶
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserSetBLL::DelTopChatFriend( const Json::Value& pJson, Json::Value& pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["lToUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    TConvert tConvert;
    int iRet = -1;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    long lToUserId = tConvert.Json2Long(pJson["lToUserId"]);
    if( lUserId == lToUserId){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    iRet = DelTopChatFriend(lUserId,lToUserId);
    //返回结果
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

/*
Description:获取我的置顶的好友
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserSetBLL::MyTopChatFriends( const Json::Value& pJson, Json::Value& pResponse )
{
    TConvert tConvert;

    //数据合法性
    if( pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }

    long lUserId = tConvert.Json2Long(pJson["lUserId"]);;



    if( lUserId <= 0){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }



    UserSetEntity setEntity;
    if ( 0 != this->mUserSetDAL.GetEntity(lUserId ,setEntity) ){
        appendlog(TTDLogger::LOG_ERROR,"UserSetBLL::MyTopChatFriend mUserSetDAL.GetData error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    pResponse["sTopChatFriends"] = setEntity.sTopChatFriends;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}
//取消好友置顶
int UserSetBLL::DelTopChatFriend(const long & lUserId, const long & lToUserId)
{
    TConvert tConvert;
    int iRet = mUserSetDAL.IsExist(lUserId);
    if( iRet == 0){
        UserSetEntity setEntity;
        mUserSetDAL.GetEntity(lUserId,setEntity);
        iRet = 0;

        int iPos = setEntity.sTopChatFriends.find( tConvert.LongToStr(lToUserId)) ;

        if( iPos >=0){
            setEntity.sTopChatFriends.erase(iPos,tConvert.LongToStr(lToUserId).length()+1);
        }
        if(setEntity.sTopChatFriends.at(setEntity.sTopChatFriends.length() -1) == ','){
            setEntity.sTopChatFriends.erase(setEntity.sTopChatFriends.length() -1);
        }
        iRet = mUserSetDAL.UpdateTopChatFriends( lUserId, setEntity.sTopChatFriends);


    }
    return iRet;
}

