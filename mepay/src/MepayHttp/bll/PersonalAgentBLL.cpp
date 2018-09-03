//#include "PersonalAgentBLL.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <sstream>
//#include "log/Logger.h"
//#include "TFile.h"
//#include "TConvert.h"
//#include "dal/PersonalAgentDAL.h"
//#include "TBLL.h"
//#include "com/EvhtpSvr.h"
//#include "com/RetJsonMsg.hpp"
//using namespace std;
//PersonalAgentBLL* PersonalAgentBLL::mInstance = NULL;
//PersonalAgentBLL* PersonalAgentBLL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new PersonalAgentBLL;
//    }
//    return mInstance;
//}

//int PersonalAgentBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
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

//    }else if( "IsPersonAgent" == psMethod){

//        iRet = IsPersonAgent(pJson,pResponse);

//    }else{
//      RetJsonMsg::SetRetMsg(-19,pResponse);
//    }
//    return iRet;
//}



////初始化
//int PersonalAgentBLL::Init()
//{
//    PersonalAgentDAL::GetInstance()->CreateTable();
//    return 0;
//}

////增加
//int PersonalAgentBLL::Insert( const  Json::Value  & pJson, Json::Value & pResponse )
//{
//    TConvert tConvert;
//    int iRet = -1;
//    PersonalAgentEntity entity;
//    entity.iState = 0;
//    long lCurrentTime = tConvert.GetCurrentTime();
//    entity.lAddTime = lCurrentTime;
//    entity.lUpdateId = lCurrentTime;
//    entity.lUpTime = lCurrentTime;

//    if( pJson["sPhone"].isNull() || pJson["sPrimaryKey"].isNull()   || pJson["iActivationState"].isNull()
//            || pJson["lActivationTime"].isNull()  || pJson["sOrderNo"].isNull()  || pJson["lMoney"].isNull()
//            || pJson["lDealTime"].isNull()  || pJson["lPushId"].isNull() || pJson["sCity"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    entity.sPrimaryKey =  tConvert.Json2String( pJson["sPrimaryKey"]) ;
//    entity.sOrderNo =  tConvert.Json2String( pJson["sOrderNo"]) ;
//    entity.sPhone = tConvert.Json2String( pJson["sPhone"]) ;
//    entity.lActivationTime = tConvert.Json2Long( pJson["lActivationTime"]) ;
//    entity.lDealTime = tConvert.Json2Long( pJson["lDealTime"]) ;
//    entity.lMoney = tConvert.Json2Long( pJson["lMoney"]) ;
//    entity.iActivationState = tConvert.Json2Int( pJson["iActivationState"]) ;
//    entity.lPushId = tConvert.Json2Long( pJson["lPushId"]) ;
//    entity.sCity = tConvert.Json2String( pJson["sCity"]) ;
//    entity.lExpireTime = entity.lActivationTime + 5184000000000;//60天到期

//    iRet = PersonalAgentDAL::GetInstance()->Insert(entity);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"PersonalAgentBLL::Insert error");
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}
////删除
//int PersonalAgentBLL::Delete( const  Json::Value  & pJson  ,Json::Value & pResponse)
//{
//    TConvert tConvert;
//    int iRet = -1;
//    PersonalAgentEntity pEntity;
//    if(pJson["sPrimaryKey"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);

//    long lCurrentTime = tConvert.GetCurrentTime();

//    iRet = PersonalAgentDAL::GetInstance()->Delete(pEntity.sPrimaryKey,lCurrentTime,lCurrentTime);
//    if ( 0 != iRet) {
//        appendlog(TTDLogger::LOG_ERROR,"PersonalAgentBLL::Delete  error");
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////修改
//int PersonalAgentBLL::Update( const  Json::Value  & pJson ,Json::Value & pResponse)
//{
//    PersonalAgentEntity pEntity;
//    TConvert tConvert;
//    int iRet =-1;
//    if(pJson["sPrimaryKey"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    SetDefault(pEntity);
//    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);
//    PersonalAgentEntity tmpEntity;
//    iRet = PersonalAgentDAL::GetInstance()->GetEntityByKey(pEntity.sPrimaryKey,1,tmpEntity);
//    if( iRet != 0){
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pEntity.lUpTime = tConvert.GetCurrentTime();
//    pEntity.lUpdateId = pEntity.lUpTime;

