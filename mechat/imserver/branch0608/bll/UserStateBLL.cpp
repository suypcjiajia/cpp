
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户状态业务接口
 *Others:
*/



#include "UserStateBLL.h"
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
using namespace std;

/*
Description:分配业务接口
Input: @psMethod 方法名 @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserStateBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "UpdateData" == psMethod){
        //状态-更新
        iRet = Updata(pJson,pResponse);
    }else if("GetNeerBy" == psMethod ){
        //状态-附近的人
        iRet =GetNeerBy(pJson,pResponse);
    }else if("LoginTime" == psMethod ){
        //状态
        iRet =GetLoginTime(pJson,pResponse);
    }else{
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
int UserStateBLL::Init()
{

    return 0;
}

/*
Description:更新状态
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserStateBLL::Updata( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["sVersion"].isNull() || pJson["dGpsLon"].isNull()
            || pJson["dGpsLat"].isNull() || pJson["sNickName"].isNull() || pJson["iSex"].isNull()
            || pJson["sSignature"].isNull() || pJson["sHeadImage"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;

    //条件合法性-坐标
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    if ( 0 >= lUserId || 0 == tConvert.Json2Long(pJson["dGpsLon"]) || 0 == tConvert.Json2Long(pJson["dGpsLat"])  ) {
        RetJsonMsg::SetRetMsg(-21,pResponse);
        return -1;
    }

    //参数转换
    UserStateEntity entity;

    entity.sPrimaryKey = tConvert.LongToStr(lUserId);
    entity.lUserId = lUserId;

    entity.sVersion = tConvert.Json2String(pJson["sVersion"]);
    entity.dGpsLon = tConvert.Json2Double(pJson["dGpsLon"]);
    entity.dGpsLat = tConvert.Json2Double(pJson["dGpsLat"]);
    entity.sNickName = tConvert.Json2String(pJson["sNickName"]);
    entity.iSex = tConvert.Json2Int(pJson["iSex"]);
    entity.sSignature = tConvert.Json2String(pJson["sSignature"]);
    entity.sHeadImage = tConvert.Json2String(pJson["sHeadImage"]);

    //操作数据库
    bool bExist = false;
    mUserStateDAL.IsExistByUserId(tConvert.LongToStr( entity.lUserId),bExist);
    if( !bExist){
        if ( 0 != this->mUserStateDAL.Insert(entity)) {
            appendlog(TTDLogger::LOG_ERROR,"UserStateBLL::Insert  error");
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -1;
        }
    }else{
        mUserStateDAL.Update(entity);
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:附近的人
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserStateBLL::GetNeerBy( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["dGpsLon"].isNull() || pJson["dGpsLat"].isNull()
            || pJson["dDistance"].isNull() || pJson["iCount"].isNull() || pJson["iSexType"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    //条件合法性-坐标
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    if ( 0 >= lUserId || 0 == tConvert.Json2Long(pJson["dGpsLon"]) || 0 == tConvert.Json2Long(pJson["dGpsLat"])  ) {
        RetJsonMsg::SetRetMsg(-21,pResponse);
        return -1;
    }

    //获取用户内容
    vector<UserStateEntity> userStateVector;

    if ( 0 != this->mUserStateDAL.GetNeerBy( lUserId,
                                             tConvert.Json2Double(pJson["dGpsLat"]),
                                             tConvert.Json2Double(pJson["dGpsLon"]),
                                             tConvert.Json2Double(pJson["dDistance"]),
                                             tConvert.Json2Int(pJson["iCount"]),
                                             tConvert.Json2Int(pJson["iSexType"]),
                                             userStateVector) ){
        appendlog(TTDLogger::LOG_ERROR,"UserStateBLL::GetNeerBy error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }



    //转换返回值
    for(int iRow = 0; iRow < userStateVector.size(); ++iRow ){
        UserStateEntity& entity = userStateVector[iRow];

        pResponse["list"][iRow]["lUserId"] = tConvert.LongToStr(entity.lUserId);
        pResponse["list"][iRow]["lUpTime"] = tConvert.LongToStr(entity.lUpTime);
        pResponse["list"][iRow]["dGpsLon"] = entity.dGpsLon;
        pResponse["list"][iRow]["dGpsLat"] = entity.dGpsLat;
        pResponse["list"][iRow]["sNickName"] = entity.sNickName;
        pResponse["list"][iRow]["iSex"] = entity.iSex;
        pResponse["list"][iRow]["sSignature"] = entity.sSignature;
        pResponse["list"][iRow]["sHeadImage"] = entity.sHeadImage;
        pResponse["list"][iRow]["lDistance"] = tConvert.LongToStr(entity.lDistance);

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
Description:获取最近的登录时间
Input:  @pJson Post请求数据
Output: @pResponse 处理结果
Return: 0 -success
*/
int UserStateBLL::GetLoginTime( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["lUserId"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iRet = -1;
    TConvert tConvert;
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    if ( 0 >= lUserId  ) {
        RetJsonMsg::SetRetMsg(-21,pResponse);
        return -1;
    }
    long lLoginTime = 0;
    iRet = mUserStateDAL.GetLoginTime(lUserId, lLoginTime);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    RetJsonMsg::SetRetMsg(0,pResponse);
    pResponse["lLoginTime"] = tConvert.LongToStr(lLoginTime) ;
    return 0;

}
