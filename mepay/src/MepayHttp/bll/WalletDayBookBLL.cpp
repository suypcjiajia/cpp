//#include "WalletDayBookBLL.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <sstream>
//#include "log/Logger.h"
//#include "TFile.h"
//#include "TConvert.h"
//#include "dal/WalletDayBookDAL.h"
//#include "dal/WalletAccountDAL.h"
//#include "com/EvhtpSvr.h"
//#include "TBLL.h"
//#include "com/RetJsonMsg.hpp"
//using namespace std;
//WalletDayBookBLL* WalletDayBookBLL::mInstance = NULL;
//WalletDayBookBLL* WalletDayBookBLL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new WalletDayBookBLL;
//    }
//    return mInstance;
//}

//int WalletDayBookBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
//    int iRet = -1;
//    if( "AddData" == psMethod){

//        iRet = Insert(pJson,pResponse);

//    }else if("UpdateData" == psMethod ){

//        iRet =Update(pJson,pResponse);

//    }else if("DeleteData" == psMethod){

//        iRet =Delete(pJson,pResponse);

//    }else if( "GetEntityByNum" == psMethod){

//        iRet = GetEntityByNum(pJson,pResponse);

//    }else if( "GetData" == psMethod){

//        iRet = GetData(pJson,pResponse);

//    }else{
//        RetJsonMsg::SetRetMsg(-19,pResponse);
//    }
//    return iRet;
//}

////初始化
//int WalletDayBookBLL::Init()
//{
//    WalletDayBookDAL::GetInstance()->CreateTable();
//    return 0;
//}

////增加
//int WalletDayBookBLL::Insert( const  Json::Value  & pJson, Json::Value & pResponse  )
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["sAccount"].isNull()  || pJson["lUserId"].isNull()
//            || pJson["sOrderNum"].isNull()  || pJson["iDir"].isNull()
//            || pJson["lMoney"].isNull()  || pJson["iMoneyType"].isNull() || pJson["iDiscount"].isNull()  || pJson["sQRcode"].isNull()
//            || pJson["lPlatformId"].isNull() || pJson["sRemark"].isNull() || pJson["sBusinessRemark"].isNull() || pJson["iStep"].isNull()){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    WalletDayBookEntity entity;
//    entity.lUserId =  tConvert.Json2Long( pJson["lUserId"]);
//    entity.iState = 0;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    entity.lAddTime = lCurrentTime;
//    entity.lUpdateId = lCurrentTime;
//    entity.lUpTime = lCurrentTime;
//    entity.sPrimaryKey = tConvert.LongToStr(lCurrentTime) + tConvert.LongToStr(entity.lUserId);
//    entity.contentEntity.sAccount = tConvert.Json2String( pJson["sAccount"]);
//    if(!pJson["sPhone"].isNull() ){
//       entity.contentEntity.sPhone = tConvert.Json2String( pJson["sPhone"]);
//    }
//    entity.contentEntity.sSerialNum =entity.sPrimaryKey;
//    entity.contentEntity.sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    entity.contentEntity.iDir = tConvert.Json2Int(pJson["iDir"]);
//    entity.contentEntity.lMoney = tConvert.Json2Long( pJson["lMoney"]);
//    entity.contentEntity.iMoneyType = tConvert.Json2Int( pJson["iMoneyType"] );
//    entity.contentEntity.iDiscount = tConvert.Json2Int( pJson["iDiscount"]);
//    entity.contentEntity.sQRcode =  tConvert.Json2String(pJson["sQRcode"]);
//    entity.contentEntity.sRemark = tConvert.Json2String( pJson["sRemark"]);
//    entity.contentEntity.sBusinessRemark =  tConvert.Json2String(pJson["sBusinessRemark"]);
//    entity.contentEntity.lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
//    entity.contentEntity.iStep = tConvert.Json2Int( pJson["iStep"] );

//    //参数值检查
//    if( (entity.contentEntity.iMoneyType < CASHTYPE || entity.contentEntity.iMoneyType > SHOPTICKETTYPE)
//            || ( entity.contentEntity.iDir < SERIALDIRIN|| entity.contentEntity.iDir > SERIALDIROUT )){
//        RetJsonMsg::SetRetMsg(-2,pResponse);
//        return -2;
//    }
//    WalletAccountEntity accEntity;
//    iRet = WalletAccountDAL::GetInstance()->GetEntityByUseId(entity.lUserId, accEntity);
//    if( iRet != 0){
//        WalletAccountEntity Acntentity;
//        Acntentity.lUserId = entity.lUserId;
//        Acntentity.iState = 0;
//        long lCurrentTime = tConvert.GetCurrentTime();
//        Acntentity.lAddTime = lCurrentTime;
//        Acntentity.lUpdateId = lCurrentTime;
//        Acntentity.lUpTime = lCurrentTime;
//        Acntentity.sPrimaryKey = entity.sPrimaryKey;
//        Acntentity.contentEntity.sAccount = entity.sPrimaryKey;
//        Acntentity.contentEntity.sPhone = entity.contentEntity.sPhone;
//        Acntentity.contentEntity.sPassWord = "";
//        Acntentity.contentEntity.lCash = 0;
//        Acntentity.contentEntity.lBandCash = 0;
//        Acntentity.contentEntity.lShopTicket =0;
//        Acntentity.contentEntity.sAddress = "";
//        Acntentity.contentEntity.sBankCard ="";
//        Acntentity.contentEntity.lPushId = 0;
//        Acntentity.contentEntity.lPlatformId = 0;
//        Acntentity.contentEntity.sQuestion ="";
//        Acntentity.contentEntity.sAnswer = "";