//    if( !pJson["sPhone"].isNull()){
//        pEntity.sPhone =  tConvert.Json2String(pJson["sPhone"]);
//    }
//    if( !pJson["iActivationState"].isNull()){
//        pEntity.iActivationState =  tConvert.Json2Int(pJson["iActivationState"]);
//    }

//    if( !pJson["lActivationTime"].isNull()){
//        pEntity.lActivationTime = tConvert.Json2Long(  pJson["lActivationTime"]);
//    }

//    if( !pJson["sOrderNo"].isNull()){
//        pEntity.sOrderNo = tConvert.Json2String( pJson["sOrderNo"]);
//    }
//    if( !pJson["lMoney"].isNull()){
//        pEntity.lMoney = tConvert.Json2Long( pJson["lMoney"]);
//    }
//    if( !pJson["lExpireTime"].isNull()){
//        pEntity.lExpireTime =  tConvert.Json2Long(pJson["lExpireTime"]);
//    }
//    if( !pJson["lPushId"].isNull()){
//        pEntity.lPushId =  tConvert.Json2Long(pJson["lPushId"]);
//    }
//    if( !pJson["sCity"].isNull()){
//        pEntity.sCity =  tConvert.Json2String(pJson["sCity"]);
//    }
//    if( !pJson["lDealTime"].isNull()){
//        pEntity.lDealTime =  tConvert.Json2Long(pJson["lDealTime"]);
//    }


//    iRet = PersonalAgentDAL::GetInstance()->Update(pEntity.sPrimaryKey,pEntity);
//    if ( 0 != iRet ) {
//        appendlog(TTDLogger::LOG_ERROR,"PersonalAgentBLL::Update error");
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    pResponse["iRet"] = 0;
//    return 0;
//}

////查询一行内容
//int PersonalAgentBLL::GetEntity(const  Json::Value  & pJson,
//                            Json::Value & pResponse)
//{
//    int iRet = -1;
//    TConvert tConvert;
//    if(pJson["sPrimaryKey"].isNull() && pJson["sOrderNo"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return iRet;
//    }
//    string sCond ;
//    int iType = 0;
//    if( ! pJson["sPrimaryKey"].isNull() ){
//        sCond =  tConvert.Json2String(pJson["sPrimaryKey"]);
//        iType = 1;
//    }
//    if( ! pJson["sOrderNo"].isNull() ){
//        sCond =  tConvert.Json2String(pJson["sOrderNo"]);
//        iType = 2;
//    }

//    PersonalAgentEntity entity;
//    iRet = PersonalAgentDAL::GetInstance()->GetEntityByKey(sCond,iType,entity);
//    if( iRet == 0  ) {
//        pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
//        pResponse["entity"]["sOrderNo"] = entity.sOrderNo;
//        pResponse["entity"]["sPhone"] = entity.sPhone;
//        pResponse["entity"]["iActivationState"] = entity.iActivationState;
//        pResponse["entity"]["lActivationTime"] = tConvert.LongToStr( entity.lActivationTime);
//        pResponse["entity"]["lDealTime"] = tConvert.LongToStr( entity.lDealTime );
//        pResponse["entity"]["lExpireTime"] = tConvert.LongToStr( entity.lExpireTime );
//        pResponse["entity"]["lMoney"] = tConvert.LongToStr( entity.lMoney);
//        pResponse["entity"]["lPushId"] = tConvert.LongToStr( entity.lPushId);
//        pResponse["entity"]["sCity"] =  entity.sCity;
//        pResponse["entity"]["lAddTime"] =  tConvert.LongToStr(entity.lAddTime);
//        pResponse["entity"]["lUpTime"] =  tConvert.LongToStr(entity.lUpTime);
//        pResponse["iRet"] = 0;
//        return 0;
//    }
//    RetJsonMsg::SetRetMsg(iRet,pResponse);
//    return iRet;
//}

////是否开通个代并付款了
//int PersonalAgentBLL::IsPersonAgent(const  Json::Value  & pJson  ,Json::Value & pResponse)
//{
//    int iRet = -1;
//    TConvert tConvert;
//    if(pJson["lUserId"].isNull() ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return iRet;
//    }
//    long lUserId = tConvert.Json2Long(pJson["lUserId"]);
//    PersonalAgentEntity perEntity;
//    iRet = PersonalAgentDAL::GetInstance()->GetEntityByKey(tConvert.LongToStr(lUserId),1,perEntity);
//    if( iRet != 0){
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }
//    iRet = 0;
//    if( perEntity.iActivationState == 3){
//        iRet = 1;
//    }
//    pResponse["iRet"] = iRet;

