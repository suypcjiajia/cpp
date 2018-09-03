#include "PowerBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "MyUtil.h"
#include "TBLL.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "com/TCurl.h"
#include "dal/ShopInfoDAL.h"

#include "com/Request.h"
using namespace std;
PowerBLL* PowerBLL::mInstance = NULL;
PowerBLL* PowerBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new PowerBLL;
    }
    return mInstance;
}


//处理带token，不加密的情况
int PowerBLL::Process2(EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;

    string sUrl = pEvhtp->GetUrl();
    string sDeToken = pEvhtp->GetHead("detoken");

    if( sUrl.find( "/power/getUser") != string::npos ){


        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        GetUser2(id,pJson,pResponse);

    }else if( sUrl.find( "/power/add") != string::npos ){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        Add2(id,pJson,pResponse);

    }else if( sUrl.find( "/power/del") != string::npos ){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        Del2(id,pJson,pResponse);

    }
    return iRet;
}


int PowerBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "Check" == psMethod){
        iRet = Check(pJson,pResponse);
    }else if( "integrality" == psMethod){
        iRet = integrality(pJson,pResponse);
    }
    else
    {
        RetJsonMsg::SetRetMsg2(-19,pResponse);
    }
    return iRet;
}



//初始化
int PowerBLL::Init()
{
    return 0;
}

//
int PowerBLL::GetUser2(long idPlatform, const  Json::Value  & pJson,
                       Json::Value  & pResponse)
{

    int iRet = -1;

    if(  pJson["page"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }


    long  userId  = -1;
    if( !pJson["userId"].isNull()){
        userId  =   tConvert.Json2Long( pJson["userId"]);
    }
    string   account;
    if( !pJson["account"].isNull()){
        account  =   tConvert.Json2String( pJson["account"]);
    }
    string  nickName  ;
    if( !pJson["nickName"].isNull()){
        nickName  =   tConvert.Json2String(  pJson["nickName"]);
    }
    int  power  = -1;
    if( !pJson["power"].isNull()){
        power  =   tConvert.Json2Int( pJson["power"]);
    }
    int page  =   tConvert.Json2Int(pJson["page"]);
    int count  =  10;



    UserPowerLst lst;int iTotalCount = 0;
    mUserPowerDAL.GetData(idPlatform,power, page,count,userId,account,nickName,lst,iTotalCount);

    int iCount = 0;
    while( iCount < lst.size()){
        UserPower  & entity = lst[iCount];
        pResponse["result"][iCount]["account"] = entity.account;
        pResponse["result"][iCount]["headImage"] =  entity.headImage;
        pResponse["result"][iCount]["nickName"] =  entity.nickName;
        pResponse["result"][iCount]["idPlatform"] = tConvert.LongToStr( entity.idPlatform) ;
        pResponse["result"][iCount]["userId"] = tConvert.LongToStr(entity.userId);
        pResponse["result"][iCount]["power"] = entity.power;

        iCount++;
    }
    if( lst.size() <= 0){
        pResponse["result"] ;
        pResponse["result"].resize(0);
    }

    int pageCount = iTotalCount/count;
    if( iTotalCount%count != 0){
        pageCount++;
    }
    pResponse["pageCount"] = pageCount;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}


int PowerBLL::Add2(long idPlatform,const  Json::Value  & pJson,
                   Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["account"].isNull()||  pJson["power"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    string account  =   tConvert.Json2String( pJson["account"]);
    int power  =   tConvert.Json2Int( pJson["power"]);

    Json::Value jsonValue;
    Json::Value jsonReturn;
    jsonValue["sAccount"]= account;
    string sUrl = "http://" + TBLL::GetInstance()->GetMechatIP() + ":23241/?sBusiness=UserQuery&sMethod=GetUserInfo";

    iRet = Request::CppHttpPost(sUrl,jsonValue,jsonReturn);

    if(0 == iRet ){
        UserPower userPower;
        userPower.userId = tConvert.Json2Long(jsonReturn["entity"]["lUserId"]);
        userPower.nickName = tConvert.Json2String(jsonReturn["entity"]["sNickName"]);
        userPower.headImage = tConvert.Json2String(jsonReturn["entity"]["sHeadImage"]);
        userPower.account = account;
        userPower.power = power;
        userPower.idPlatform = idPlatform;
        iRet = mUserPowerDAL.Add(userPower);
    }

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    if( iRet == -5){
        pResponse["msg"] = "手机号还没注册";
    }

    return iRet;

}

int PowerBLL::Del2(long idPlatform,const  Json::Value  & pJson,
                   Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["userId"].isNull()||  pJson["power"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    long userId  =   tConvert.Json2Long( pJson["userId"]);
    int power  =   tConvert.Json2Int( pJson["power"]);

    iRet = mUserPowerDAL.Del(idPlatform,userId,power);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}

//检验商家资料完整性
int PowerBLL::integrality(const  Json::Value  & pJson,
                Json::Value & pResponse)
{
    if(  pJson["idPlatform"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    ShopInfoEntity entity;
    ShopInfoDAL::GetInstance()->GetEntity(tConvert.Json2String(pJson["idPlatform"]),1,entity);
    bool bRet =  ShopInfoBLL::GetInstance()->IsShopPerfect(entity);
    pResponse["result"]["integrality"] = bRet;
    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}


//检验是否有权限
int PowerBLL::Check(const  Json::Value  & pJson,
                    Json::Value & pResponse)
{
    int iRet = 0;

    if( pJson["userId"].isNull() || pJson["idPlatform"].isNull() || pJson["power"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    long idPlatform  =   tConvert.Json2Long(pJson["idPlatform"]);
    int power  =   tConvert.Json2Int( pJson["power"]);

    ShopInfoEntity shopInfo;

    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(idPlatform),1,shopInfo);
    string msg;

    if( iRet != 0){
        msg = "平台号不存在";
        iRet = -1;
    }
    if( shopInfo.contentEntity.sCtiy == "美支付供应商"){
        msg = "不能登录供应商";
        iRet = -1;
    }

    if( iRet == 0){

        if( shopInfo.contentEntity.lUserId == userId){

            iRet = 0;

        }else{

            iRet = mUserPowerDAL.IsExist(idPlatform,userId,power);

            if( 0 == iRet){
                iRet = 0;
            }else{
                msg = "您没有登录权限,请联系商家";
            }
        }
    }


    pResponse["result"]["sQrCodeID"] = shopInfo.sPrimaryKey;
    pResponse["result"]["sAccount"] = shopInfo.contentEntity.sAccount;
    pResponse["result"]["sShopPhone"] = shopInfo.contentEntity.sShopPhone;
    pResponse["result"]["sMerchantPhone"] = shopInfo.contentEntity.sShopPhone;
    pResponse["result"]["sShopName"] = shopInfo.contentEntity.sShopName;

    RetJsonMsg::SetRetMsg2(iRet,pResponse,msg);
    return iRet;

}

