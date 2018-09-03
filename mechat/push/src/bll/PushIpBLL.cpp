#include "PushIpBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "TLog.h"
#include "TFile.h"
#include "TConvert.h"
#include "TBLL.h"
#include "MyAES.h"
#include "TMd5.h"
#include "com/PostsAes.h"
//#include "com/EvhtpSvr.h"

using namespace std;

//方法调用
int PushIpBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Replace" == psMethod ){
        //状态-更新
        iRet = Replace(pJson,pResponse);
    }else if( "GetEntity" == psMethod ){
        //状态-查询行数据
        iRet =GetEntity(pJson,pResponse);
    }else{
        //EvhtpSvr::SetRetMsg(-19,pResponse);
    }
    return iRet;
}

//初始化
int PushIpBLL::Init()
{

    return 0;
}

//推送机-更新
int PushIpBLL::Replace( const  Json::Value  & pJson, Json::Value & pResponse )
{
    //数据合法性
    if( pJson["lUserId"].isNull() || pJson["sPushIp"].isNull() ){
        //EvhtpSvr::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;

    //条件合法性-坐标
    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
    if ( 0 >= lUserId ) {
        //EvhtpSvr::SetRetMsg(-21,pResponse);
        return -1;
    }

    //参数转换
    PushIpEntity entity;

    entity.lUserId = lUserId;
    entity.sPushIp = tConvert.Json2String(pJson["sPushIp"]);

    //操作数据库
    if ( 0 != this->mPushIpDAL.Replace(entity)) {
        tLog(LOG_ERROR,"PushIpBLL::Replace error");
        //EvhtpSvr::SetRetMsg(-4,pResponse);
        return -1;
    }

    //EvhtpSvr::SetRetMsg(0,pResponse);
    return 0;
}

//推送机-查询行数据
int PushIpBLL::GetEntity( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    //数据合法性
    if( pJson["lUserId"].isNull() ){
        //EvhtpSvr::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;

    //获取用户内容
    PushIpEntity entity;
    if ( 0 != this->mPushIpDAL.GetEntity( tConvert.Json2Long(pJson["lUserId"]),entity) ){
        tLog(LOG_ERROR,"PushIpBLL::GetEntity error");
        //EvhtpSvr::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (entity.sPushIp.empty()) {
        //EvhtpSvr::SetRetMsg(-5,pResponse);
        return -1;
    }
    //转换返回值
    pResponse["entity"]["lUserId"] = tConvert.LongToStr(entity.lUserId);
    pResponse["entity"]["lUpTime"] = tConvert.LongToStr(entity.lUpTime);
    pResponse["entity"]["sAccount"] = entity.sPushIp;

    //EvhtpSvr::SetRetMsg(0,pResponse);
    return 0;
}
