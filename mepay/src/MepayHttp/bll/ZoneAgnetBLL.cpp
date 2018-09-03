#include "ZoneAgnetBLL.h"
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/ZoneAgentDAL.h"
#include "TBLL.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
using namespace std;
ZoneAgentBLL* ZoneAgentBLL::mInstance = NULL;
ZoneAgentBLL* ZoneAgentBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new ZoneAgentBLL;
    }
    return mInstance;
}

int ZoneAgentBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "AddData" == psMethod){

        iRet = Insert(pJson,pResponse);

    }else if("UpdateData" == psMethod ){

        iRet =Update(pJson,pResponse);

    }else if("DeleteData" == psMethod){

        iRet =Delete(pJson,pResponse);

    }else if( "GetEntity" == psMethod){

        iRet = GetEntity(pJson,pResponse);

    }else if( "GetData" == psMethod){

        iRet = GetData(pJson,pResponse);

    }else if( "ModifyPwd" == psMethod){

        iRet = ModifyPwd(pJson,pResponse);

    }else if( "Login" == psMethod){

        iRet = Login(pJson,pResponse);

    }else if( "ResetPwd" == psMethod){

        iRet = ResetPwd(pJson,pResponse);

    }else{
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}


//处理带token，不加密的情况
int ZoneAgentBLL::Process2( EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;

    string sUrl = pEvhtp->GetUrl();


    if( sUrl.find( "/zoneAgent/getZoneByCity") != string::npos ){

        iRet = GetZoneByCity(pJson,pResponse);

    }
    else if( sUrl.find( "/zoneAgent/isOpen") != string::npos ){

        TConvert tConvert;
        string zone = pEvhtp->GetQuery("zone");
        zone = tConvert.URLDecode(zone);

        Json::Value jIn;
        jIn["zone"] = zone;

        iRet = IsOpen(jIn,pResponse);

    }
    return iRet;
}


//初始化
int ZoneAgentBLL::Init()
{
    ZoneAgentDAL::GetInstance()->CreateTable();
    return 0;
}

