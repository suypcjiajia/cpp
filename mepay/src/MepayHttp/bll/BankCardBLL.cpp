#include "BankCardBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/BankCardDAL.h"
#include "TBLL.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "com/TCurl.h"
using namespace std;
BankCardBLL* BankCardBLL::mInstance = NULL;
BankCardBLL* BankCardBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new BankCardBLL;
    }
    return mInstance;
}

int BankCardBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "AddData" == psMethod){

        iRet = Insert(pJson,pResponse);

    }else if("DeleteData" == psMethod){

        iRet =Delete(pJson,pResponse);

    }else if( "GetEntity" == psMethod){

        iRet = GetEntity(pJson,pResponse);

    }else if( "GetData" == psMethod){

        iRet = GetData(pJson,pResponse);

    }else{
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}



//初始化
int BankCardBLL::Init()
{
    BankCardDAL::GetInstance()->CreateTable();
    return 0;
}

//增加(绑定银行卡)
int BankCardBLL::Insert( const  Json::Value  & pJson, Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;
    BankCardEntity entity;
    entity.iState = 0;
    long lCurrentTime = tConvert.GetCurrentTime();
    entity.lAddTime = lCurrentTime;
    entity.lUpdateId = lCurrentTime;
    entity.lUpTime = lCurrentTime;

    if( pJson["sBankCard"].isNull() || pJson["lUserId"].isNull()  || pJson["sPhone"].isNull() || pJson["sName"].isNull()
            || pJson["sBank"].isNull()  || pJson["sBranch"].isNull()  || pJson["iCerType"].isNull()
            || pJson["sCerId"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    entity.lUserId =  tConvert.Json2Long( pJson["lUserId"]) ;
    entity.sBankCard = tConvert.Json2String( pJson["sBankCard"]) ;
    entity.sName = tConvert.Json2String( pJson["sName"]) ;
    entity.sBank = tConvert.Json2String( pJson["sBank"]) ;
    entity.sBranch = tConvert.Json2String( pJson["sBranch"]) ;
    entity.iCerType = tConvert.Json2Int( pJson["iCerType"]) ;
    entity.sCerId = tConvert.Json2String( pJson["sCerId"]) ;
    entity.sPrimaryKey = entity.sBankCard + tConvert.LongToStr(entity.lUserId);
    string sPhone = tConvert.Json2String( pJson["sPhone"]) ;

    if( ! pJson["sCustomerNm"].isNull() ){
        entity.sCustomerNm =  tConvert.Json2String(pJson["sCustomerNm"]);
    }else{
        entity.sCustomerNm = "";
    }
    if( ! pJson["sCredit1"].isNull() ){
        entity.sCredit1 =  tConvert.Json2String(pJson["sCredit1"]);
    }else{
        entity.sCredit1 = "";
    }
    if( ! pJson["sCredit2"].isNull() ){
        entity.sCredit2 =  tConvert.Json2String(pJson["sCredit2"]);
    }else{
        entity.sCredit2 = "";
    }
    if( ! pJson["sBindId"].isNull() ){
        entity.sBindId =  tConvert.Json2String(pJson["sBindId"]);
    }else{
        entity.sBindId = "";
    }
    if( ! pJson["sOrderNum"].isNull() ){
        entity.sOrderNum =  tConvert.Json2String(pJson["sOrderNum"]);
    }else{
        entity.sOrderNum = "";
    }
    if( ! pJson["sMerId"].isNull() ){
        entity.sMerId =  tConvert.Json2String(pJson["sMerId"]);
    }else{
        entity.sMerId = "";
    }
    if( ! pJson["lBandTime"].isNull() ){
        entity.lBandTime =  tConvert.Json2Long(pJson["lBandTime"]);
    }else{
        entity.lBandTime = 0;
    }

    BankCardEntity bankEntity;
    iRet = BankCardDAL::GetInstance()->GetEntity(entity.sPrimaryKey,bankEntity);
    if( iRet == 0){
        RetJsonMsg::SetRetMsg(-3,pResponse);
        return -3;
    }

    iRet = Verify(entity.sBankCard, entity.sName, entity.sCerId, sPhone);
    if( 0 != iRet){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    iRet = BankCardDAL::GetInstance()->IsExist(entity.sPrimaryKey);
    if( iRet == 0  ){
        SetDefault(bankEntity);
        BankCardDAL::GetInstance()->Update(entity.sPrimaryKey,bankEntity);
    }else {

        iRet = BankCardDAL::GetInstance()->Insert(entity);
        if ( 0 != iRet) {
            appendlog(TTDLogger::LOG_ERROR,"BankCardBLL::Insert error");
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            return iRet;
        }
    }
    pResponse["iRet"] = 0;
    return 0;
}
//删除(解绑银行卡)
int BankCardBLL::Delete( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    ZoneAgentEntity pEntity;
    if(pJson["sBankCard"].isNull() || pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sBankCard"]) + tConvert.Json2String( pJson["lUserId"]) ;

    long lCurrentTime = tConvert.GetCurrentTime();

    iRet = BankCardDAL::GetInstance()->Delete(pEntity.sPrimaryKey,lCurrentTime,lCurrentTime);
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"BankCardBLL::Delete  error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"] = 0;
    return 0;
}


//查询一行内容
int BankCardBLL::GetEntity(const  Json::Value  & pJson,
                           Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    string sCond;
    if(pJson["sBankCard"].isNull() || pJson["lUserId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    sCond = tConvert.Json2String( pJson["sBankCard"]) + tConvert.Json2String( pJson["lUserId"]) ;

    BankCardEntity entity;
    iRet = BankCardDAL::GetInstance()->GetEntity(sCond,entity);
    if( iRet == 0  ) {
        pResponse["entity"]["sBankCard"] = entity.sBankCard;
        pResponse["entity"]["iCerType"] = entity.iCerType;
        pResponse["entity"]["lUserId"] =tConvert.LongToStr( entity.lUserId);
        pResponse["entity"]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["entity"]["sBank"] = entity.sBank;
        pResponse["entity"]["sBranch"] =  entity.sBranch;
        pResponse["entity"]["sCerId"] = entity.sCerId;
        pResponse["entity"]["sName"] = entity.sName;
        pResponse["iRet"] = 0;
        return 0;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//查询数据-條件查詢，为控制流量，设为分批返回
int BankCardBLL::GetData(const  Json::Value  & pJson,
                         Json::Value  & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    BankCardQueryEntity pQueryEntity;

    if( pJson["iCount"].isNull()  || pJson["iIndex"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    if( !pJson["lUserId"].isNull()){
        pQueryEntity.lUserId =   tConvert.Json2Long(pJson["lUserId"]);
    }else{
        pQueryEntity.lUserId = -1;
    }


    int piIndex = tConvert.Json2Int( pJson["iIndex"]);
    int piCount = tConvert.Json2Int( pJson["iCount"]);
    BankCardList  plsTmp;
    int iTotalCount;
    iRet = BankCardDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    int iCount = 0;
    while( iCount < plsTmp.size()){
        BankCardEntity  & entity = plsTmp[iCount];
        pResponse["list"][iCount]["sBankCard"] = entity.sBankCard;
        pResponse["list"][iCount]["iCerType"] = entity.iCerType;
        pResponse["list"][iCount]["lUserId"] = tConvert.LongToStr( entity.lUserId);
        pResponse["list"][iCount]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        pResponse["list"][iCount]["sBank"] = entity.sBank;
        pResponse["list"][iCount]["sBranch"] =  entity.sBranch;
        pResponse["list"][iCount]["sCerId"] = entity.sCerId;
        pResponse["list"][iCount]["sName"] = entity.sName;

        iCount++;
    }
    if( plsTmp.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    //返回总个数
    pResponse["iTotal"]  = iTotalCount;
    pResponse["iRet"] =0;
    return 0;
}




void BankCardBLL::SetDefault( BankCardEntity &  entity)
{
    entity.iCerType = -1;
    entity.lBandTime = -1;
    entity.lUserId = -1;
    entity.sBank = "-1nochange";
    entity.sBindId = "-1nochange";
    entity.sBranch = "-1nochange";
    entity.sCerId = "-1nochange";
    entity.sCredit1 = "-1nochange";
    entity.sCredit2 = "-1nochange";
    entity.sCustomerNm = "-1nochange";
    entity.sMerId = "-1nochange";
    entity.sName = "-1nochange";
    entity.sOrderNum = "-1nochange";
    entity.sBankCard = "-1nochange";
}

//银行卡4要素验证(apix平台)
int BankCardBLL::Verify(const string & psBankCardNo, const string & psName, const string & psIdCardNo, const string & psPhone)
{
    TConvert tConvert;
    string sResponse;
    vector<string> sHeadVector;

    string sUrl = "http://v.apix.cn/apixcredit/idcheck/bankcard?type=bankcard_four";
    sUrl += "&bankcardno=" + psBankCardNo;
    sUrl += "&name=" + psName;
    sUrl += "&idcardno=" + psIdCardNo;
    sUrl += "&phone=" + psPhone;

    sHeadVector.push_back("apix-key: 8c1f907911bc41f5575ea098dd6fdf0b");
    sHeadVector.push_back("content-type: application/json");
    sHeadVector.push_back("accept: application/json");

    TCurl tCurl;
    int iRet = tCurl.Get(sUrl,sHeadVector, sResponse);
    if( 0 == iRet){
        Json::Value jsonReturn;
        Json::Reader reader;
        if (!reader.parse(sResponse, jsonReturn)){
            return -1;
        }
        //        code值说明：
        //        查询成功：
        //        0   验证信息一致
        //        1   验证信息不一致
        //        2   库中无此银行卡号
        return tConvert.Json2Int(jsonReturn["code"]) ;


    }
    return -1;

}