//}

////查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//int PersonalAgentBLL::GetData(const  Json::Value  & pJson,
//                          Json::Value  & pResponse)
//{
//    TConvert tConvert;
//    PersonalAgentQueryEntity pQueryEntity;

//    if( pJson["iCount"].isNull()  || pJson["iIndex"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -1;
//    }
//    if( ! pJson["iActivationState"].isNull()){
//        pQueryEntity.iActivationState = tConvert.Json2Int( pJson["iActivationState"]);
//    }else{
//        pQueryEntity.iActivationState = -1;
//    }
//    if( ! pJson["lActivationTimeStart"].isNull()){
//        pQueryEntity.lActivationTimeStart = tConvert.Json2Long( pJson["lActivationTimeStart"]);
//    }else{
//        pQueryEntity.lActivationTimeStart = -1;
//    }
//    if( ! pJson["lActivationTimeEnd"].isNull()){
//        pQueryEntity.lActivationTimeEnd = tConvert.Json2Long( pJson["lActivationTimeEnd"]);
//    }else{
//        pQueryEntity.lActivationTimeEnd = -1;
//    }
//    if( pQueryEntity.lActivationTimeEnd < pQueryEntity.lActivationTimeStart){
//        RetJsonMsg::SetRetMsg(-2,pResponse);
//        return -2;
//    }
//    if(! pJson["sPhone"].isNull()){
//        pQueryEntity.sPhone = tConvert.Json2String( pJson["sPhone"]);
//    }else{
//        pQueryEntity.sPhone = "";
//    }
//    if(! pJson["sCity"].isNull()){
//        pQueryEntity.sCity = tConvert.Json2String( pJson["sCity"]);
//    }else{
//        pQueryEntity.sCity = "";
//    }

//    int piIndex = tConvert.Json2Int( pJson["iIndex"]);
//    int piCount = tConvert.Json2Int( pJson["iCount"]);
//    PersonalAgentList  plsTmp;
//    int iTotalCount = 0;
//    PersonalAgentDAL::GetInstance()->GetData(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

//    int iCount = 0;
//    while( iCount < plsTmp.size()){
//        PersonalAgentEntity  & entity = plsTmp[iCount];
//        pResponse["list"][iCount]["sPrimaryKey"] = entity.sPrimaryKey;
//        pResponse["list"][iCount]["sOrderNo"] = entity.sOrderNo;
//        pResponse["list"][iCount]["sPhone"] = entity.sPhone;
//        pResponse["list"][iCount]["iActivationState"] = entity.iActivationState;
//        pResponse["list"][iCount]["lActivationTime"] = tConvert.LongToStr( entity.lActivationTime);
//        pResponse["list"][iCount]["lExpireTime"] = tConvert.LongToStr( entity.lExpireTime );
//        pResponse["list"][iCount]["lDealTime"] = tConvert.LongToStr( entity.lDealTime );
//        pResponse["list"][iCount]["lMoney"] = tConvert.LongToStr( entity.lMoney);
//        pResponse["list"][iCount]["lPushId"] = tConvert.LongToStr( entity.lPushId);
//        pResponse["list"][iCount]["sCity"] =  entity.sCity;
//        pResponse["list"][iCount]["lAddTime"] =  tConvert.LongToStr(entity.lAddTime);
//        pResponse["list"][iCount]["lUpTime"] =  tConvert.LongToStr(entity.lUpTime);
//         iCount++;
//    }
//    if( plsTmp.size() <= 0){
//        pResponse["list"] ;
//        pResponse["list"].resize(0);
//    }

//    pResponse["iRet"] =0;
//    //返回总个数
//    pResponse["iTotal"]  = iTotalCount;
//    return 0;
//}

//void PersonalAgentBLL::SetDefault(PersonalAgentEntity &  pEntity)
//{
//    pEntity.iActivationState = -1;
//    pEntity.lActivationTime = -1;
//    pEntity.lDealTime = -1;
//    pEntity.lExpireTime = -1;
//    pEntity.lMoney = -1;
//    pEntity.lPushId = -1;
//    pEntity.sOrderNo = "-1nochange";
//    pEntity.sPhone = "-1nochange";
//    pEntity.sCity = "-1nochange";
//}