//增加
int ZoneAgentBLL::Insert( const  Json::Value  & pJson, Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;
    ZoneAgentEntity entity;
    entity.iState = 0;
    long lCurrentTime = tConvert.GetCurrentTime();
    entity.lAddTime = lCurrentTime;
    entity.lUpdateId = lCurrentTime;
    entity.lUpTime = lCurrentTime;

    if( pJson["sAccount"].isNull() || pJson["lUserId"].isNull()   || pJson["sPassWord"].isNull()
            || pJson["sName"].isNull()  || pJson["sUnit"].isNull()  || pJson["sID"].isNull()
            || pJson["sCity"].isNull()   ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    entity.lUserId =  tConvert.Json2Long( pJson["lUserId"]) ;
    entity.contentEntity.sAccount =  tConvert.Json2String( pJson["sAccount"]) ;
    entity.sPrimaryKey = entity.contentEntity.sAccount;
    //   entity.sPrimaryKey = tConvert.LongToStr(tConvert.GetCurrentTime()) + tConvert.IntToStr(pthread_self());
    //entity.contentEntity.sAccount =  entity.sPrimaryKey;
    if( ! pJson["sPhone"].isNull() ){
        entity.contentEntity.sPhone =  tConvert.Json2String(pJson["sPhone"]);
    }else{
        entity.contentEntity.sPhone = "";
    }
    entity.contentEntity.sPassWord =  tConvert.Json2String( pJson["sPassWord"]);
    entity.contentEntity.sName =tConvert.Json2String( pJson["sName"]);
    entity.contentEntity.sUnit = tConvert.Json2String( pJson["sUnit"]);
    entity.contentEntity.sID = tConvert.Json2String(pJson["sID"]);
    entity.contentEntity.sCity = tConvert.Json2String( pJson["sCity"]);
    if( ! pJson["sRemark"].isNull() ){
        entity.contentEntity.sRemark =  tConvert.Json2String(pJson["sRemark"]);
    }else{
        entity.contentEntity.sRemark = "";
    }

    iRet = ZoneAgentDAL::GetInstance()->Insert(entity);
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"ZoneAgentBLL::Insert error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"] = 0;
    pResponse["sPrimaryKey"] = entity.sPrimaryKey;
    return 0;
}
//删除
int ZoneAgentBLL::Delete( const  Json::Value  & pJson  ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    ZoneAgentEntity pEntity;
    if(pJson["sPrimaryKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);

    long lCurrentTime = tConvert.GetCurrentTime();

    iRet = ZoneAgentDAL::GetInstance()->Delete(pEntity.sPrimaryKey,lCurrentTime,lCurrentTime);
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"ZoneAgentBLL::Delete  error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"] = 0;
    return 0;
}

//修改
int ZoneAgentBLL::Update( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    ZoneAgentEntity pEntity;
    SetDefault(pEntity);
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    pEntity.lUpdateId = lCurrentTime;
    pEntity.lUpTime = lCurrentTime;
    int iRet =-1;
    if(pJson["sPrimaryKey"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);
    ZoneAgentEntity zoneEntity;
    iRet = ZoneAgentDAL::GetInstance()->GetEntityByKey( pEntity.sPrimaryKey,1,zoneEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    if( !pJson["sAccount"].isNull()){
        pEntity.contentEntity.sAccount = tConvert.Json2String( pJson["sAccount"]);
    }
    if( !pJson["sPhone"].isNull()){
        pEntity.contentEntity.sPhone =  tConvert.Json2String(pJson["sPhone"]);
    }
    if( !pJson["sPassWord"].isNull()){
        pEntity.contentEntity.sPassWord =  tConvert.Json2String(pJson["sPassWord"]);
    }
    if( !pJson["sName"].isNull()){
        pEntity.contentEntity.sName = tConvert.Json2String( pJson["sName"]);
    }
    if( !pJson["sUnit"].isNull()){
        pEntity.contentEntity.sUnit = tConvert.Json2String( pJson["sUnit"]);
    }
    if( !pJson["sID"].isNull()){
        pEntity.contentEntity.sID = tConvert.Json2String( pJson["sID"]);
    }
    if( !pJson["sCity"].isNull()){
        pEntity.contentEntity.sCity =  tConvert.Json2String(pJson["sCity"]);
    }
    if( !pJson["sRemark"].isNull()){
        pEntity.contentEntity.sRemark = tConvert.Json2String( pJson["sRemark"]);
    }

    iRet = ZoneAgentDAL::GetInstance()->Update(pEntity.sPrimaryKey,pEntity);
    if ( 0 != iRet ) {
        appendlog(TTDLogger::LOG_ERROR,"ZoneAgentBLL::Update error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"] = 0;
    return 0;
}
int ZoneAgentBLL::IsOpen(const  Json::Value  & pJson,
                         Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    if(pJson["zone"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return iRet;
    }


    string zone =  tConvert.Json2String(pJson["zone"]);
    ZoneAgentEntity entity;
    iRet = ZoneAgentDAL::GetInstance()->LikeKey(zone,entity);
    bool bOpen = false;
    if( iRet == 0  ) {
        bOpen = true;
    }
    pResponse["result"]["open"] = bOpen;
    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//查询一行内容(APP)
int ZoneAgentBLL::GetEntity(const  Json::Value  & pJson,
                            Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    if(pJson["sCond"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return iRet;
    }
    int iType = 1;
    if( !pJson["iType"].isNull()){
        iType = tConvert.Json2Int( pJson["iType"]);
    }

    string sCond =  tConvert.Json2String(pJson["sCond"]);
    ZoneAgentEntity entity;
    iRet = ZoneAgentDAL::GetInstance()->GetEntityByKey(sCond,iType,entity);
    if( iRet == 0  ) {
        pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
        pResponse["entity"]["sAccount"] = entity.contentEntity.sAccount;
        pResponse["entity"]["sPhone"] = entity.contentEntity.sPhone;
        pResponse["entity"]["sPassWord"] = entity.contentEntity.sPassWord;
        pResponse["entity"]["sName"] = entity.contentEntity.sName;
        pResponse["entity"]["sUnit"] =  entity.contentEntity.sUnit;
        pResponse["entity"]["sID"] = entity.contentEntity.sID;
        pResponse["entity"]["sCity"] = entity.contentEntity.sCity;
        pResponse["entity"]["sRemark"] = entity.contentEntity.sRemark;
        pResponse["entity"]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
        pResponse["entity"]["lUpTime"] = tConvert.LongToStr( entity.lUpTime );
        pResponse["entity"]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["iRet"] = 0;
        return 0;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

int ZoneAgentBLL::GetZoneByCity(const  Json::Value  & pJson,
                                Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;
    if(pJson["city"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return iRet;
    }

    pResponse["result"] ;
    pResponse["result"].resize(0);

    string city =  tConvert.Json2String(pJson["city"]);

    ZoneAgentList  plsTmp;

    ZoneAgentDAL::GetInstance()->GetListByCity(city,plsTmp);

    int iCount = 0;
    while( iCount < plsTmp.size()){
        ZoneAgentEntity  & entity = plsTmp[iCount];

        pResponse["result"][iCount]["zone"] = entity.contentEntity.sCity;
        iCount++;
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;



}

//查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
int ZoneAgentBLL::GetData(const  Json::Value  & pJson,
                          Json::Value  & pResponse)
{
    TConvert tConvert;
    ZoneAgentQueryEntity pQueryEntity;

    if( pJson["iCount"].isNull()  || pJson["iIndex"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    if( !pJson["sAccount"].isNull()){
        pQueryEntity.sAccount =   tConvert.Json2String(pJson["sAccount"]);
    }else{
        pQueryEntity.sAccount.clear();
    }
    if( !pJson["sCity"].isNull()){
        pQueryEntity.sCity = tConvert.Json2String( pJson["sCity"]);
    }else{
        pQueryEntity.sCity.clear();
    }
    if( !pJson["sID"].isNull()){
        pQueryEntity.sID = tConvert.Json2String( pJson["sID"]);
    }else{
        pQueryEntity.sID.clear();
    }
    if( !pJson["sName"].isNull()){
        pQueryEntity.sName = tConvert.Json2String( pJson["sName"]);
    }else{
        pQueryEntity.sName.clear();
    }
    if( !pJson["sPhone"].isNull()){
        pQueryEntity.sPhone =  tConvert.Json2String(pJson["sPhone"]);
    }else{
        pQueryEntity.sPhone.clear();
    }
    if( !pJson["sRemark"].isNull()){
        pQueryEntity.sRemark = tConvert.Json2String( pJson["sRemark"]);
    }else{
        pQueryEntity.sRemark.clear();
    }
    if( !pJson["sUnit"].isNull()){
        pQueryEntity.sUnit = tConvert.Json2String( pJson["sUnit"]);
    }else{
        pQueryEntity.sUnit.clear();
    }
    if( !pJson["lUserId"].isNull()){
        pQueryEntity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
    }else{
        pQueryEntity.lUserId = -1;
    }
    int piIndex = tConvert.Json2Int( pJson["iIndex"]);
    int piCount = tConvert.Json2Int( pJson["iCount"]);
    ZoneAgentList  plsTmp;
    int iTotalCount;
    int iTotal = ZoneAgentDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

    int iCount = 0;
    while( iCount < plsTmp.size()){
        ZoneAgentEntity  & entity = plsTmp[iCount];
        pResponse["list"][iCount]["sPrimaryKey"] = entity.sPrimaryKey;
        pResponse["list"][iCount]["sAccount"] = entity.contentEntity.sAccount;
        pResponse["list"][iCount]["sPhone"] = entity.contentEntity.sPhone;
        pResponse["list"][iCount]["sPassWord"] = entity.contentEntity.sPassWord;
        pResponse["list"][iCount]["sName"] = entity.contentEntity.sName;
        pResponse["list"][iCount]["sUnit"] =  entity.contentEntity.sUnit;
        pResponse["list"][iCount]["sID"] = entity.contentEntity.sID;
        pResponse["list"][iCount]["sCity"] = entity.contentEntity.sCity;
        pResponse["list"][iCount]["sRemark"] = entity.contentEntity.sRemark;
        pResponse["list"][iCount]["lAddTime"] = tConvert.LongToStr( entity.lAddTime);
        pResponse["list"][iCount]["lUpTime"] = tConvert.LongToStr( entity.lUpTime );
        pResponse["list"][iCount]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        iCount++;
    }
    if( plsTmp.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    //返回总个数
    pResponse["iTotal"]  = iTotalCount;
    pResponse["iRet"] =iTotal;
    return 0;
}

int ZoneAgentBLL::ModifyPwd(const  Json::Value  & pJson ,Json::Value & pResponse){
    TConvert tConvert;
    int iRet = -1;
    if(pJson["sPrimaryKey"].isNull()  || pJson["sOldPwd"].isNull()  || pJson["sNewPwd"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return iRet;
    }
    string sPrimaryKey =  tConvert.Json2String( pJson["sPrimaryKey"]);
    string sOldPwd  = tConvert.Json2String( pJson["sOldPwd"]);
    string sNewPwd = tConvert.Json2String( pJson["sNewPwd"]);
    long lUpdateId = tConvert.GetCurrentTime();
    ZoneAgentEntity zoneEntity;
    iRet = ZoneAgentDAL::GetInstance()->GetEntityByKey( sPrimaryKey,1,zoneEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    ZoneAgentEntity entity;
    iRet = ZoneAgentDAL::GetInstance()->GetEntityByKey(sPrimaryKey,1,entity);
    if( 0 == iRet){
        if( sOldPwd != entity.contentEntity.sPassWord){
            RetJsonMsg::SetRetMsg(-6,pResponse);
            return -6;
        }
        if ( 0 != ZoneAgentDAL::GetInstance()->ModifyPwd( sPrimaryKey,sNewPwd,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//登陆
int ZoneAgentBLL::Login( const  Json::Value  & pJson ,Json::Value & pResponse ){

    int iRet = -1;
    TConvert tConvert;
    if(pJson["sAccount"].isNull()  || pJson["sPassWord"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return iRet;
    }
    string sAccount  = tConvert.Json2String( pJson["sAccount"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);

    ZoneAgentEntity entity;
    iRet =  ZoneAgentDAL::GetInstance()->GetEntityByKey(sAccount,1,entity);
    if( 0 == iRet ){
        if( entity.contentEntity.sPassWord == sPassWord){
            pResponse["iRet"] = 0;
            return  0;
        }else{
            RetJsonMsg::SetRetMsg(-6,pResponse);
            return -6;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

int ZoneAgentBLL::ResetPwd(   const  Json::Value  & pJson ,Json::Value & pResponse){
    TConvert tConvert;
    int iRet = -1;
    if(pJson["sPrimaryKey"].isNull()  || pJson["sNewPwd"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sPrimaryKey  =  tConvert.Json2String(pJson["sPrimaryKey"]);
    string sNewPwd =  tConvert.Json2String(pJson["sNewPwd"]);
    long lUpdateId = tConvert.GetCurrentTime();
    ZoneAgentEntity zoneEntity;
    iRet = ZoneAgentDAL::GetInstance()->GetEntityByKey( sPrimaryKey,1,zoneEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    if ( 0 != ZoneAgentDAL::GetInstance()->ModifyPwd( sPrimaryKey,sNewPwd,lUpdateId ) ) {
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -4;
    }
    pResponse["iRet"] = 0;
    return iRet;
}

void ZoneAgentBLL::SetDefault(ZoneAgentEntity & entity){
    entity.contentEntity.sAccount = "-1nochange";
    entity.contentEntity.sCity = "-1nochange";
    entity.contentEntity.sID = "-1nochange";
    entity.contentEntity.sName = "-1nochange";
    entity.contentEntity.sPassWord = "-1nochange";
    entity.contentEntity.sPhone = "-1nochange";
    entity.contentEntity.sRemark = "-1nochange";
    entity.contentEntity.sUnit = "-1nochange";
}