//        iRet =  WalletAccountDAL::GetInstance()->Insert(Acntentity);
//        if ( 0 != iRet ) {
//            appendlog(TTDLogger::LOG_ERROR,"Add Acount err  lUserId=%ld",Acntentity.lUserId);
//            RetJsonMsg::SetRetMsg(iRet,pResponse);
//            return iRet;
//        }
//    }

////    long lMoney = entity.contentEntity.lMoney;
////    bool bDisErr = false;
////    if( entity.contentEntity.iDir == SERIALDIROUT){
////        if( entity.contentEntity.iMoneyType ==CASHTYPE ){
////           if( accEntity.contentEntity.lCash - lMoney < 0 ){
////               bDisErr = true;
////           }

////        }else  if( entity.contentEntity.iMoneyType ==BANDCASHTYPE ){
////            if( accEntity.contentEntity.lBandCash - lMoney < 0 ){
////                bDisErr = true;
////            }

////        }else  if( entity.contentEntity.iMoneyType ==SHOPTICKETTYPE ){
////            if( accEntity.contentEntity.lShopTicket - lMoney < 0 ){
////                bDisErr = true;
////            }
////        }
////    }
////    if( bDisErr){
////        RetJsonMsg::SetRetMsg(-18,pResponse);
////        return -18;
////    }


//    iRet =  WalletDayBookDAL::GetInstance()->Insert(entity);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletDayBookBLL::Insert  err lUserId=%ld ",entity.lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    pResponse["sPrimaryKey"] = entity.sPrimaryKey;

//    return 0;
//}
////删除
//int WalletDayBookBLL::Delete( const  Json::Value  & pJson, Json::Value & pResponse  )
//{
//    TConvert tConvert;
//    int iRet = -1;
//    WalletDayBookEntity pEntity;
//    if( pJson["sPrimaryKey"].isNull() || pJson["lUserId"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);
//    pEntity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lCurrentTime = tConvert.GetCurrentTime();
//    pEntity.lUpdateId = lCurrentTime;
//    pEntity.lUpTime = lCurrentTime;

//    iRet = WalletDayBookDAL::GetInstance()->Delete(pEntity.sPrimaryKey,lCurrentTime,lCurrentTime);
//    if ( 0 != iRet ) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletDayBookBLL::Delete err lUserId=%ld ",pEntity.lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    return 0;
//}

////修改
//int WalletDayBookBLL::Update( const  Json::Value  & pJson, Json::Value & pResponse )
//{
//    // ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    int iRet = -1;
//    WalletDayBookEntity pEntity;
//    if( pJson["sPrimaryKey"].isNull() || pJson["lUserId"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -1;
//    }
//    pEntity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    pEntity.iState = 0;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    pEntity.lAddTime = lCurrentTime;
//    pEntity.lUpdateId = lCurrentTime;
//    pEntity.lUpTime = lCurrentTime;
//    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sSerialNum"]);
//    pEntity.contentEntity.sAccount =tConvert.Json2String( pJson["sAccount"]);
//    pEntity.contentEntity.sPhone = tConvert.Json2String(pJson["sPhone"]);
//    pEntity.contentEntity.sSerialNum =tConvert.Json2String( pJson["sSerialNum"]);
//    pEntity.contentEntity.sOrderNum =tConvert.Json2String( pJson["sOrderNum"]);
//    pEntity.contentEntity.iDir =tConvert.Json2Int( pJson["iDir"]);
//    pEntity.contentEntity.lMoney =tConvert.Json2Long( pJson["lMoney"]);
//    pEntity.contentEntity.iMoneyType =tConvert.Json2Int( pJson["iMoneyType"]);
//    pEntity.contentEntity.iDiscount =tConvert.Json2Int( pJson["iDiscount"]);
//    pEntity.contentEntity.sQRcode =tConvert.Json2String( pJson["sQRcode"]);
//    pEntity.contentEntity.sRemark = tConvert.Json2String(pJson["sRemark"]);
//    pEntity.contentEntity.sBusinessRemark =tConvert.Json2String( pJson["sBusinessRemark"]);

//    WalletDayBookEntity tmpEntity;
//    iRet = WalletDayBookDAL::GetInstance()->GetEntityBySerialNum(pEntity.sPrimaryKey,tmpEntity);
//    if( iRet != 0){
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    iRet = WalletDayBookDAL::GetInstance()->Update(pEntity.sPrimaryKey,pEntity);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"DetailBLL::Update err lUserId=%ld ",pEntity.lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}


