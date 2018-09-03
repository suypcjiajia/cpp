//#include "WalletAccountBLL.h"
//#include "ErrDef.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <sstream>
//#include "log/Logger.h"
//#include "TFile.h"
//#include "TConvert.h"
//#include "dal/WalletAccountDAL.h"
//#include "dal/WalletDayBookDAL.h"
//#include "dal/PersonalAgentDAL.h"
//#include "bll/PersonalAgentBLL.h"
//#include "TBLL.h"
//#include "com/EvhtpSvr.h"
//#include "com/RetJsonMsg.hpp"
//#include "MyUtil.h"
//using namespace std;
//WalletAccountBLL* WalletAccountBLL::mInstance = NULL;
//WalletAccountBLL* WalletAccountBLL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new WalletAccountBLL;
//    }
//    return mInstance;
//}

//int WalletAccountBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
//    int iRet = -1;
//    if( "AddData" == psMethod){

//        iRet = Insert(pJson,pResponse);

//    }else if("UpdateData" == psMethod ){

//        iRet =Update(pJson,pResponse);

//    }else if("DeleteData" == psMethod){

//        iRet =Delete(pJson,pResponse);

//    }else if( "GetEntity" == psMethod){

//        iRet = GetEntity(pJson,pResponse);

//    }else if( "GetData" == psMethod){

//        iRet = GetData(pJson,pResponse);

//    }else if( "ModifyPwd" == psMethod){

//        iRet = ModifyPwd(pJson,pResponse);

//    }else if( "Login" == psMethod){

//        iRet = Login(pJson,pResponse);

//    }else if( "ResetPwd" == psMethod){

//        iRet = ResetPwd(pJson,pResponse);

//    }else if( "InCash" == psMethod){

//        iRet = InCash(pJson,pResponse);

//    }else if( "InBandCash" == psMethod){

//        iRet = InBandCash(pJson,pResponse);

//    }else if( "InShopTicket" == psMethod){

//        iRet = InShopTicket(pJson,pResponse);

//    }else if( "OutCash" == psMethod){

//        iRet = OutCash(pJson,pResponse);

//    }else if( "BackCash" == psMethod){

//        iRet = BackCash(pJson,pResponse);

//    }else if( "ThirdPartyPay" == psMethod){

//        iRet = ThirdPartyPay(pJson,pResponse);

//    }else if( "Pay" == psMethod){

//        iRet = Pay(pJson,pResponse);

//    }else if( "BackPay" == psMethod){

//        iRet = BackPay(pJson,pResponse);

//    }else if( "OkPay" == psMethod){

//        iRet = OkPay(pJson,pResponse);

//    }else{
//        RetJsonMsg::SetRetMsg(-19,pResponse);
//    }
//    return iRet;
//}

////初始化
//int WalletAccountBLL::Init()
//{
//    WalletAccountDAL::GetInstance()->CreateTable();
//    return 0;
//}

////现金充值
//int WalletAccountBLL::InCash(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  ||  pJson["sCard"].isNull() ||  pJson["sOrderNum"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    string sCard = tConvert.Json2String( pJson["sCard"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);

//    iRet = WalletAccountDAL::GetInstance()->InCash(lUserId,lValue,sCard,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::InCash  lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}
////绑定现金充值
//int WalletAccountBLL::InBandCash(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  ||  pJson["sCard"].isNull() ||  pJson["sOrderNum"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    string sCard = tConvert.Json2String( pJson["sCard"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);

//    iRet = WalletAccountDAL::GetInstance()->InBandCash(lUserId,lValue,sCard,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::InBandCash  Err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;

//}

////购物币充值
//int WalletAccountBLL::InShopTicket(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  ||  pJson["sCard"].isNull() ||  pJson["sOrderNum"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    string sCard = tConvert.Json2String( pJson["sCard"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);

//    iRet = WalletAccountDAL::GetInstance()->InShopTicket(lUserId,lValue,sCard,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::InShopTicket  Err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////现金提现
//int WalletAccountBLL::OutCash(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  ||  pJson["sCard"].isNull() ||  pJson["sOrderNum"].isNull() ){
//        RetJsonMsg::SetRetMsg( ERRPOSTPARAM ,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    string sCard = tConvert.Json2String( pJson["sCard"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    WalletAccountEntity entity;
//    iRet  = WalletAccountDAL::GetInstance()->GetEntityByUseId(lUserId,entity) ;
//    if(iRet != 0){
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OutCash  GetEntityByUseId lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    if( entity.contentEntity.lCash - lValue < 0){
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OutCash  too large lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(ERRDATAOPERATE,pResponse);
//        return ERRDATAOPERATE;
//    }

//    iRet = WalletAccountDAL::GetInstance()->OutCash(lUserId,lValue,sCard,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OutCash Err  lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;

//}

////现金提现退回
//int WalletAccountBLL::BackCash(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull()  ||  pJson["sRemark"].isNull() ||  pJson["sOrderNum"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    string sCard = tConvert.Json2String( pJson["sRemark"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);

//    iRet = WalletAccountDAL::GetInstance()->BackCash(lUserId,sCard,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::BackCash Err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    return 0;

//}

////第三方支付
//int WalletAccountBLL::ThirdPartyPay(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  ||  pJson["lPlatformId"].isNull() ||  pJson["sOrderNum"].isNull() || pJson["sRemark"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    string sRemark = tConvert.Json2String( pJson["sRemark"]);

//    iRet = WalletAccountDAL::GetInstance()->ThirdPartyPay(lUserId,lValue,lPlatformId,sRemark,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::ThirdPartyPay err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

//// 钱包支付
//int WalletAccountBLL::Pay(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() || pJson["lValue"].isNull()  || pJson["iPayType"].isNull()  ||  pJson["lPlatformId"].isNull()
//            ||  pJson["sOrderNum"].isNull() || pJson["sRemark"].isNull()  || pJson["iDivided"].isNull()){
//        RetJsonMsg::SetRetMsg( ERRPOSTPARAM , pResponse);
//        return ERRPOSTPARAM;
//    }
//    int iPayType = tConvert.Json2Int(pJson["iPayType"]);
//    if( iPayType < 1){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM,pResponse);
//        return ERRPOSTPARAM;
//    }
//    if( iPayType == PAYTYPESTC || iPayType == PAYTYPESTB){
//        if (pJson["lValue2"].isNull() ){
//            RetJsonMsg::SetRetMsg( ERRPOSTPARAM,pResponse);
//            return ERRPOSTPARAM;
//        }
//    }

//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    long lValue = tConvert.Json2Long( pJson["lValue"]);
//    long lValue2 = tConvert.Json2Long( pJson["lValue2"]);
//    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    string sRemark = tConvert.Json2String( pJson["sRemark"]);
//    int iDivided = tConvert.Json2Int( pJson["iDivided"] );

//    iRet = WalletAccountDAL::GetInstance()->Pay(lUserId,lValue,lValue2,iPayType,lPlatformId,sRemark,sOrderNum ,iDivided);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::Pay  lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;

//}

//// 退款
//int WalletAccountBLL::BackPay(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() ||  pJson["sOrderNum"].isNull() || pJson["sRemark"].isNull() ){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    string sRemark = tConvert.Json2String( pJson["sRemark"]);

//    iRet = WalletAccountDAL::GetInstance()->BackPay(lUserId,sRemark,sOrderNum);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::BackPay  lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg( iRet ,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    return 0;

//}

//// 确认支付
//int WalletAccountBLL::OkPay(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    int iRet = -1;
//    if( pJson["lUserId"].isNull() ||  pJson["sOrderNum"].isNull()  || pJson["sPassWord"].isNull()  || pJson["lCost"].isNull()){
//        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
//        return ERRPOSTPARAM;
//    }
//    TConvert tConvert;
//    long lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"]);
//    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);
//    long lCost = tConvert.Json2Long( pJson["lCost"]);

//    WalletAccountEntity accountEntity;
//    iRet = WalletAccountDAL::GetInstance()->GetEntityByUseId( lUserId,accountEntity);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OkPay GetEntityByUseId err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    //验证密码
//    if( accountEntity.contentEntity.sPassWord != sPassWord){
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OkPay   PassWord err lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(-6,pResponse);
//        return -1;
//    }

//    iRet = WalletAccountDAL::GetInstance()->OkPay(lUserId,sOrderNum,lCost);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::OkPay  lUserId=%ld ",lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////增加
//int WalletAccountBLL::Insert(const  Json::Value  & pJson, Json::Value & pResponse )
//{
//    //  ScopeLock lock( this->mMutex );
//    int iRet =-1;
//    TConvert tConvert;
//    if( pJson["sAccount"].isNull()   || pJson["lUserId"].isNull()
//            || pJson["sPassWord"].isNull()  || pJson["lCash"].isNull()  || pJson["lBandCash"].isNull()
//            || pJson["lShopTicket"].isNull()  || pJson["sAddress"].isNull() || pJson["sBankCard"].isNull()  || pJson["lPushId"].isNull()
//            || pJson["lPlatformId"].isNull() || pJson["sQuestion"].isNull()  || pJson["sAnswer"].isNull()){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }

//    WalletAccountEntity entity;
//    entity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
//    entity.iState = 0;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    entity.lAddTime = lCurrentTime;
//    entity.lUpdateId = lCurrentTime;
//    entity.lUpTime = lCurrentTime;
//    entity.sPrimaryKey = tConvert.Json2String(pJson["sAccount"]);
//    entity.contentEntity.sAccount = tConvert.Json2String(pJson["sAccount"]);
//    if(! pJson["sPhone"].isNull() ){
//        entity.contentEntity.sPhone =tConvert.Json2String( pJson["sPhone"]);
//    }else{
//        entity.contentEntity.sPhone = "";
//    }
//    entity.contentEntity.sPassWord = tConvert.Json2String(pJson["sPassWord"]);
//    entity.contentEntity.lCash = tConvert.Json2Int(pJson["lCash"]);
//    entity.contentEntity.lBandCash = tConvert.Json2Long(pJson["lBandCash"]);
//    entity.contentEntity.lShopTicket =tConvert.Json2Long( pJson["lShopTicket"]);
//    entity.contentEntity.sAddress = tConvert.Json2String(pJson["sAddress"]);
//    entity.contentEntity.sBankCard =tConvert.Json2String( pJson["sBankCard"]);
//    entity.contentEntity.lPushId =tConvert.Json2Long( pJson["lPushId"]);
//    entity.contentEntity.lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
//    entity.contentEntity.sQuestion =tConvert.Json2String( pJson["sQuestion"]);
//    entity.contentEntity.sAnswer = tConvert.Json2String(pJson["sAnswer"]);

//    iRet =  WalletAccountDAL::GetInstance()->Insert(entity);
//    if ( 0 != iRet ) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::Insert  lUserId=%ld",entity.lUserId);
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    pResponse["sPrimaryKey"] = entity.sPrimaryKey;

//    return 0;
//}
////删除
//int WalletAccountBLL::Delete( const  Json::Value  & pJson  ,Json::Value & pResponse )
//{
//    //  ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    int iRet =-1;
//    WalletAccountEntity pEntity;
//    if( pJson["sPrimaryKey"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    pEntity.sPrimaryKey = tConvert.Json2String(pJson["sPrimaryKey"]);
//    long lCurrentTime = tConvert.GetCurrentTime();
//    pEntity.lUpdateId = lCurrentTime;
//    pEntity.lUpTime = lCurrentTime;

//    iRet = WalletAccountDAL::GetInstance()->Delete(pEntity.sPrimaryKey,pEntity.lUpTime,pEntity.lUpTime);
//    if ( 0 != iRet ) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::Delete Account=%s ",pEntity.sPrimaryKey.c_str());
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    pResponse["iRet"] = 0;
//    return 0;
//}

////修改
//int WalletAccountBLL::Update( const  Json::Value  & pJson  ,Json::Value & pResponse )
//{
//    TConvert tConvert;
//    int iRet = -1;
//    WalletAccountEntity entity;
//    SetDefault(entity);
//    if( pJson["sPrimaryKey"].isNull()   ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }

//    entity.sPrimaryKey = tConvert.Json2String(  pJson["sPrimaryKey"]);
//    entity.contentEntity.sAccount = entity.sPrimaryKey;

//    if( !pJson["sPhone"].isNull()   ){
//        entity.contentEntity.sPhone = tConvert.Json2String(  pJson["sPhone"]);
//    }
//    if( !pJson["lCash"].isNull()   ){
//        entity.contentEntity.lCash = tConvert.Json2Long(  pJson["lCash"]);
//    }
//    if( !pJson["lBandCash"].isNull()   ){
//        entity.contentEntity.lBandCash = tConvert.Json2Long(  pJson["lBandCash"]);
//    }
//    if( !pJson["lShopTicket"].isNull()   ){
//        entity.contentEntity.lShopTicket = tConvert.Json2Long(  pJson["lShopTicket"]);
//    }
//    if( !pJson["sAddress"].isNull()   ){
//        entity.contentEntity.sAddress = tConvert.Json2String(  pJson["sAddress"]);
//    }
//    if( !pJson["sBankCard"].isNull()   ){
//        entity.contentEntity.sBankCard = tConvert.Json2String(  pJson["sBankCard"]);
//    }
//    if( !pJson["lPushId"].isNull()   ){
//        entity.contentEntity.lPushId = tConvert.Json2Long(  pJson["lPushId"]);
//    }
//    if( !pJson["lPlatformId"].isNull()   ){
//        entity.contentEntity.lPlatformId = tConvert.Json2Long(  pJson["lPlatformId"]);
//    }
//    if( !pJson["sQuestion"].isNull()   ){
//        entity.contentEntity.sQuestion = tConvert.Json2String(  pJson["sQuestion"]);
//    }
//    if( !pJson["sAnswer"].isNull()   ){
//        entity.contentEntity.sAnswer = tConvert.Json2String(  pJson["sAnswer"]);
//    }
//    if( !pJson["sPassWord"].isNull()   ){
//        entity.contentEntity.sPassWord = tConvert.Json2String(  pJson["sPassWord"]);
//    }



//    entity.iState = 0;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    entity.lAddTime = lCurrentTime;
//    entity.lUpdateId = lCurrentTime;
//    entity.lUpTime = lCurrentTime;

//    WalletAccountEntity tmpEntity;
//    iRet = WalletAccountDAL::GetInstance()->GetEntityByKey(entity.sPrimaryKey,tmpEntity);
//    if( iRet != 0){
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    iRet = WalletAccountDAL::GetInstance()->Update(entity.sPrimaryKey,entity);
//    if ( 0 != iRet ) {
//        appendlog(TTDLogger::LOG_ERROR,"WalletAccountBLL::Update sKey=%s ",entity.sPrimaryKey.c_str());
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////个代付费
//int WalletAccountBLL::PerAgentPay(const  Json::Value  & pJson, Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if(pJson["lValue"].isNull()  || pJson["iPayType"].isNull() || pJson["lUserId"].isNull()  || pJson["sOrderNum"].isNull()
//            || pJson["sCity"].isNull() || pJson["lPushId"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }

//    int iPayType = tConvert.Json2Int(pJson["iPayType"]);

//    if( iPayType != PAYTYPECASH && iPayType != PAYTYPEBANDCASH){
//        RetJsonMsg::SetRetMsg( -2,pResponse);
//        return -2;
//    }
//    long lValue = tConvert.Json2Long( pJson["lValue"] );
//    long lUserId = tConvert.Json2Long( pJson["lUserId"] ) ;
//    long lPushId = tConvert.Json2Long( pJson["lPushId"] ) ;
//    string sOrderNum = tConvert.Json2String( pJson["sOrderNum"] ) ;
//    string sCity = tConvert.Json2String( pJson["sCity"] ) ;
//    iRet = WalletAccountDAL::GetInstance()->PerAgentPay( lUserId, lValue,iPayType,sOrderNum,sCity,lPushId);
//    if( 0 != iRet){
//        RetJsonMsg::SetRetMsg( iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////查询一行内容(APP)
//int WalletAccountBLL::GetEntity(const  Json::Value  & pJson  ,Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    if( pJson["sCond"].isNull() ||  pJson["iType"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    int iType = tConvert.Json2Int( pJson["iType"] );
//    string sCond = tConvert.Json2String( pJson["sCond"] );
//    WalletAccountEntity  entity;
//    iRet = WalletAccountDAL::GetInstance()->GetEntity(sCond,iType,entity);
//    if( 0 ==  iRet){
//        pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
//        pResponse["entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
//        pResponse["entity"]["lUpTime"] =tConvert.LongToStr(  entity.lUpTime);
//        pResponse["entity"]["lUpdateId"] = tConvert.LongToStr( entity.lUpdateId);
//        pResponse["entity"]["lUserId"] =tConvert.LongToStr(  entity.lUserId);
//        pResponse["entity"]["sAccount"] = entity.contentEntity.sAccount;
//        pResponse["entity"]["sPhone"] = entity.contentEntity.sPhone;
//        pResponse["entity"]["lCash"] =tConvert.LongToStr(  entity.contentEntity.lCash);
//        pResponse["entity"]["lBandCash"] = tConvert.LongToStr( entity.contentEntity.lBandCash);
//        pResponse["entity"]["lShopTicket"] =tConvert.LongToStr(  entity.contentEntity.lShopTicket);
//        pResponse["entity"]["sAddress"] = entity.contentEntity.sAddress;
//        pResponse["entity"]["lPushId"] =tConvert.LongToStr( entity.contentEntity.lPushId);
//        pResponse["entity"]["lPlatformId"] =tConvert.LongToStr(  entity.contentEntity.lPlatformId);
//        pResponse["entity"]["sPassWord"] =  entity.contentEntity.sPassWord;
//        pResponse["iRet"] = 0;
//        return 0;
//    }

//    RetJsonMsg::SetRetMsg(iRet,pResponse);
//    return iRet;
//}

////查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//int WalletAccountBLL::GetData(const  Json::Value  & pJson  ,Json::Value & pResponse )
//{
//    TConvert tConvert;
//    WalletAccountQueryEntity pQueryEntity;
//    if( pJson["iIndex"].isNull() || pJson["iCount"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    if( ! pJson["sAccount"].isNull()){
//        pQueryEntity.sAccount =  tConvert.Json2String( pJson["sAccount"]);
//    }
//    else{
//        pQueryEntity.sAccount = "";
//    }
//    if( ! pJson["sPhone"].isNull()){
//        pQueryEntity.sPhone = tConvert.Json2String(pJson["sPhone"]);
//    }else{
//        pQueryEntity.sPhone = "";
//    }
//    int piIndex =  tConvert.Json2Int( pJson["iIndex"] );
//    int piCount =   tConvert.Json2Int( pJson["iCount"] );
//    WalletAccountList  plsTmp;
//    int iTotalCount;
//    if ( 0 == WalletAccountDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount)){
//        int iCount = 0;
//        while(iCount <  plsTmp.size()){
//            WalletAccountEntity  & entity = plsTmp[iCount];
//            pResponse["list"][iCount]["sPrimaryKey"] = entity.sPrimaryKey;
//            pResponse["list"][iCount]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
//            pResponse["list"][iCount]["lUpTime"] =tConvert.LongToStr(  entity.lUpTime);
//            pResponse["list"][iCount]["lUpdateId"] = tConvert.LongToStr( entity.lUpdateId);
//            pResponse["list"][iCount]["lUserId"] =tConvert.LongToStr(  entity.lUserId);
//            pResponse["list"][iCount]["sAccount"] = entity.contentEntity.sAccount;
//            pResponse["list"][iCount]["sPhone"] = entity.contentEntity.sPhone;
//            pResponse["list"][iCount]["lCash"] =tConvert.LongToStr(  entity.contentEntity.lCash);
//            pResponse["list"][iCount]["lBandCash"] = tConvert.LongToStr( entity.contentEntity.lBandCash);
//            pResponse["list"][iCount]["lShopTicket"] =tConvert.LongToStr(  entity.contentEntity.lShopTicket);
//            pResponse["list"][iCount]["sAddress"] = entity.contentEntity.sAddress;
//            pResponse["list"][iCount]["lPushId"] =tConvert.LongToStr( entity.contentEntity.lPushId);
//            pResponse["list"][iCount]["lPlatformId"] =tConvert.LongToStr(  entity.contentEntity.lPlatformId);
//            pResponse["list"][iCount]["sPassWord"] =  entity.contentEntity.sPassWord;
//            iCount++;
//        }

//    }

//    //返回总个数
//    pResponse["iRet"] =  0;
//    pResponse["iTotal"]  = iTotalCount;
//    if( plsTmp.size() <= 0){
//        pResponse["list"] ;
//        pResponse["list"].resize(0);
//    }
//    return 0;
//}


//int WalletAccountBLL::ModifyPwd(const  Json::Value  & pJson  ,Json::Value & pResponse){
//    //ScopeLock lock( this->mMutex );
//    TConvert tConvert;
//    int iRet = -1;
//    if(pJson["sPrimaryKey"].isNull()  || pJson["sOldPwd"].isNull()  || pJson["sNewPwd"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    string sPrimaryKey = tConvert.Json2String(  pJson["sPrimaryKey"]);
//    string sOldPwd  =  tConvert.Json2String(pJson["sOldPwd"]);
//    string sNewPwd =  tConvert.Json2String(pJson["sNewPwd"]);
//    //新密码不能为空
//    if( sNewPwd.empty() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    long lUpdateId = tConvert.GetCurrentTime();
//    WalletAccountEntity entity;
//    WalletAccountDAL::GetInstance()->GetEntityByUseId(tConvert.StrToLong(sPrimaryKey),entity);
//    //旧密码不正确
//    if( sOldPwd != entity.contentEntity.sPassWord){
//        RetJsonMsg::SetRetMsg(-6,pResponse);
//        return iRet;
//    }
//    if ( 0 != WalletAccountDAL::GetInstance()->ModifyPwd( sPrimaryKey,sNewPwd,lUpdateId ) ) {
//        RetJsonMsg::SetRetMsg(-4,pResponse);
//        return iRet;
//    }
//    //第一次修改密码就触发全民推广收益
//    if( sOldPwd == ""){
//        //   AllPush(entity.contentEntity.lPushId,entity.contentEntity.lPlatformId);
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////登陆
//int WalletAccountBLL::Login( const  Json::Value  & pJson  ,Json::Value & pResponse){
//    TConvert tConvert;
//    int iRet = -1;
//    if(pJson["sAccount"].isNull()  || pJson["sPassWord"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return iRet;
//    }
//    string sAccount =  tConvert.Json2String( pJson["sAccount"]);
//    string sPassWord  = tConvert.Json2String( pJson["sPassWord"]);
//    //密码不能为空
//    if( sPassWord.empty() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    WalletAccountEntity entity;
//    WalletAccountDAL::GetInstance()->GetEntityByUseId(tConvert.StrToLong(sAccount),entity);
//    if( sPassWord != entity.contentEntity.sPassWord){
//        RetJsonMsg::SetRetMsg(-6,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return iRet;
//}
////重置密码
//int WalletAccountBLL::ResetPwd(  const  Json::Value  & pJson  ,Json::Value & pResponse){
//    TConvert tConvert;
//    int iRet = -1;
//    if(pJson["sPrimaryKey"].isNull()  || pJson["sNewPwd"].isNull()  || pJson["sQuestion"].isNull()  || pJson["sAnswer"].isNull()){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return iRet;
//    }
//    long lUpdateId = tConvert.GetCurrentTime();
//    string sPrimaryKey =  tConvert.Json2String( pJson["sPrimaryKey"]);
//    string sNewPwd  = tConvert.Json2String( pJson["sNewPwd"]);
//    string sQuestion =  tConvert.Json2String( pJson["sQuestion"]);
//    string sAnswer  = tConvert.Json2String( pJson["sAnswer"]);
//    //新密码不能为空
//    if( sNewPwd.empty() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }

//    WalletAccountEntity entity;
//    WalletAccountDAL::GetInstance()->GetEntityByUseId(tConvert.StrToLong(sPrimaryKey),entity);
//    if(  sQuestion != entity.contentEntity.sQuestion && sAnswer != entity.contentEntity.sAnswer){
//        RetJsonMsg::SetRetMsg(-6,pResponse);
//        return iRet;
//    }
//    if ( 0 != WalletAccountDAL::GetInstance()->ModifyPwd( sPrimaryKey,sNewPwd,lUpdateId ) ) {
//        RetJsonMsg::SetRetMsg(-4,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return iRet;

//}

////全民推广
//int WalletAccountBLL::AllPush(  const long & plPushId, const long & plPlatformId )
//{
//    string  sUUID = MyUtil::GetUUID();
//    return WalletAccountDAL::GetInstance()->AllPush(plPushId,plPlatformId,sUUID);
//}

//void WalletAccountBLL::SetDefault(WalletAccountEntity & entity)
//{
//    entity.contentEntity.lBandCash = -1;
//    entity.contentEntity.lCash = -1;
//    entity.contentEntity.lPlatformId = -1;
//    entity.contentEntity.lPushId = -1;
//    entity.contentEntity.lShopTicket = -1;
//    entity.contentEntity.sAccount = "-1nochange";
//    entity.contentEntity.sAddress = "-1nochange";
//    entity.contentEntity.sAnswer = "-1nochange";
//    entity.contentEntity.sBankCard = "-1nochange";
//    entity.contentEntity.sPassWord = "-1nochange";
//    entity.contentEntity.sPhone = "-1nochange";
//    entity.contentEntity.sQuestion = "-1nochange";
//}