////查询一行内容
//int WalletDayBookBLL::GetEntityByNum(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["sSerialNum"].isNull()   ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -1;
//    }
//    string sNum = tConvert.Json2String( pJson["sSerialNum"]);
//    WalletDayBookEntity entity;
//    iRet = WalletDayBookDAL::GetInstance()->GetEntityBySerialNum(sNum,entity);
//    if( iRet == 0){
//        pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
//        pResponse["entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
//        pResponse["entity"]["lUpTime"] = tConvert.LongToStr( entity.lUpTime );
//        pResponse["entity"]["lUpdateId"] = tConvert.LongToStr(  entity.lUpdateId );
//        pResponse["entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId);
//        pResponse["entity"]["sAccount"] = entity.contentEntity.sAccount;
//        pResponse["entity"]["sPhone"] = entity.contentEntity.sPhone;
//        pResponse["entity"]["sSerialNum"] = entity.contentEntity.sSerialNum;
//        pResponse["entity"]["sOrderNum"] = entity.contentEntity.sOrderNum;
//        pResponse["entity"]["iDir"] =  entity.contentEntity.iDir;
//        pResponse["entity"]["lMoney"] = tConvert.LongToStr( entity.contentEntity.lMoney );
//        pResponse["entity"]["iMoneyType"] = entity.contentEntity.iMoneyType;
//        pResponse["entity"]["sQRcode"] = entity.contentEntity.sQRcode;
//        pResponse["entity"]["iDiscount"] = entity.contentEntity.iDiscount;
//        pResponse["entity"]["sRemark"] = entity.contentEntity.sRemark;
//        pResponse["iRet"] = 0;
//        return 0;
//    }
//    RetJsonMsg::SetRetMsg(iRet,pResponse);
//    return iRet;
//}


////查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//int WalletDayBookBLL::GetData(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    WalletDayBookQueryEntity pQueryEntity;
//    if( pJson["iIndex"].isNull() || pJson["iCount"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return iRet;
//    }
//    if( ! pJson["lUserId"].isNull()  ){
//        pQueryEntity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    }else{
//        pQueryEntity.lUserId = -1;
//    }
//    if( ! pJson["sOrderNum"].isNull()  ){
//        pQueryEntity.sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    }else{
//        pQueryEntity.sOrderNum = "";
//    }
//    if( ! pJson["iStep"].isNull()  ){
//        pQueryEntity.iStep = tConvert.Json2Int( pJson["iStep"]);
//    }else{
//        pQueryEntity.iStep = -1;
//    }
//    if( ! pJson["iMoneyType"].isNull()  ){
//        pQueryEntity.iMoneyType = tConvert.Json2Int( pJson["iMoneyType"]);
//    }else{
//        pQueryEntity.iMoneyType = -1;
//    }

//    int piIndex =  tConvert.Json2Int(pJson["iIndex"]);
//    int piCount =  tConvert.Json2Int(pJson["iCount"]);
//    WalletDayBookList  plsTmp;
//    int iTotalCount;
//    iRet = WalletDayBookDAL::GetInstance()->GetList(pQueryEntity,piIndex, piCount,plsTmp,iTotalCount);
//    if( iRet < 0)
//    {
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    int iCount = 0;
//    while( iCount < plsTmp.size()){
//        WalletDayBookEntity &  entity = plsTmp[iCount];
//        pResponse["list"][iCount ]["sPrimaryKey"] = entity.sPrimaryKey;
//        pResponse["list"][iCount ]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
//        pResponse["list"][iCount ]["lUserId"] = tConvert.LongToStr(entity.lUserId);
//        pResponse["list"][iCount ]["sAccount"] = entity.contentEntity.sAccount;
//        pResponse["list"][iCount ]["sPhone"] = entity.contentEntity.sPhone;
//        pResponse["list"][iCount ]["sSerialNum"] = entity.contentEntity.sSerialNum;
//        pResponse["list"][iCount ]["sOrderNum"] = entity.contentEntity.sOrderNum;
//        pResponse["list"][iCount ]["iDir"] =  entity.contentEntity.iDir;
//        pResponse["list"][iCount ]["lMoney"] = tConvert.LongToStr(entity.contentEntity.lMoney );
//        pResponse["list"][iCount ]["iMoneyType"] = entity.contentEntity.iMoneyType;
//        pResponse["list"][iCount ]["sQRcode"] = entity.contentEntity.sQRcode;
//        pResponse["list"][iCount ]["iDiscount"] = entity.contentEntity.iDiscount;
//        pResponse["list"][iCount ]["sRemark"] = entity.contentEntity.sRemark;
//        pResponse["list"][iCount ]["iStep"] = entity.contentEntity.iStep;
//        iCount++;
//    }
//    //返回总个数
//    pResponse["iRet"] = 0;
//    pResponse["iTotal"]  = iTotalCount;
//    if( plsTmp.size() <= 0){
//        pResponse["list"] ;
//        pResponse["list"].resize(0);
//    }
//    return 0;
//}


