#include "ShopInfoBLL.h"

#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/ShopInfoDAL.h"
#include "dal/PersonalAgentDAL.h"
#include "bll/TBLL.h"
#include <math.h>
#include "ErrDef.h"
#include "TMd5.h"
#include "HangPing.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "com/PostsAes.h"
#include "com/TCurl.h"
#include "TypeDef.h"
#include "MyUtil.h"

#include "dal/ZoneAgentDAL.h"
using namespace std;

ShopInfoBLL* ShopInfoBLL::mInstance = NULL;
ShopInfoBLL* ShopInfoBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new ShopInfoBLL;
    }
    return mInstance;
}

//处理带token，不加密的情况
int ShopInfoBLL::Process2(EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;

    string sUrl = pEvhtp->GetUrl();
    string sDeToken = pEvhtp->GetHead("detoken");

    if( sUrl.find( "/near/pwd") != string::npos ){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);
        iRet = ModifyPwd2(id,pJson,pResponse);

    }else if( sUrl.find("/near/modify") != string::npos ){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);
        iRet = Update2(id,pJson,pResponse);

    }else if(sUrl.find( "/near/discount") != string::npos){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);
        iRet = ApplyDiscount2(id,pJson,pResponse);

    }else if(sUrl.find( "/near/getTrade") != string::npos){

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        StringLst lst;
        tConvert.Split(sUrl,"/",lst);
        string sStartTime = lst.at(3);
        string sEndTime = lst.at(4);
        string sPage = lst.at(5);
        string sPayPhone;
        if( lst.size() >= 7)
            sPayPhone = lst.at(6);

        iRet = GetTrade2(tConvert.LongToStr(id),sStartTime,sEndTime,sPayPhone,sPage,pResponse);

    }else if(sUrl.find( "/near/getEntity") != string::npos){

        StringLst lst;
        tConvert.Split(sUrl,"/",lst);
        string sPlatformId = lst.at(3);

        iRet = GetEntity2(tConvert.StrToLong( sPlatformId ),pJson,pResponse);

    }else if(sUrl.find( "/near/getData") != string::npos){

        iRet =GetData2( pJson,pResponse);
    }else if(sUrl.find( "/near/getCommtent") != string::npos){

        StringLst lst;
        tConvert.Split(sUrl,"/",lst);
        string sPlatformId = lst.at(3);
        string sAddTime = lst.at(4);

        iRet = GetCommtent2(tConvert.StrToLong( sPlatformId ),tConvert.StrToLong(sAddTime),10,pResponse);

    }else if(sUrl.find( "/near/writeCommtent") != string::npos){


        iRet =WriteCommtent2( pJson,pResponse);

    }else if(sUrl.find( "/near/writeSuperCommtent") != string::npos){

        iRet =WriteCommtentBySuper2( pJson,pResponse);
    }else if(sUrl.find( "/near/getShow") != string::npos){//分页查询产品（已审核）

        iRet =ShowProduct2( pJson,pResponse);
    }else if(sUrl.find( "/near/addShow") != string::npos){//添加一个产品

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        iRet =AddProduct2(id, pJson,pResponse);
    }else if(sUrl.find( "/near/showProduct") != string::npos){//获取某个附近商家的产品(已审核）

        StringLst lst;
        tConvert.Split(sUrl,"/",lst);
        string sPlatformId = lst.at(3);
        string sPage = lst.at(4);

        iRet = ShowProduct2(tConvert.StrToLong( sPlatformId ),tConvert.StrToLong(sPage),pResponse);

    }else if(sUrl.find( "/near/getProduct") != string::npos){//获取产品(根据条件，包括审核状态等等）

        iRet = GetProduct2(pJson,pResponse);

    }else if(sUrl.find( "/near/getShopProduct") != string::npos){//商家登录后获取产品(根据条件，包括审核状态等等）

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        iRet = GetProductByToken(id,pJson,pResponse);

    }else if(sUrl.find( "/near/ModifyShow") != string::npos){//更改一个产品

        if( sDeToken.empty()){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            return -2;
        }

        long id = GetPlatformId(sDeToken);

        iRet =UpProduct2(id, pJson,pResponse);
    }
    return iRet;
}

int ShopInfoBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if("UpdateData" == psMethod ){

        iRet =Update(pJson,pResponse);

    }else if("DeleteData" == psMethod){

        iRet =Delete(pJson,pResponse);

    }else if( "GetEntity" == psMethod){

        iRet = GetEntity(pJson,pResponse);

    }else if( "GetData" == psMethod){

        iRet = GetData(pJson,pResponse);

    }else if( "ModifyPwd" == psMethod){

        iRet = ModifyPwd(pJson,pResponse);

    }else if( "Create" == psMethod){

        iRet = CreateShop(pJson,pResponse);

    }else if( "Login" == psMethod){

        iRet = Login(pJson,pResponse);

    }else if( "GetMyShop" == psMethod){
        iRet = GetMyShop(pJson, pResponse);
    }else if( "ResetPwd" == psMethod){
        iRet = ResetPwd(pJson,pResponse);
    }else if( "ApplyDiscount" == psMethod){
        iRet = ApplyDiscount(pJson,pResponse);
    }else if( "ChangeDiscount" == psMethod){
        iRet = ChangeDiscount(pJson,pResponse);
    }else if("GetPage" == psMethod){
        iRet = GetPage(pJson,pResponse);
    }else if( "CreateShopApp" == psMethod){
        iRet = CreateShopApp(pJson, pResponse);
    }else if( "ShopsByPerAgent" == psMethod){
        iRet = ShopsByPerAgent(pJson, pResponse);
    }else if( "Doc" == psMethod){
        iRet = Doc( pJson, pResponse);
    }else if( "AddBank" == psMethod){
        iRet = AddBank(pJson,pResponse);
    }else if( "GetBank" == psMethod){
        iRet = GetBank(pJson,pResponse);
    }else if( "Setting" == psMethod){
        iRet = Setting(pJson,pResponse);
    }else if( "GetSetting" == psMethod){
        iRet = GetSetting(pJson,pResponse);
    }else if( "ModifyPhone" == psMethod){
        iRet = ModifyPhone(pJson,pResponse);
    }else if( "GetState" == psMethod){
        iRet = GetState(pJson,pResponse);
    }else if( "CashBackShops" == psMethod){
        iRet = CashBackShops(pJson,pResponse);
    }else if( "RecommendShops" == psMethod){
        iRet = RecommendShops(pJson,pResponse);
    }else if( "GetNearBy" == psMethod){
        iRet = GetNearBy(pJson,pResponse);
    }else if( "GetLotsPlatfrom" == psMethod){
        iRet = GetLotsPlatfrom(pJson,pResponse);
    }else if( "WriteCommtent" == psMethod){
        iRet = WriteCommtent(pJson,pResponse);
    }else if( "GetCommtent" == psMethod){
        iRet = GetCommtent(pJson,pResponse);
    }else if( "HotShops" == psMethod){
        iRet = HotShops(pJson,pResponse);
    }else if( "Products" == psMethod){
        iRet = Products(pJson,pResponse);
    }else if("ShowProduct" == psMethod){
        iRet = ShowProduct(pJson,pResponse);
    }else if("AddAdv" == psMethod){
        iRet = AddAdv(pJson,pResponse);
    }else if("DelAdv" == psMethod){
        iRet = DelAdv(pJson,pResponse);
    }else if("AdvVerify" == psMethod){
        iRet = AdvVerify(pJson,pResponse);
    }else if("GetAdv" == psMethod){
        iRet = GetAdv(pJson,pResponse);
    }else if("AddProduct" == psMethod){
        iRet = AddProduct(pJson,pResponse);
    }else if("DelProduct" == psMethod){
        iRet = DelProduct(pJson,pResponse);
    }else if("ProductVerify" == psMethod){
        iRet = ProductVerify(pJson,pResponse);
    }else if("GetProduct" == psMethod){
        iRet = GetProduct(pJson,pResponse);
    }else if("UpProduct" == psMethod){
        iRet = UpProduct(pJson,pResponse);
    }else if( "Check" == psMethod){
        iRet = Check(pJson,pResponse);
    }else if( "ModifyCheck" == psMethod){
        iRet = ModifyCheck(pJson,pResponse);
    }else if("Reset" == psMethod){
        iRet = Reset(pJson,pResponse);
    }else if("CityShopCount" == psMethod){
        iRet = CityShopCount(pJson,pResponse);
    }else if("GetObjects" == psMethod){
        iRet = GetObjects(pJson,pResponse);
    }else if("GetShopsByObjest" == psMethod){
        iRet = GetShopsByObjest(pJson,pResponse);
    }else if("GetQuikMark" == psMethod){
        iRet = GetQuikMark(pJson,pResponse);
    }else if("GetClass" == psMethod){
        iRet = GetClass(pJson,pResponse);
    }else if("OpenbyPerAgent" == psMethod){
        iRet = OpenbyPerAgent(pJson,pResponse);
    }else if( "ShopPerfect" == psMethod){
        iRet = ShopPerfect( pJson,pResponse);
    }else if( "PromotionConfig" == psMethod){
        iRet = PromotionConfig( pJson,pResponse);
    }else if("GetAgentInfoByPlatformId" == psMethod){
        iRet = GetAgentInfoByPlatformId(pJson,pResponse);
    }else if("MyShop" == psMethod){
        iRet = MyShop(pJson,pResponse);
    }else if("MyImpowerShop" == psMethod){
        iRet = MyImpowerShop(pJson,pResponse);
    }
    else {
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}

//初始化
int ShopInfoBLL::Init()
{
    mBankDal.CreateTable();
    mAdvDAL.CreateTable();
    mKeySecretDAL.CreateTable();
    mSubjectDAL.CreateTable();
    mClassDAL.CreateTable();
    return 0;
}

int ShopInfoBLL::Delete( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    RetJsonMsg::SetRetMsg(-15,pResponse);
    return -1;
    TConvert tConvert;
    int iRet = -1;
    if( pJson["sPrimaryKey"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    ShopInfoEntity pEntity;
    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"] );


    long lCurrentTime = tConvert.GetCurrentTime();
    //删除
    iRet = ShopInfoDAL::GetInstance()->Delete( pEntity.sPrimaryKey,lCurrentTime,lCurrentTime ) ;
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"UserIndexBLL::Delete sPrimaryKey=%s",pEntity.sPrimaryKey.c_str());
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"] = 0;
    return 0;
}
//更新商家基本资料
int ShopInfoBLL::Update( const  Json::Value  & pJson ,Json::Value & pResponse  )
{
    //  ScopeLock lock( this->mMutex );
    int iRet = -1;
    TConvert tConvert;
    if( pJson["sPrimaryKey"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    //修改
    ShopInfoEntity pEntity;
    SetDefault(pEntity);
    long lCurrentTime = tConvert.GetCurrentTime();
    long lUpdateId = lCurrentTime;
    pEntity.lAddTime = lCurrentTime;
    pEntity.lUpTime = lCurrentTime;
    pEntity.iState = 0;
    pEntity.lUpdateId = lUpdateId;
    pEntity.sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"]);

    ShopInfoEntity shopEntity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(pEntity.sPrimaryKey,1, shopEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    if( ! pJson["lUserId"].isNull() ){
        pEntity.contentEntity.lUserId =tConvert.Json2Long( pJson["lUserId"]);
    }else{
        pEntity.contentEntity.lUserId = -1;
    }
    if(! pJson["dGpsLat"].isNull() ){
        pEntity.contentEntity.dGpsLat =tConvert.Json2Double( pJson["dGpsLat"]);
    }else{
        pEntity.contentEntity.dGpsLat = -1.0;
    }
    if( ! pJson["dGpsLon"].isNull() ){
        pEntity.contentEntity.dGpsLon =tConvert.Json2Double( pJson["dGpsLon"]);
    }else{
        pEntity.contentEntity.dGpsLon = -1.0;
    }

    pEntity.contentEntity.iOffLineDisCount = -1;
    //只有补全资料时才可以直接修改折扣
    if( ! pJson["iOffLineDisCount"].isNull() && shopEntity.contentEntity.iOffLineDisCount == -1){
        int iOffLineDisCount  =tConvert.Json2Int( pJson["iOffLineDisCount"]);
        if( iOffLineDisCount > 0){
            pEntity.contentEntity.iOffLineDisCount = iOffLineDisCount;
        }
    }

    if( ! pJson["lServiceUserId"].isNull() ){
        pEntity.contentEntity.lServiceUserId =tConvert.Json2Long( pJson["lServiceUserId"]);
    }else{
        pEntity.contentEntity.lServiceUserId = -1;
    }
    if( ! pJson["sAddress"].isNull() ){
        pEntity.contentEntity.sAddress =tConvert.Json2String( pJson["sAddress"]);
        string sAgent = "龙华区";
        if( pEntity.contentEntity.sAddress.find("深圳") != string::npos &&
                pEntity.contentEntity.sAddress.find(sAgent) != string::npos){
            pEntity.contentEntity.sAddress = pEntity.contentEntity.sAddress.replace(pEntity.contentEntity.sAddress.find(sAgent),sAgent.length(),"龙华新区");
        }

    }
    if( ! pJson["iAwardingPoints"].isNull() ){
        pEntity.contentEntity.iAwardingPoints =tConvert.Json2Int( pJson["iAwardingPoints"]);
    }else{
        pEntity.contentEntity.iAwardingPoints = -1;
    }
    if( ! pJson["sBusiness"].isNull() ){
        pEntity.contentEntity.sBusiness =tConvert.Json2String( pJson["sBusiness"]);
    }
    if( ! pJson["sCompanyName"].isNull() ){
        pEntity.contentEntity.sCompanyName =tConvert.Json2String( pJson["sCompanyName"]);
    }

    if( ! pJson["lPerAgentId"].isNull() ){
        pEntity.contentEntity.lPerAgentId =tConvert.Json2Long( pJson["lPerAgentId"]);
    }else{
        pEntity.contentEntity.lPerAgentId = -1;
    }
    if( ! pJson["sHours"].isNull() ){
        pEntity.contentEntity.sHours =tConvert.Json2String( pJson["sHours"]);
    }
    if( ! pJson["sIdPhoto"].isNull() ){
        pEntity.contentEntity.sIdPhoto =tConvert.Json2String( pJson["sIdPhoto"]);
    }
    if( ! pJson["sShopPhone"].isNull() ){
        pEntity.contentEntity.sShopPhone =tConvert.Json2String( pJson["sShopPhone"]);
    }
    if( ! pJson["sLicensePhoto"].isNull() ){
        pEntity.contentEntity.sLicensePhoto =tConvert.Json2String( pJson["sLicensePhoto"]);
    }
    if( ! pJson["sPosNum"].isNull() ){
        pEntity.contentEntity.sPosNum =tConvert.Json2String( pJson["sPosNum"]);
    }
    if( ! pJson["sSevvicePhone"].isNull() ){
        pEntity.contentEntity.sSevvicePhone =tConvert.Json2String( pJson["sSevvicePhone"]);
    }
    if( ! pJson["sShopDetailPhoto"].isNull() ){
        pEntity.contentEntity.sShopDetailPhoto =tConvert.Json2String( pJson["sShopDetailPhoto"]);
    }
    if( ! pJson["sShopFrontPhoto"].isNull() ){
        pEntity.contentEntity.sShopFrontPhoto =tConvert.Json2String( pJson["sShopFrontPhoto"]);
    }
    if( ! pJson["sShopName"].isNull() ){
        pEntity.contentEntity.sShopName =tConvert.Json2String( pJson["sShopName"]);
    }
    if( ! pJson["sOtherCerPhoto"].isNull() ){
        pEntity.contentEntity.sOtherCerPhoto =tConvert.Json2String( pJson["sOtherCerPhoto"]);
    }
    if( ! pJson["sTradeMark"].isNull() ){
        pEntity.contentEntity.sTradeMark =tConvert.Json2String( pJson["sTradeMark"]);
    }
    if( ! pJson["sShopType"].isNull() ){
        pEntity.contentEntity.sShopType =tConvert.Json2String( pJson["sShopType"]);
    }
    if( ! pJson["iAvgConsume"].isNull() ){
        pEntity.contentEntity.iAvgConsume =tConvert.Json2Int( pJson["iAvgConsume"]);
    }else{
        pEntity.contentEntity.iAvgConsume = -1;
    }
    if( ! pJson["iShowNearby"].isNull() ){
        pEntity.contentEntity.iShowNearby =tConvert.Json2Int( pJson["iShowNearby"]);
    }else{
        pEntity.contentEntity.iShowNearby = -1;
    }
    if( ! pJson["iProrate"].isNull() ){
        pEntity.contentEntity.iProrate =tConvert.Json2Int( pJson["iProrate"]);
    }else{
        pEntity.contentEntity.iProrate = -1;
    }
    if( ! pJson["iVerify"].isNull() ){
        pEntity.contentEntity.iVerify =tConvert.Json2Int( pJson["iVerify"]);
        if( pEntity.contentEntity.iVerify < 0 || pEntity.contentEntity.iVerify > 3){
            RetJsonMsg::SetRetMsg(-2,pResponse);
            return -2;
        }
    }else{
        pEntity.contentEntity.iVerify = -1;
    }
    if( ! pJson["iAuth"].isNull() ){
        pEntity.contentEntity.iAuth =tConvert.Json2Int( pJson["iAuth"]);
        if( pEntity.contentEntity.iAuth < 0 || pEntity.contentEntity.iAuth > 3){
            RetJsonMsg::SetRetMsg(-2,pResponse);
            return -2;
        }
    }

    if( ! pJson["iCoinType"].isNull() ){
        pEntity.contentEntity.iCoinType =tConvert.Json2Int( pJson["iCoinType"]);
    }else{
        pEntity.contentEntity.iCoinType = -1;
    }

    if( ! pJson["iOffLineDisCount"].isNull() ){

        int iOffLineDisCount  =tConvert.Json2Int( pJson["iOffLineDisCount"]);
        if( (shopEntity.contentEntity.iOffLineDisCount != iOffLineDisCount) && (iOffLineDisCount >=60 && iOffLineDisCount <=95)){//不能和原来的折扣一样

            Json::Value jParam,jRes;
            jParam["lPlatformId"] = pJson["sPrimaryKey"];
            jParam["iDiscount"] = pJson["iOffLineDisCount"];
            ApplyDiscount(jParam,jRes);
        }
    }


    if( !pJson["iDelivery"].isNull()){
        Json::Value jParam,jRes;
        jParam["lPlatformId"] = pJson["sPrimaryKey"];
        jParam["iDelivery"] = pJson["iDelivery"];
        Setting(jParam,jRes);
    }

    if( !pJson["iPark"].isNull()){
        Json::Value jParam,jRes;
        jParam["lPlatformId"] = pJson["sPrimaryKey"];
        jParam["iPark"] = pJson["iPark"];
        Setting(jParam,jRes);
    }

    if( !pJson["iWifi"].isNull()){
        Json::Value jParam,jRes;
        jParam["lPlatformId"] = pJson["sPrimaryKey"];
        jParam["iWifi"] = pJson["iWifi"];
        Setting(jParam,jRes);
    }

    if( pEntity.contentEntity.iVerify == 1 && shopEntity.contentEntity.iVerify !=1){//从未审核改为审核
        string sOrgPwd = MyUtil::MakeRandNum(6);
        long lUp = tConvert.GetCurrentTime();
        ShopInfoEntity shopEntity2;
        ShopInfoDAL::GetInstance()->GetEntity(pEntity.sPrimaryKey,1,shopEntity2);
        if( "美支付供应商" == shopEntity2.contentEntity.sCtiy)
        {
            if( 0 == ShopInfoDAL::GetInstance()->ModifyPwd(pEntity.sPrimaryKey, Md5ToLower("pay" +sOrgPwd),lUp))
            {
                //发送商家开通通知
                string sMsg = pEntity.sPrimaryKey;
                sMsg += ",";
                sMsg += sOrgPwd;
                SendPhoneNotifyCode( shopEntity2.contentEntity.sShopPhone, sMsg,"11");
                SendNotify(shopEntity2.contentEntity.lUserId,shopEntity2.sPrimaryKey,sOrgPwd);

            }
        }
    }

    if( pEntity.contentEntity.iCoinType >= 0 && !pEntity.contentEntity.sSevvicePhone.empty()
            && pEntity.contentEntity.iOffLineDisCount >=0 && !pEntity.contentEntity.sShopType.empty()){
        pEntity.iState = 0;
    }
    iRet = ShopInfoDAL::GetInstance()->Update( pEntity.sPrimaryKey,pEntity ) ;
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"UserIndexBLL::Update sPrimaryKey=%s",pEntity.sPrimaryKey.c_str());
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    //更改平台主或申请平台通过时，让平台主关注自己的平台号
    if( pEntity.contentEntity.lUserId > 0 ||  pEntity.contentEntity.iVerify != -1){
        bool bRet = Attention(shopEntity.contentEntity.lUserId,tConvert.StrToLong( pEntity.sPrimaryKey));
        if( !bRet){
            appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::Update Attention err [PlatformId:%s][lUserId:%ld]", pEntity.sPrimaryKey.c_str(),shopEntity.contentEntity.lUserId);
        }
    }


    SendModifyShopNotice( tConvert.StrToLong( pEntity.sPrimaryKey),pEntity.contentEntity.sShopName,pEntity.contentEntity.sAddress,pEntity.contentEntity.iCoinType
                          ,pEntity.contentEntity.iOffLineDisCount);

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}



//更新商家基本资料,处理带token，不加密的情况
int ShopInfoBLL::Update2(long lPlatformId, const  Json::Value  & pJson ,Json::Value & pResponse  )
{
    //  ScopeLock lock( this->mMutex );
    int iRet = -1;
    TConvert tConvert;

    //修改
    ShopInfoEntity pEntity;
    SetDefault(pEntity);
    long lCurrentTime = tConvert.GetCurrentTime();
    long lUpdateId = lCurrentTime;
    pEntity.lAddTime = lCurrentTime;
    pEntity.lUpTime = lCurrentTime;
    pEntity.iState = 0;
    pEntity.lUpdateId = lUpdateId;
    pEntity.sPrimaryKey = tConvert.LongToStr( lPlatformId);

    ShopInfoEntity shopEntity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(pEntity.sPrimaryKey,1, shopEntity);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg2(iRet,pResponse);
        return iRet;
    }


    if( ! pJson["coinType"].isNull() ){
        pEntity.contentEntity.iCoinType =tConvert.Json2Int( pJson["coinType"]);
    }else{
        pEntity.contentEntity.iCoinType = -1;
    }
    if( ! pJson["useId"].isNull() ){
        pEntity.contentEntity.lUserId =tConvert.Json2Long( pJson["useId"]);
    }else{
        pEntity.contentEntity.lUserId = -1;
    }
    if(! pJson["gpsLat"].isNull() ){
        pEntity.contentEntity.dGpsLat =tConvert.Json2Double( pJson["gpsLat"]);
    }else{
        pEntity.contentEntity.dGpsLat = -1.0;
    }
    if( ! pJson["gpsLon"].isNull() ){
        pEntity.contentEntity.dGpsLon =tConvert.Json2Double( pJson["gpsLon"]);
    }else{
        pEntity.contentEntity.dGpsLon = -1.0;
    }
    if( ! pJson["offLineDisCount"].isNull() ){
        pEntity.contentEntity.iOffLineDisCount =tConvert.Json2Int( pJson["offLineDisCount"]);
    }else{
        pEntity.contentEntity.iOffLineDisCount = -1;
    }
    if( ! pJson["serviceId"].isNull() ){
        pEntity.contentEntity.lServiceUserId =tConvert.Json2Long( pJson["serviceId"]);
    }else{
        pEntity.contentEntity.lServiceUserId = -1;
    }
    if( ! pJson["address"].isNull() ){
        pEntity.contentEntity.sAddress =tConvert.Json2String( pJson["address"]);
    }
    if( ! pJson["awardingPoints"].isNull() ){
        pEntity.contentEntity.iAwardingPoints =tConvert.Json2Int( pJson["awardingPoints"]);
    }else{
        pEntity.contentEntity.iAwardingPoints = -1;
    }
    if( ! pJson["business"].isNull() ){
        pEntity.contentEntity.sBusiness =tConvert.Json2String( pJson["business"]);
    }
    if( ! pJson["companyName"].isNull() ){
        pEntity.contentEntity.sCompanyName =tConvert.Json2String( pJson["companyName"]);
    }

    if( ! pJson["perAgentId"].isNull() ){
        pEntity.contentEntity.lPerAgentId =tConvert.Json2Long( pJson["perAgentId"]);
    }else{
        pEntity.contentEntity.lPerAgentId = -1;
    }
    if( ! pJson["hours"].isNull() ){
        pEntity.contentEntity.sHours =tConvert.Json2String( pJson["hours"]);
    }
    if( ! pJson["idPhoto"].isNull() ){
        pEntity.contentEntity.sIdPhoto =tConvert.Json2String( pJson["idPhoto"]);
        if( !IsPic(pEntity.contentEntity.sIdPhoto)){
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;

        }
    }
    if( ! pJson["shopPhone"].isNull() ){
        pEntity.contentEntity.sShopPhone =tConvert.Json2String( pJson["shopPhone"]);
    }
    if( ! pJson["licensePhoto"].isNull() ){
        pEntity.contentEntity.sLicensePhoto =tConvert.Json2String( pJson["licensePhoto"]);
        if( !IsPic(pEntity.contentEntity.sLicensePhoto)){
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;

        }
    }
    if( ! pJson["posNum"].isNull() ){
        pEntity.contentEntity.sPosNum =tConvert.Json2String( pJson["posNum"]);
    }
    if( ! pJson["sevicePhone"].isNull() ){
        pEntity.contentEntity.sSevvicePhone =tConvert.Json2String( pJson["sevicePhone"]);
    }
    if( ! pJson["shopDetailPhoto"].isNull() ){
        pEntity.contentEntity.sShopDetailPhoto =tConvert.Json2String( pJson["shopDetailPhoto"]);

    }
    if( ! pJson["shopFrontPhoto"].isNull() ){
        pEntity.contentEntity.sShopFrontPhoto =tConvert.Json2String( pJson["shopFrontPhoto"]);
        if( !IsPic(pEntity.contentEntity.sShopFrontPhoto)){
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;

        }
    }
    if( ! pJson["shopName"].isNull() ){
        pEntity.contentEntity.sShopName =tConvert.Json2String( pJson["shopName"]);
    }
    if( ! pJson["otherCerPhoto"].isNull() ){
        pEntity.contentEntity.sOtherCerPhoto =tConvert.Json2String( pJson["otherCerPhoto"]);
    }
    if( ! pJson["tradeMark"].isNull() ){
        pEntity.contentEntity.sTradeMark =tConvert.Json2String( pJson["tradeMark"]);
    }
    if( ! pJson["shopType"].isNull() ){
        pEntity.contentEntity.sShopType =tConvert.Json2String( pJson["shopType"]);
    }
    if( ! pJson["avgConsume"].isNull() ){
        pEntity.contentEntity.iAvgConsume =tConvert.Json2Int( pJson["avgConsume"]);
    }else{
        pEntity.contentEntity.iAvgConsume = -1;
    }
    if( ! pJson["showNearby"].isNull() ){
        pEntity.contentEntity.iShowNearby =tConvert.Json2Int( pJson["showNearby"]);
    }else{
        pEntity.contentEntity.iShowNearby = -1;
    }
    if( ! pJson["prorate"].isNull() ){
        pEntity.contentEntity.iProrate =tConvert.Json2Int( pJson["prorate"]);
    }else{
        pEntity.contentEntity.iProrate = -1;
    }
    if( ! pJson["verify"].isNull() ){
        pEntity.contentEntity.iVerify =tConvert.Json2Int( pJson["verify"]);
        if( pEntity.contentEntity.iVerify < 0 || pEntity.contentEntity.iVerify > 3){
            RetJsonMsg::SetRetMsg2(-1,pResponse);
            return -1;
        }
    }else{
        pEntity.contentEntity.iVerify = -1;
    }
    if( ! pJson["auth"].isNull() ){
        pEntity.contentEntity.iAuth =tConvert.Json2Int( pJson["auth"]);
        if( pEntity.contentEntity.iAuth < 0 || pEntity.contentEntity.iAuth > 3){
            RetJsonMsg::SetRetMsg2(-1,pResponse);
            return -1;
        }
    }

    if( pEntity.contentEntity.iVerify == 1 ){
        string sOrgPwd = MyUtil::MakeRandNum(6);
        long lUp = tConvert.GetCurrentTime();
        ShopInfoEntity shopEntity2;
        ShopInfoDAL::GetInstance()->GetEntity(pEntity.sPrimaryKey,1,shopEntity2);
        if( "美支付供应商" == shopEntity2.contentEntity.sCtiy)
        {
            if( 0 == ShopInfoDAL::GetInstance()->ModifyPwd(pEntity.sPrimaryKey, Md5ToLower("pay" +sOrgPwd),lUp))
            {
                //发送商家开通通知
                string sMsg = pEntity.sPrimaryKey;
                sMsg += ",";
                sMsg += sOrgPwd;
                SendPhoneNotifyCode( shopEntity2.contentEntity.sShopPhone, sMsg,"11");
                SendNotify(shopEntity2.contentEntity.lUserId,shopEntity2.sPrimaryKey,sOrgPwd);

            }
        }
    }
    iRet = ShopInfoDAL::GetInstance()->Update( pEntity.sPrimaryKey,pEntity ) ;
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"UserIndexBLL::Update sPrimaryKey=%s",pEntity.sPrimaryKey.c_str());
        RetJsonMsg::SetRetMsg2(iRet,pResponse);
        return iRet;
    }
    //更改平台主或申请平台通过时，让平台主关注自己的平台号
    if( pEntity.contentEntity.lUserId > 0 ||  pEntity.contentEntity.iVerify != -1){
        bool bRet = Attention(shopEntity.contentEntity.lUserId,tConvert.StrToLong( pEntity.sPrimaryKey));
        if( !bRet){
            appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::Update Attention err [PlatformId:%s][lUserId:%ld]", pEntity.sPrimaryKey.c_str(),shopEntity.contentEntity.lUserId);
        }
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}

//查询数据-平台号ID或帐号或用户ID,(处理带token，不加密的情况)
int ShopInfoBLL::GetEntity2( long lPlatformId ,const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;


    ShopInfoEntity  entity;
    iRet =  ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(lPlatformId) ,1,entity);
    if ( 0 == iRet){
        pResponse["result"]["lPlatformId"] = entity.sPrimaryKey;
        pResponse["result"]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);

        pResponse["result"]["lServiceUserId"] =  tConvert.LongToStr( entity.contentEntity.lServiceUserId);
        pResponse["result"]["account"] = entity.contentEntity.sAccount;
        pResponse["result"]["shopName"] = entity.contentEntity.sShopName;
        pResponse["result"]["companyName"] = entity.contentEntity.sCompanyName;

        pResponse["result"]["shopPhone"] =  entity.contentEntity.sShopPhone;
        pResponse["result"]["business"] = entity.contentEntity.sBusiness;
        pResponse["result"]["shopType"] = entity.contentEntity.sShopType;

        pResponse["result"]["offLineDisCount"] =  entity.contentEntity.iOffLineDisCount;
        pResponse["result"]["licensePhoto"] = entity.contentEntity.sLicensePhoto;
        pResponse["result"]["idPhoto"] = entity.contentEntity.sIdPhoto;

        pResponse["result"]["shopFrontPhoto"] =  entity.contentEntity.sShopFrontPhoto;
        pResponse["result"]["shopDetailPhoto"] = entity.contentEntity.sShopDetailPhoto;
        pResponse["result"]["address"] = entity.contentEntity.sAddress;

        pResponse["result"]["city"] =  entity.contentEntity.sCtiy;
        pResponse["result"]["gpsLat"] = entity.contentEntity.dGpsLat;
        pResponse["result"]["gpsLon"] = entity.contentEntity.dGpsLon;

        pResponse["result"]["hours"] =  entity.contentEntity.sHours;
        pResponse["result"]["posNum"] = entity.contentEntity.sPosNum;
        pResponse["result"]["perAgentId"] =tConvert.LongToStr( entity.contentEntity.lPerAgentId);

        pResponse["result"]["awardingPoints"] =  entity.contentEntity.iAwardingPoints;
        pResponse["result"]["sevicePhone"] = entity.contentEntity.sSevvicePhone;


        pResponse["result"]["avgConsume"] =  entity.contentEntity.iAvgConsume;
        pResponse["result"]["showNearby"] = entity.contentEntity.iShowNearby;
        pResponse["result"]["prorate"] = entity.contentEntity.iProrate;
        pResponse["result"]["userId"] =  tConvert.LongToStr(  entity.contentEntity.lUserId );
        pResponse["result"]["verify"] = entity.contentEntity.iVerify;
        pResponse["result"]["auth"] = entity.contentEntity.iAuth;
        pResponse["result"]["otherCerPhoto"] = entity.contentEntity.sOtherCerPhoto;
        pResponse["result"]["tradeMark"] = entity.contentEntity.sTradeMark;
        pResponse["result"]["ticketScale"] = GetGiveCoin( entity.contentEntity.iOffLineDisCount);
        pResponse["result"]["coinType"] = entity.contentEntity.iCoinType;

        BankEntity bank;

        mBankDal.GetEntity( tConvert.StrToLong( entity.sPrimaryKey),bank);
        pResponse["result"]["bankAccount"] = bank.sAccount;
        pResponse["result"]["bank"] = bank.sBank;
        pResponse["result"]["bankBranch"] = bank.sBranch;
        pResponse["result"]["bankName"] = bank.sName;


        ShopSetting set;
        set.iCashBack = enum_cashback_close;
        set.iCoinBack = enum_coinback_close;
        set.lStartTime = 0;
        set.iRestrict = enum_buyrestrict_false;
        set.iWifi = enum_wifi_close;
        set.iPark = enum_park_close;
        set.iDelivery = enum_delivery_close;
        mShopSettingDal.GetEntity(tConvert.StrToLong( entity.sPrimaryKey),set);
        pResponse["result"]["cashBack"] = set.iCashBack;
        pResponse["result"]["coinBack"] = set.iCoinBack;
        pResponse["result"]["startTime"] = tConvert.TimeToStr( set.lStartTime );
        pResponse["result"]["restrict"] = set.iRestrict ;
        pResponse["result"]["park"] = set.iPark ;
        pResponse["result"]["wifi"] = set.iWifi ;
        pResponse["result"]["delivery"] = set.iDelivery ;

        ShopAdvLst shopadvlst;int iTotal;
        mAdvDAL.GetData(tConvert.StrToLong( entity.sPrimaryKey),1,10,enum_verify_yes,enum_show_yes,shopadvlst,iTotal);
        for( int i = 0; i < shopadvlst.size(); i++){
            pResponse["result"]["advLink"][i] = shopadvlst[i].sAdvLink;
        }
        if( shopadvlst.size() <= 0){
            pResponse["result"]["advLink"] ;
            pResponse["result"]["advLink"].resize(0);
        }

        pResponse["result"]["score"] = GetAvgScore(tConvert.StrToLong( entity.sPrimaryKey),0);



    }

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}

//查询数据-平台号ID或帐号或用户ID
int ShopInfoBLL::GetEntity( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    string sCond ;int iType;
    if( ! pJson["lPlatformId"].isNull()  ){
        sCond = tConvert.Json2String( pJson["lPlatformId"] );
        iType = 1;
    }else if( ! pJson["sAccount"].isNull()  ){
        sCond = tConvert.Json2String( pJson["sAccount"] );
        iType = 2;
    }else if( ! pJson["sPosNum"].isNull()  ){
        sCond = tConvert.Json2String( pJson["sPosNum"] );
        iType = 4;
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    ShopInfoEntity  entity;
    iRet =  ShopInfoDAL::GetInstance()->GetEntity( sCond ,iType,entity);
    if ( 0 == iRet){
        pResponse["entity"]["sPrimaryKey"] = entity.sPrimaryKey;
        pResponse["entity"]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
        //  pResponse["entity"]["lUpTime"] =   tConvert.LongToStr(entity.lUpTime);
        //  pResponse["entity"]["lUpdateId"] = tConvert.LongToStr( entity.lUpdateId);
        pResponse["entity"]["lServiceUserId"] =  tConvert.LongToStr( entity.contentEntity.lServiceUserId);
        pResponse["entity"]["sAccount"] = entity.contentEntity.sAccount;
        pResponse["entity"]["sShopName"] = entity.contentEntity.sShopName;
        pResponse["entity"]["sCompanyName"] = entity.contentEntity.sCompanyName;

        pResponse["entity"]["sShopPhone"] =  entity.contentEntity.sShopPhone;
        pResponse["entity"]["sBusiness"] = entity.contentEntity.sBusiness;
        pResponse["entity"]["sShopType"] = entity.contentEntity.sShopType;

        pResponse["entity"]["iOffLineDisCount"] =  entity.contentEntity.iOffLineDisCount;
        pResponse["entity"]["sLicensePhoto"] = entity.contentEntity.sLicensePhoto;
        pResponse["entity"]["sIdPhoto"] = entity.contentEntity.sIdPhoto;

        pResponse["entity"]["sShopFrontPhoto"] =  entity.contentEntity.sShopFrontPhoto;
        pResponse["entity"]["sShopDetailPhoto"] = entity.contentEntity.sShopDetailPhoto;
        pResponse["entity"]["sAddress"] = entity.contentEntity.sAddress;

        pResponse["entity"]["sCtiy"] =  entity.contentEntity.sCtiy;
        pResponse["entity"]["dGpsLat"] = entity.contentEntity.dGpsLat;
        pResponse["entity"]["dGpsLon"] = entity.contentEntity.dGpsLon;

        pResponse["entity"]["sHours"] =  entity.contentEntity.sHours;
        pResponse["entity"]["sPosNum"] = entity.contentEntity.sPosNum;
        pResponse["entity"]["lPerAgentId"] =tConvert.LongToStr( entity.contentEntity.lPerAgentId);

        pResponse["entity"]["iAwardingPoints"] =  entity.contentEntity.iAwardingPoints;
        pResponse["entity"]["sSevvicePhone"] = entity.contentEntity.sSevvicePhone;
        //      pResponse["entity"]["dDistance"] = entity.contentEntity.dDistance;

        pResponse["entity"]["iAvgConsume"] =  entity.contentEntity.iAvgConsume;
        pResponse["entity"]["iShowNearby"] = entity.contentEntity.iShowNearby;
        pResponse["entity"]["iProrate"] = entity.contentEntity.iProrate;
        pResponse["entity"]["iProrate"] = entity.contentEntity.iProrate;
        pResponse["entity"]["lUserId"] =  tConvert.LongToStr(  entity.contentEntity.lUserId );
        pResponse["entity"]["iVerify"] = entity.contentEntity.iVerify;
        pResponse["entity"]["iAuth"] = entity.contentEntity.iAuth;
        pResponse["entity"]["sOtherCerPhoto"] = entity.contentEntity.sOtherCerPhoto;
        pResponse["entity"]["sTradeMark"] = entity.contentEntity.sTradeMark;
        pResponse["entity"]["iTicketScale"] = GetGiveCoin( entity.contentEntity.iOffLineDisCount);
        pResponse["entity"]["iCoinType"] = entity.contentEntity.iCoinType;
        pResponse["entity"]["iCoinPercent"] = entity.contentEntity.iCoinPercent;
        pResponse["entity"]["iPromotionDiscount"] = entity.contentEntity.iPromotionDiscount;

        BankEntity bank;

        mBankDal.GetEntity( tConvert.StrToLong( entity.sPrimaryKey),bank);
        pResponse["entity"]["sBankAccount"] = bank.sAccount;
        pResponse["entity"]["sBank"] = bank.sBank;
        pResponse["entity"]["sBankBranch"] = bank.sBranch;
        pResponse["entity"]["sBankName"] = bank.sName;


        ShopSetting set;
        set.iCashBack = enum_cashback_close;
        set.iCoinBack = enum_coinback_close;
        set.lStartTime = 0;
        set.iRestrict = enum_buyrestrict_false;
        set.iWifi = enum_wifi_close;
        set.iPark = enum_park_close;
        set.iDelivery = enum_delivery_close;
        mShopSettingDal.GetEntity(tConvert.StrToLong( entity.sPrimaryKey),set);
        pResponse["entity"]["iCashBack"] = set.iCashBack;
        pResponse["entity"]["iCoinBack"] = set.iCoinBack;
        pResponse["entity"]["sStartTime"] = tConvert.TimeToStr( set.lStartTime );
        pResponse["entity"]["iRestrict"] = set.iRestrict ;
        pResponse["entity"]["iPark"] = set.iPark ;
        pResponse["entity"]["iWifi"] = set.iWifi ;
        pResponse["entity"]["iDelivery"] = set.iDelivery ;

        ShopAdvLst shopadvlst;int iTotal;
        mAdvDAL.GetData(tConvert.StrToLong( entity.sPrimaryKey),1,10,enum_verify_yes,enum_show_yes,shopadvlst,iTotal);
        for( int i = 0; i < shopadvlst.size(); i++){
            pResponse["entity"]["sAdvLink"][i] = shopadvlst[i].sAdvLink;
        }
        if( shopadvlst.size() <= 0){
            pResponse["entity"]["sAdvLink"] ;
            pResponse["entity"]["sAdvLink"].resize(0);
        }

        pResponse["entity"]["iScore"] = GetAvgScore(tConvert.StrToLong( entity.sPrimaryKey),0);



    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//查询数据-條件查詢(管理后台)(处理带token，不加密的情况)
int ShopInfoBLL::GetData2(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert  tConvert;
    int iRet = -1;
    ShopQueryEntity pQueryEntity;


    if( pJson["page"].isNull() || pJson["count"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pResponse["result"]["shops"].resize(0);


    if( ! pJson["shopName"].isNull()){
        pQueryEntity.sShopName = tConvert.Json2String( pJson["shopName"]);
    }
    if( ! pJson["idPlatform"].isNull()){
        pQueryEntity.sPlatformId = tConvert.Json2String( pJson["idPlatform"]);
    }
    pQueryEntity.iVerify = 1;
    pQueryEntity.sCtiy = "citys";



    int page = tConvert.Json2Int( pJson["page"]);
    int count =  tConvert.Json2Int( pJson["count"] );
    if( count > 20){
        count = 20;
    }

    int piIndex = ( page -1  ) * count;
    ShopInfoList  plsTmp;
    int iTotalCount;
    iRet = ShopInfoDAL::GetInstance()->GetList(pQueryEntity,piIndex,count,plsTmp,iTotalCount);

    for( int i = 0; i < plsTmp.size(); i++){
        ShopInfoEntity & entity = plsTmp[i];

        pResponse["result"]["shops"][i]["idPlatform"]  = entity.sPrimaryKey;
        pResponse["result"]["shops"][i]["shopName"]  = entity.contentEntity.sShopName;
        pResponse["result"]["shops"][i]["shopPhone"]  = entity.contentEntity.sShopPhone;
        pResponse["result"]["shops"][i]["shopType"]  = entity.contentEntity.sShopType;

    }

    //返回总个数
    pResponse["result"]["total"]  = iTotalCount;

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return 0;

}

//查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
int ShopInfoBLL::GetData(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert  tConvert;
    int iRet = -1;
    ShopQueryEntity pQueryEntity;


    if( pJson["iIndex"].isNull() || pJson["iCount"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    if( ! pJson["dGpsLat"].isNull()){
        pQueryEntity.dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);
    }else{
        pQueryEntity.dGpsLat = -1.0;
    }
    if( ! pJson["dGpsLon"].isNull()){
        pQueryEntity.dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);
    }else{
        pQueryEntity.dGpsLon = -1.0;
    }
    if( ! pJson["sCtiy"].isNull()){
        pQueryEntity.sCtiy = tConvert.Json2String( pJson["sCtiy"]);
    }else{
        pQueryEntity.sCtiy = "";
    }
    if( ! pJson["sShopName"].isNull()){
        pQueryEntity.sShopName = tConvert.Json2String( pJson["sShopName"]);
    }else{
        pQueryEntity.sShopName = "";
    }
    if( ! pJson["sShopType"].isNull()){
        pQueryEntity.sShopType = tConvert.Json2String( pJson["sShopType"]);
    }else{
        pQueryEntity.sShopType = "";
    }
    if( ! pJson["iVerify"].isNull()){
        pQueryEntity.iVerify = tConvert.Json2Int( pJson["iVerify"]);
    }else{
        pQueryEntity.iVerify = -1;
    }
    if( ! pJson["iAuth"].isNull()){
        pQueryEntity.iAuth = tConvert.Json2Int( pJson["iAuth"]);
    }else{
        pQueryEntity.iAuth = -1;
    }
    if( ! pJson["sShopPhone"].isNull()){
        pQueryEntity.sShopPhone = tConvert.Json2String( pJson["sShopPhone"]);
    }else{
        pQueryEntity.sShopPhone = "";
    }
    if( ! pJson["lPerAgentId"].isNull()){
        pQueryEntity.lPerAgentId = tConvert.Json2Long( pJson["lPerAgentId"]);
    }else{
        pQueryEntity.lPerAgentId = -1;
    }
    if( ! pJson["iApplyDiscount"].isNull()){
        pQueryEntity.iApplyDiscount = tConvert.Json2Int( pJson["iApplyDiscount"]);
    }else{
        pQueryEntity.iApplyDiscount = -1;
    }
    if( ! pJson["iShowNearby"].isNull()){
        pQueryEntity.iShowNearby = tConvert.Json2Int( pJson["iShowNearby"]);
    }else{
        pQueryEntity.iShowNearby = -1;
    }

    if( !pJson["sStartTime"].isNull()){
        pQueryEntity.lStartTime = tConvert.StrToTime( tConvert.Json2String(pJson["sStartTime"]).c_str() ) * 1e6;

    }else{
        pQueryEntity.lStartTime = -1;

    }

    if(  !pJson["sEndTime"].isNull()){

        pQueryEntity.lEndTime = tConvert.StrToTime( tConvert.Json2String(pJson["sEndTime"]).c_str() ) * 1e6;
    }else{

        pQueryEntity.lEndTime = -1;
    }
    if(  !pJson["sPlatformId"].isNull()){

        pQueryEntity.sPlatformId =  tConvert.Json2String(pJson["sPlatformId"]) ;
    }else{

        pQueryEntity.sPlatformId = "";
    }
    if(  !pJson["lUserId"].isNull()){

        pQueryEntity.lUserId =  tConvert.Json2Long(pJson["lUserId"]) ;
    }else{

        pQueryEntity.lUserId = -1;
    }


    int piIndex = tConvert.Json2Int( pJson["iIndex"]);
    int piCount =  tConvert.Json2Int( pJson["iCount"] );
    if( piCount > 20){
        piCount = 20;
    }
    ShopInfoList  plsTmp;
    int iTotalCount;
    iRet = ShopInfoDAL::GetInstance()->GetData(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);
    GetPhone(plsTmp);


    if( 0 == iRet ){
        DataLst2Json(plsTmp,pResponse,1);
    }else{
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"]  =0;
    //返回总个数
    pResponse["iTotal"]  = iTotalCount;
    return 0;
}

//获取个代手机号
bool ShopInfoBLL::GetPhone(const  ShopInfoList & plsTmp)
{
    Json::Value jPost;int iIndex = 0;TConvert tConvert;
    for( ShopInfoList::const_iterator it = plsTmp.begin(); it != plsTmp.end(); it++, iIndex++){
        const ShopInfoEntity & tmp = *it;

        jPost["lUserIdLst"][iIndex]["lUserId"] = tConvert.LongToStr( tmp.contentEntity.lPerAgentId );
    }

    //URL
    string sUrl = "http://"+ TBLL::GetInstance()->GetMechatIP() +":23241/?sBusiness=UserQuery&sMethod=GetUserInfoList";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jPost.toStyledString());

    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return false;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return false;
    }

    Json::Value jList = jsonReturn["list"];
    for( int i  = 0; i < jList.size(); i++){
        mUserIdPhoneMap[ tConvert.Json2Long( jList[i]["lUserId"])] =  tConvert.Json2String(jList[i]["sAccount"]);
    }



    return true;
}


bool ShopInfoBLL::IsPic(string sAddr)
{
    if( sAddr.length() <= 4)
        return false;

    //    string fix = sAddr.substr(sAddr.length() -4);
    //    if( fix == "jpg" || fix == "png"){
    //        return true;
    //    }
    return true;
}

void ShopInfoBLL::ArrayToString(const Json::Value & j,string & s)
{
    TConvert tConvert;
    for( int i= 0 ; i < j.size(); i++){
        s += (tConvert.Json2String(j[i]) + ",");
    }
    s = s.substr(0,s.length() -1);

}

//商家资料完整判断
bool ShopInfoBLL::IsShopPerfect(const ShopInfoEntity & entity)
{
    if( entity.contentEntity.sSevvicePhone.empty()){
        return false;
    }
    if( entity.contentEntity.sShopType.empty()){
        return false;
    }
    if( entity.contentEntity.iOffLineDisCount < 0){
        return false;
    }
    if( entity.contentEntity.iCoinType < 0){
        return false;
    }

    return true;
}

//获取附近联盟商家
int ShopInfoBLL::GetNearBy(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert  tConvert;
    int iRet = -1;
    ShopQueryEntity pQueryEntity;
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull()  ||
            pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }


    pQueryEntity.dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);

    pQueryEntity.dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);


    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int piCount =  tConvert.Json2Int( pJson["iCount"] );
    if( piCount > 20){
        piCount = 20;
    }
    int  piIndex = (iPage -1 )* piCount;

    pQueryEntity.iApplyDiscount = -1;
    pQueryEntity.iAuth = -1;
    pQueryEntity.iShowNearby  =1;//1显示
    pQueryEntity.iVerify = 1;//1 审核通过
    pQueryEntity.lEndTime = -1;
    pQueryEntity.lPerAgentId = -1;
    pQueryEntity.lStartTime = -1;


    if( !pJson["bStar"].isNull() ){
        pQueryEntity.bStar = tConvert.Json2Bool(pJson["bStar"]);
    }else{
        pQueryEntity.bStar = false;
    }


    pQueryEntity.sCtiy = "citys";
    if( pQueryEntity.bStar){//如果用户选了星级排序，需要匹配区域
        if( !pJson["sCity"].isNull() ){
            pQueryEntity.sCtiy = tConvert.Json2String(pJson["sCity"]);
            if( pQueryEntity.sCtiy == "深圳龙华区"){
                pQueryEntity.sCtiy = "深圳龙华新区";
            }
        }
    }

    if( !pJson["iAuth"].isNull() ){
        pQueryEntity.iAuth = tConvert.Json2Int(pJson["iAuth"]);
    }else{
        pQueryEntity.iAuth = -1;
    }


    if( !pJson["iEnumDiscount"].isNull() ){
        pQueryEntity.iEnumDiscount = tConvert.Json2Int(pJson["iEnumDiscount"]);
    }else{
        pQueryEntity.iEnumDiscount = -1;
    }

    ShopInfoList  plsTmp;
    int iTotalCount;

    iRet = ShopInfoDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

    if( 0 == iRet ){
        DataLst2Json(plsTmp,pResponse,2);
        //返回总个数
        pResponse["iTotal"]  = iTotalCount;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


//获取热门商家
int ShopInfoBLL::HotShops( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull() || pJson["sCity"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    pResponse["list"] ;
    pResponse["list"].resize(0);

    double dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);

    double dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);

    string sCity = tConvert.Json2String( pJson["sCity"]);

    Json::Value  jsonPost;
    jsonPost["x"] = dGpsLon;
    jsonPost["y"] = dGpsLat;
    jsonPost["sCity"] = sCity;


    Json::Value jsonResponse;

    iRet =  Requst("http://mepay.tymplus.com/GetDataInterface/PointShop/TradingTimes_PlatformIdList.aspx",jsonPost,jsonResponse);


    if( iRet == 0){

        Json::Value jPlatformIds = jsonResponse["list"];


        ShopInfoList lst;
        int iNum = 0;
        for( int i = 0; i < jPlatformIds.size() ; i++){
            ShopInfoEntity entity;
            iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.Json2String(jPlatformIds[i]),1,entity);
            if( iRet == 0){
                entity.contentEntity.lDistance = (long)GetDistance(dGpsLat,dGpsLon,entity.contentEntity.dGpsLat,entity.contentEntity.dGpsLon);
                lst.push_back(entity);
                iNum++;
            }
            if( iNum == 3){
                break;
            }
        }
        DataLst2Json(lst,pResponse,2);
        RetJsonMsg::SetRetMsg(0,pResponse);

    }else{

        RetJsonMsg::SetRetMsg(iRet,pResponse);

        if( !jsonResponse["err"].isNull())
            pResponse["sMsg"] = jsonResponse["err"];

    }

}


//获取乐兑商品（分页）(APP调用)
int ShopInfoBLL::Products( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if( pJson["sSearch"].isNull()  || pJson["iPage"].isNull()
            ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert  tConvert;int iRet = -1;
    string sSearch = tConvert.Json2String( pJson["sSearch"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);

    Json::Value jProducts;
    iRet = GetProduct(iPage,1,sSearch,jProducts);
    if( 0 == iRet){
        pResponse["sProducts"] = jProducts["result"];
        RetJsonMsg::SetRetMsg(0,pResponse);
    }else{
        pResponse["sProducts"];
        pResponse["sProducts"].resize(0);
        pResponse["sMsg"] = jProducts["result"];
        pResponse["iRet"] = iRet;
    }
    return iRet;
}

//获取联盟商家商品（分页）(APP调用)
int ShopInfoBLL::ShowProduct( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if( pJson["lPlatformId"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()
            ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert  tConvert;
    int iRet = -1;
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }

    ShopInfoEntity shop;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(lPlatformId),1,shop);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    int iDiscount = shop.contentEntity.iOffLineDisCount;
    if(iDiscount <= 0){
        iDiscount = 100;
    }
    ShowProLst lst;int iTotalCount;
    mShowDAL.GetData(lPlatformId,iPage,iCount,enum_verify_yes,enum_show_yes,"",lst,iTotalCount);

    for( int i  =0 ; i < lst.size() ; i++){
        ShowPro & entity = lst[i];
        pResponse["list"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["list"][i]["lPlatformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["list"][i]["sName"] =  entity.sName ;
        pResponse["list"][i]["sPicLink"] =  entity.sPicLink ;
        pResponse["list"][i]["dPrice"] =  entity.dPrice ;
        pResponse["list"][i]["dCoin"] =  entity.dPrice * GetGiveCoin(iDiscount)/100;
        pResponse["list"][i]["dCoin"] =  entity.dCoin;
        pResponse["list"][i]["iCoinType"] =  entity.iCoinType;
    }
    if( lst.size() == 0){
        pResponse["list"];
        pResponse["list"].resize(0);
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

//获取联盟商家商品（分页）处理带token，不加密的情况
int ShopInfoBLL::ShowProduct2( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    if( pJson["page"].isNull()  ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    pResponse["result"]["product"].resize(0);


    QueryShowPro query;
    if( ! pJson["shopName"].isNull()){
        query.sShopName = tConvert.Json2String( pJson["shopName"]);
    }
    if( ! pJson["idPlatform"].isNull()){
        query.sPlatformId = tConvert.Json2String( pJson["idPlatform"]);
    }



    int page = tConvert.Json2Int( pJson["page"]);
    int count = 10;



    ShowProLst showlst;int total;
    mShowDAL.GetData(query,page,count,showlst,total);

    for(int i = 0; i < showlst.size(); i++){
        ShowPro & entity = showlst[i];
        pResponse["result"]["product"][i]["coin"] = entity.dCoin;
        pResponse["result"]["product"][i]["price"] = entity.dPrice;
        pResponse["result"]["product"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["result"]["product"][i]["idPlatform"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["result"]["product"][i]["shopName"] = entity.sShopName;
        pResponse["result"]["product"][i]["name"] = entity.sName;
        pResponse["result"]["product"][i]["picLink"] = entity.sPicLink;
        pResponse["result"]["product"][i]["coinType"] = entity.iCoinType;
    }
    pResponse["result"]["total"] = total;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;


}


//查询数据（APP调用）
int ShopInfoBLL::GetPage(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    if( pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull()  || pJson["iPage"].isNull() || pJson["iCount"].isNull()
            ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert  tConvert;
    int iRet = -1;
    ShopQueryEntity pQueryEntity;
    pQueryEntity.dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);
    pQueryEntity.dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);




    if( !pJson["sShopName"].isNull()  ){
        pQueryEntity.sShopName = tConvert.Json2String( pJson["sShopName"]);
    }else{
        pQueryEntity.sShopName.clear();
    }

    if( !pJson["sShopType"].isNull()){
        pQueryEntity.sShopType = tConvert.Json2String( pJson["sShopType"]);
    }else{
        pQueryEntity.sShopType.clear();
    }
    if( !pJson["sCity"].isNull() ){
        pQueryEntity.sCtiy = tConvert.Json2String( pJson["sCity"]);
        if( pQueryEntity.sCtiy == "深圳龙华区"){
            pQueryEntity.sCtiy = "深圳龙华新区";
        }
    }else{
        pQueryEntity.sCtiy = "citys";
    }

    if( !pJson["sStartTime"].isNull()){
        pQueryEntity.lStartTime = tConvert.StrToTime( tConvert.Json2String(pJson["sStartTime"]).c_str() ) * 1e6;

    }else{
        pQueryEntity.lStartTime = -1;

    }

    if(  !pJson["sEndTime"].isNull()){

        pQueryEntity.lEndTime = tConvert.StrToTime( tConvert.Json2String(pJson["sEndTime"]).c_str() ) * 1e6;
    }else{

        pQueryEntity.lEndTime = -1;
    }


    pQueryEntity.iAuth = -1;
    pQueryEntity.iVerify = 1;

    pQueryEntity.sShopPhone = "";
    pQueryEntity.lPerAgentId = -1;
    pQueryEntity.iApplyDiscount = -1;
    pQueryEntity.iShowNearby = 1;


    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int piCount =  tConvert.Json2Int( pJson["iCount"] );
    if( piCount > 20){
        piCount = 20;
    }
    int piIndex = (iPage-1)*piCount;
    ShopInfoList  plsTmp;
    int iTotalCount;
    iRet = ShopInfoDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);
    if( 0 == iRet ){
        DataLst2Json(plsTmp,pResponse,2);
    }else{
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    pResponse["iRet"]  =0;
    //返回总数
    pResponse["iTotal"]  = iTotalCount;


    pResponse["sProducts"];
    pResponse["sProducts"].resize(0);
    if( 1== iPage){
        Json::Value jProduct;
        string sSearch = pQueryEntity.sShopName;
        if( sSearch.empty()){
            sSearch = pQueryEntity.sShopType;
        }
        if(GetProduct(1,0,sSearch,jProduct) == 0){
            if(jProduct["result"].size() > 2){
                for(int i = 0 ; i < 2; i++){
                    pResponse["sProducts"][i] = jProduct["result"][i];
                }
            }else{
                pResponse["sProducts"] = jProduct["result"];
            }
        }
    }
    return 0;

}

//获取我的平台列表
int ShopInfoBLL::GetMyShop( const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert tConvert;
    int iRet = -1;
    if(  pJson["lUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lUserId = tConvert.Json2Long( pJson["lUserId"] );
    int iType = 0;
    if( !pJson["iType"].isNull() ){
        iType = tConvert.Json2Int( pJson["iType"] );
    }
    ShopInfoList  plsTmp;
    iRet = ShopInfoDAL::GetInstance()->GetMyShop(lUserId, iType,plsTmp);
    if( 0 == iRet ){
        DataLst2Json(plsTmp,pResponse,1);
    }else{
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    //
    pResponse["iRet"]  = 0;
    return 0;
}

//商家资料完整判断
int ShopInfoBLL::ShopPerfect(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;
    if(  pJson["lUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pResponse["list"] ;
    pResponse["list"].resize(0);

    long lUserId = tConvert.Json2Long( pJson["lUserId"] );
    int iType = 0;//all
    if( !pJson["iType"].isNull() ){
        iType = tConvert.Json2Int( pJson["iType"] );
    }
    ShopInfoList  plsTmp;
    iRet = ShopInfoDAL::GetInstance()->GetMyShop(lUserId, iType,plsTmp);
    if( 0 != iRet ){

        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    int iCount = 0;
    for( int i = 0; i < plsTmp.size(); i++){
        ShopInfoEntity & en = plsTmp[i];
        bool bPerfect = IsShopPerfect(en);
        if( !bPerfect){
            pResponse["list"][iCount++]["lPlatformId"] =en.sPrimaryKey;
        }
    }
    pResponse["sTips"] = "尊敬的用户,您有一些联盟商家的资料尚未补全,需补全后才能正常开通店铺";
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

int ShopInfoBLL::ModifyPwd(const  Json::Value  & pJson ,Json::Value & pResponse){

    TConvert tConvert;
    int iRet =-1;
    if(pJson["sPrimaryKey"].isNull()  || pJson["sNewPwd"].isNull()  || pJson["sOldPwd"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sPrimaryKey = tConvert.Json2String( pJson["sPrimaryKey"] );
    string sNewPwd =tConvert.Json2String( pJson["sNewPwd"] );
    string sOldPwd = tConvert.Json2String( pJson["sOldPwd"] );
    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(sPrimaryKey,1,entity);
    if( iRet ==0){
        if( entity.contentEntity.sPassWord != sOldPwd){
            RetJsonMsg::SetRetMsg(-6,pResponse);
            return -6;
        }

        //修改
        if ( 0 != ShopInfoDAL::GetInstance()->ModifyPwd( sPrimaryKey, sNewPwd,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


int ShopInfoBLL::ModifyPwd2(long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse){

    TConvert tConvert;
    int iRet =-1;
    if( pJson["new"].isNull()  || pJson["old"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sPrimaryKey = tConvert.LongToStr( lPlatformId );
    string sNewPwd =tConvert.Json2String( pJson["new"] );
    string sOldPwd = tConvert.Json2String( pJson["old"] );
    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(sPrimaryKey,1,entity);
    if( iRet ==0){
        if( entity.contentEntity.sPassWord != sOldPwd){
            RetJsonMsg::SetRetMsg2(-6,pResponse);
            return -6;
        }

        //修改
        if ( 0 != ShopInfoDAL::GetInstance()->ModifyPwd( sPrimaryKey, sNewPwd,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg2(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}



//重置密码
int ShopInfoBLL::ResetPwd( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet =-1;
    if(pJson["lPlatformId"].isNull()  || pJson["sNewPwd"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sPrimaryKey = tConvert.Json2String( pJson["lPlatformId"] );
    string sNewPwd =tConvert.Json2String( pJson["sNewPwd"] );
    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(sPrimaryKey,1,entity);
    if( iRet ==0){
        //修改
        if ( 0 != ShopInfoDAL::GetInstance()->ModifyPwd( sPrimaryKey, sNewPwd,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


//使用验证码重置密码
int ShopInfoBLL::Reset( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet =-1;
    if(pJson["lPlatformId"].isNull()  || pJson["sNewPwd"].isNull()  || pJson["sCode"].isNull() || pJson["sShopPhone"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sPrimaryKey = tConvert.Json2String( pJson["lPlatformId"] );
    string sNewPwd =tConvert.Json2String( pJson["sNewPwd"] );
    string sCode =tConvert.Json2String( pJson["sCode"] );
    string sShopPhone =tConvert.Json2String( pJson["sShopPhone"] );

    //手机验证码
    if ( ! IsUserPhoneCode(sShopPhone,sCode) ) {
        RetJsonMsg::SetRetMsg(-8,pResponse);
        return -1;
    }

    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(sPrimaryKey,1,entity);
    if( iRet ==0){
        //修改
        if ( 0 != ShopInfoDAL::GetInstance()->ModifyPwd( sPrimaryKey, sNewPwd,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//申请变更折扣
int ShopInfoBLL::ApplyDiscount( const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert tConvert;
    int iRet =-1;
    if(pJson["lPlatformId"].isNull()  || pJson["iDiscount"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"] );
    int iDiscount =tConvert.Json2Int( pJson["iDiscount"] );

    if( iDiscount <=0 || iDiscount >= 100 ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr( lPlatformId),1,entity);
    if( iRet ==0){
        char buff[8] = {0};
        sprintf( buff , "%02d",iDiscount);
        string sOldRemark = entity.contentEntity.sRemark;
        string sRemark;
        if( sOldRemark.length() >2){
            sRemark = buff + entity.contentEntity.sRemark.substr(2);
        }else{
            sRemark = buff;
        }
        if ( 0 != ShopInfoDAL::GetInstance()->UpRemark( lPlatformId, sRemark,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//申请变更折扣,处理带token，不加密的情况
int ShopInfoBLL::ApplyDiscount2( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert tConvert;
    int iRet =-1;
    if( pJson["discount"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    int iDiscount =tConvert.Json2Int( pJson["discount"] );

    if( iDiscount <=0 || iDiscount >= 100 ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr( lPlatformId),1,entity);
    if( iRet ==0){
        char buff[8] = {0};
        sprintf( buff , "%02d",iDiscount);
        string sOldRemark = entity.contentEntity.sRemark;
        string sRemark;
        if( sOldRemark.length() >2){
            sRemark = buff + entity.contentEntity.sRemark.substr(2);
        }else{
            sRemark = buff;
        }
        if ( 0 != ShopInfoDAL::GetInstance()->UpRemark( lPlatformId, sRemark,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}

//获取收款记录,处理带token，不加密的情况
int ShopInfoBLL::GetTrade2(const string & sPlatformId, const string & sStartTime, const string & sEndTime
                           ,const string & sPayerPhone,const string & sPage,Json::Value & pResponse)
{
    TConvert  tConvert;
    int iRet = -1;

    Json::Value  jsonPost;
    jsonPost["lPlatformId"] = sPlatformId;
    jsonPost["sStartTime"] = sStartTime;
    jsonPost["sEndTime"] = sEndTime;
    if( !sPayerPhone.empty())
        jsonPost["sPayerPhone"] = sPayerPhone;
    jsonPost["iPageSize"] = 10;
    jsonPost["iPageIndex"] = sPage;


    Json::Value jsonResponse;
    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() +
            ":10001/GetDataInterface/Consume/ReportForm/GetRecRecordByTimeSlot.aspx";
    iRet =  Requst(sUrl,jsonPost,jsonResponse);




    iRet = tConvert.Json2Int( jsonResponse["iRet"] );
    string err = tConvert.Json2String(jsonResponse["err"]);
    pResponse["state"] = tConvert.IntToStr( iRet );
    if( 0 != iRet){
        pResponse["result"] = err;
    }else{

        pResponse["result"]["listRecRecord"] = jsonResponse["listRecRecord"];
        pResponse["result"]["pageCount"] = jsonResponse["iPageCount"];
        pResponse["result"]["totalMoney"] = jsonResponse["dTotalMoney"];
    }
    return iRet;



}

//通过变更申请
int ShopInfoBLL::ChangeDiscount( const  Json::Value  & pJson ,Json::Value & pResponse  )
{
    TConvert tConvert;
    int iRet =-1;
    if(pJson["lPlatformId"].isNull()  || pJson["iPass"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"] );
    int iPass = tConvert.Json2Int( pJson["iPass"] );//1 -通过   2-不通过
    long lUpdateId = tConvert.GetCurrentTime();
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr( lPlatformId),1,entity);
    if( iRet ==0){
        string sOldRemark = entity.contentEntity.sRemark;
        if( sOldRemark.substr(0,2) == "00"){
            RetJsonMsg::SetRetMsg(-1,pResponse);
            return -1;
        }
        string sRemark;
        if( sOldRemark.length() > 2){
            sRemark = "00" + sOldRemark.substr(2);
        }else{
            sRemark = "00";
        }
        int iDiscount = entity.contentEntity.iOffLineDisCount;
        if( iPass == 1){
            iDiscount =  atoi( entity.contentEntity.sRemark.substr(0,2).c_str());
        }


        if ( 0 != ShopInfoDAL::GetInstance()->UpDiscount( lPlatformId, iDiscount, sRemark,lUpdateId ) ) {
            RetJsonMsg::SetRetMsg(-4,pResponse);
            return -4;
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//创建平台
int ShopInfoBLL::CreateShop(const  Json::Value  & pJson ,Json::Value & pResponse)
{

    TConvert tConvert;
    int iRet = -1;

    if(  pJson["lUserId"].isNull()   || pJson["sShopName"].isNull()  || pJson["sCompanyName"].isNull()
         || pJson["sShopPhone"].isNull()  || pJson["sBusiness"].isNull() || pJson["sShopType"].isNull()  || pJson["iOffLineDisCount"].isNull()
         || pJson["sLicensePhoto"].isNull() || pJson["sIdPhoto"].isNull()  || pJson["sShopFrontPhoto"].isNull()
         || pJson["sShopDetailPhoto"].isNull() || pJson["sAddress"].isNull()  || pJson["sCtiy"].isNull()
         || pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull()  || pJson["sHours"].isNull()
         || pJson["sPosNum"].isNull() || pJson["lPerAgentId"].isNull()  || pJson["iAwardingPoints"].isNull()
         || pJson["lServiceUserId"].isNull() || pJson["sSevvicePhone"].isNull()  || pJson["iAvgConsume"].isNull()
         || pJson["iShowNearby"].isNull() || pJson["iProrate"].isNull()  ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return ERRPOSTPARAM;
    }

    ShopInfoEntity entity;
    ShopInfoEntity entityShop;
    entity.contentEntity.sShopName = tConvert.Json2String( pJson["sShopName"]);
    iRet = ShopInfoDAL::GetInstance()->GetEntity(entity.contentEntity.sShopName,5,entityShop);
    if( iRet == 0 ){//店名已存在
        RetJsonMsg::SetRetMsg(-102 ,pResponse);
        return -102;
    }

    long lMaxPlatformId;
    iRet = ShopInfoDAL::GetInstance()->GetMaxPlatformId( lMaxPlatformId);
    if(iRet != 0){
        RetJsonMsg::SetRetMsg(-1 ,pResponse);
        return -1;
    }


    long lCurrentTime = tConvert.GetCurrentTime();
    long lUpdateId = lCurrentTime;
    entity.lAddTime = lCurrentTime;
    entity.lUpTime = lCurrentTime;
    entity.iState = 0;

    entity.lUpdateId = lUpdateId;
    entity.sPrimaryKey =  tConvert.LongToStr(++lMaxPlatformId);
    entity.contentEntity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
    entity.contentEntity.dGpsLat =tConvert.Json2Double( pJson["dGpsLat"]);
    entity.contentEntity.dGpsLon =tConvert.Json2Double( pJson["dGpsLon"]);
    entity.contentEntity.iOffLineDisCount = tConvert.Json2Int( pJson["iOffLineDisCount"]);
    entity.contentEntity.lServiceUserId = tConvert.Json2Long( pJson["lServiceUserId"]);
    entity.contentEntity.sCtiy =tConvert.Json2String(  pJson["sCtiy"]);
    entity.contentEntity.iCoinType = 0;
    if( entity.contentEntity.sCtiy == "美支付供应商" ){
        entity.contentEntity.iVerify = 0;// 美支付供应商未审核
    }else{
        entity.contentEntity.iVerify = 1;// 联盟商家默认审核通过
    }


    char in_utf8[128] = {0};
    char out_gb2312[255] = {0};
    char out_ping[256] = {0};
    strncpy(in_utf8,entity.contentEntity.sCtiy.c_str(),128);
    //unicode码转为gb2312码
    //tConvert.u2g(in_utf8,strlen(in_utf8),out_gb2312,255);
    tConvert.utf8togb2312(in_utf8,strlen(in_utf8), out_gb2312,255);
    //得到地名拼音
    pinyin_gb2312(out_gb2312, out_ping, true, false, false, false, false);
    string sAccount;
    string sPing =out_ping;
    ShopInfoDAL::GetInstance()->GetMaxCityAccount(sPing,sAccount);
    int iIndex = 1;
    if( sAccount.find(sPing) != string::npos ){
        sPing = GetPinFromAccount(sAccount);
        string sNum = sAccount.substr(sPing.length());
        iIndex = atoi( sNum.c_str() );
        iIndex++;
    }
    sAccount = sPing + tConvert.IntToStr( iIndex);
    entity.contentEntity.sAccount = sAccount;
    string sOrgPwd ;
    if( entity.contentEntity.sCtiy != "美支付供应商" ){
        sOrgPwd = MyUtil::MakeRandNum(6);
    }else{
        sOrgPwd  = "888888";
    }
    entity.contentEntity.sPassWord = Md5ToLower("pay" + sOrgPwd ) ;
    entity.contentEntity.sAddress =tConvert.Json2String( pJson["sAddress"]);
    entity.contentEntity.iAwardingPoints =tConvert.Json2Int(  pJson["iAwardingPoints"]);
    entity.contentEntity.sBusiness =tConvert.Json2String( pJson["sBusiness"]);
    entity.contentEntity.sCompanyName =tConvert.Json2String(  pJson["sCompanyName"]);

    entity.contentEntity.lPerAgentId = tConvert.Json2Long( pJson["lPerAgentId"]);
    entity.contentEntity.sHours =tConvert.Json2String( pJson["sHours"]);
    entity.contentEntity.sIdPhoto = tConvert.Json2String( pJson["sIdPhoto"]);
    entity.contentEntity.sLicensePhoto = tConvert.Json2String( pJson["sLicensePhoto"]);
    entity.contentEntity.sPosNum = tConvert.Json2String( pJson["sPosNum"]);
    entity.contentEntity.sSevvicePhone = tConvert.Json2String( pJson["sSevvicePhone"]);
    entity.contentEntity.sShopDetailPhoto =tConvert.Json2String(  pJson["sShopDetailPhoto"]);
    entity.contentEntity.sShopFrontPhoto = tConvert.Json2String( pJson["sShopFrontPhoto"]);
    entity.contentEntity.sShopPhone =tConvert.Json2String( pJson["sShopPhone"]);
    entity.contentEntity.sShopType = tConvert.Json2String( pJson["sShopType"]);
    entity.contentEntity.iAvgConsume =tConvert.Json2Int(  pJson["iAvgConsume"]);
    entity.contentEntity.iShowNearby = tConvert.Json2Int( pJson["iShowNearby"]);
    entity.contentEntity.iProrate = tConvert.Json2Int( pJson["iProrate"]);
    if( !pJson["sOtherCerPhoto"].isNull()){
        entity.contentEntity.sOtherCerPhoto = tConvert.Json2String(pJson["sOtherCerPhoto"]);
    }
    //判断开通人是不是个代
    PersonalAgentEntity perAgtEntity;
    iRet = PersonalAgentDAL::GetInstance()->GetEntityByKey(tConvert.LongToStr(entity.contentEntity.lPerAgentId),1,perAgtEntity);
    if( iRet == 0){
        entity.contentEntity.iProrate = 0;
    }else{
        entity.contentEntity.iProrate = 1;
    }
    //添加
    iRet = ShopInfoDAL::GetInstance()->Insert( entity ) ;
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::CreateShop error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }

    pResponse["iRet"] = 0;
    pResponse["sPlatformId"] = entity.sPrimaryKey;
    pResponse["sAccount"] = entity.contentEntity.sAccount;

    appendlog(TTDLogger::LOG_INFO,"ShopInfoBLL::CreateShop success platform:%s pwd:%s", entity.sPrimaryKey.c_str(), sOrgPwd.c_str());


    if( entity.contentEntity.sCtiy != "美支付供应商" ){
        //发送商家开通通知
        string sMsg = entity.sPrimaryKey;
        sMsg += ",";
        sMsg += sOrgPwd;
        SendPhoneNotifyCode(entity.contentEntity.sShopPhone, sMsg,"11");
        SendNotify(entity.contentEntity.lUserId, entity.sPrimaryKey, sOrgPwd);

        //申请平台通过时，让平台主关注自己的平台号
        if(entity.contentEntity.iVerify == 1){
            bool bRet = Attention(entity.contentEntity.lUserId,tConvert.StrToLong( entity.sPrimaryKey));
            if( !bRet){
                appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::Update Attention err [PlatformId:%s][lUserId:%ld]", entity.sPrimaryKey.c_str(),entity.contentEntity.lUserId);
            }
        }

    }

    return 0;
}

//开店（APP调用）
int ShopInfoBLL::CreateShopApp( const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if(  pJson["lLoginId"].isNull() || pJson["sShopName"].isNull() || pJson["sCompanyName"].isNull()
         || pJson["sShopPhone"].isNull()
         || pJson["sLicensePhoto"].isNull() || pJson["sIdPhoto"].isNull()  || pJson["sShopFrontPhoto"].isNull()
         || pJson["sAddress"].isNull()  || pJson["sCity"].isNull()
         || pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull()
         || pJson["iShowNearby"].isNull() ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return ERRPOSTPARAM;
    }

    if( !pJson["sLicensePhoto"].isArray() || !pJson["sIdPhoto"].isArray()  || !pJson["sShopFrontPhoto"].isArray()){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return ERRPOSTPARAM;
    }

    ShopInfoEntity entity;
    ShopInfoEntity entityShop;

    entity.contentEntity.iShowNearby = tConvert.Json2Int( pJson["iShowNearby"]);
    if( entity.contentEntity.iShowNearby != 0 && entity.contentEntity.iShowNearby != 1){
        RetJsonMsg::SetRetMsg(-1 ,pResponse,"ShowNearby err");
        return -2;
    }
    if( !pJson["iCoinType"].isNull()){
        entity.contentEntity.iCoinType = tConvert.Json2Int( pJson["iCoinType"]);
        if( entity.contentEntity.iCoinType < -1 || entity.contentEntity.iCoinType > 1){
            RetJsonMsg::SetRetMsg(-1 ,pResponse,"CoinType err");
            return -2;
        }
    }else{
        entity.contentEntity.iCoinType = -1;
    }

    entity.contentEntity.sAddress =tConvert.Json2String( pJson["sAddress"]);

    if(entity.contentEntity.sAddress.empty()){//地址不能为空
        RetJsonMsg::SetRetMsg(-1 ,pResponse,"Address err");
        return -2;
    }

    string sAgent = "龙华区";
    if( entity.contentEntity.sAddress.find("深圳") != string::npos &&
            entity.contentEntity.sAddress.find(sAgent) != string::npos){
        entity.contentEntity.sAddress = entity.contentEntity.sAddress.replace(entity.contentEntity.sAddress.find(sAgent),sAgent.length(),"龙华新区");
    }


    entity.contentEntity.lPerAgentId = tConvert.Json2Long( pJson["lLoginId"]);//个代就是登录者Id

    entity.contentEntity.sShopPhone = tConvert.Json2String( pJson["sShopPhone"]);
    //短信验证码在客服端作验证了
    // string sCode = tConvert.Json2String( pJson["sCode"]);
    //是否正确的验证码
    //    iRet = UserPhoneCode(entity.contentEntity.sShopPhone, sCode);
    //    if( iRet != 0){
    //        RetJsonMsg::SetRetMsg(-8 ,pResponse);
    //        return -8;
    //    }

    entity.contentEntity.sCompanyName = tConvert.Json2String( pJson["sCompanyName"]);
    entity.contentEntity.sShopName = tConvert.Json2String( pJson["sShopName"]);
    iRet = ShopInfoDAL::GetInstance()->GetEntity(entity.contentEntity.sShopName,5,entityShop);
    if( iRet == 0 ){//店名已存在
        RetJsonMsg::SetRetMsg(-102 ,pResponse);
        return -102;
    }
    entity.contentEntity.sShopPhone = tConvert.Json2String( pJson["sShopPhone"]);
    //手机号必须注册了天天兑用户
    iRet = GetUserInfoByPhone(entity.contentEntity.sShopPhone, entity.contentEntity.lUserId);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(-5 ,pResponse,"店主手机号还没注册");
        return -5;
    }


    string sPerPhone;
    GetUserInfoById(entity.contentEntity.lPerAgentId,sPerPhone);


    //判断是不是个代
    string sWalletUrl = "http://guanli.tymplus.com/GetDataInterface/GetRoleByPhone.aspx";
    Json::Value jWalletPost,jWalletRespons;
    jWalletPost["sPhone"] = sPerPhone;
    iRet = Request::WalletHttpPost(sWalletUrl,jWalletPost,jWalletRespons);
    bool bIsPerAgent  = false,bIsZoneAgent = false;
    if( iRet == 0){
        bIsPerAgent = tConvert.Json2Bool(  jWalletRespons["bIsPerAgent"] );
        bIsZoneAgent = tConvert.Json2Bool(  jWalletRespons["bIsZoneAgent"] );
    }
    if( !bIsPerAgent){
        RetJsonMsg::SetRetMsg(-1 ,pResponse,"非个代无法开店");
        return -1;
    }



    long lMaxPlatformId;
    iRet = ShopInfoDAL::GetInstance()->GetMaxPlatformId( lMaxPlatformId);
    if(iRet != 0){
        RetJsonMsg::SetRetMsg(-1 ,pResponse);
        return -1;
    }
    long lCurrentTime = tConvert.GetCurrentTime();
    long lUpdateId = lCurrentTime;
    entity.lAddTime = lCurrentTime;
    entity.lUpTime = lCurrentTime;


    entity.lUpdateId = lUpdateId;
    entity.sPrimaryKey =  tConvert.LongToStr(++lMaxPlatformId);

    entity.contentEntity.dGpsLat =tConvert.Json2Double( pJson["dGpsLat"]);
    entity.contentEntity.dGpsLon =tConvert.Json2Double( pJson["dGpsLon"]);
    if( !pJson["iOffLineDisCount"].isNull()){
        entity.contentEntity.iOffLineDisCount = tConvert.Json2Int( pJson["iOffLineDisCount"]);
    }else{
        entity.contentEntity.iOffLineDisCount = -1;
    }
    entity.contentEntity.lServiceUserId = 0;
    entity.contentEntity.sCtiy =tConvert.Json2String(  pJson["sCity"]);
    if( entity.contentEntity.sCtiy == "美支付供应商" ){
        entity.contentEntity.iVerify = 0;// 美支付供应商未审核
    }else{
        entity.contentEntity.iVerify = 1;// 联盟商家默认审核通过
    }


    char in_utf8[128] = {0};
    char out_gb2312[255] = {0};
    char out_ping[256] = {0};
    strncpy(in_utf8,entity.contentEntity.sCtiy.c_str(),128);
    //unicode码转为gb2312码
    //tConvert.u2g(in_utf8,strlen(in_utf8),out_gb2312,255);
    tConvert.utf8togb2312(in_utf8,strlen(in_utf8), out_gb2312,255);
    //得到地名拼音


    pinyin_gb2312(out_gb2312, out_ping, true, false, false, false, false);
    string sAccount;
    string sPing =out_ping;
    ShopInfoDAL::GetInstance()->GetMaxCityAccount(sPing,sAccount);
    int iIndex = 1;
    if( sAccount.find(sPing) != string::npos ){
        sPing = GetPinFromAccount(sAccount);
        string sNum = sAccount.substr(sPing.length());
        iIndex = atoi( sNum.c_str() );
        iIndex++;
    }
    sAccount = sPing + tConvert.IntToStr( iIndex);
    entity.contentEntity.sAccount = sAccount;


    string sOrgPwd ;
    if( entity.contentEntity.sCtiy != "美支付供应商" ){
        sOrgPwd = MyUtil::MakeRandNum(6);
    }else{
        sOrgPwd  = "888888";
    }
    entity.contentEntity.sPassWord = Md5ToLower("pay" + sOrgPwd ) ;

    entity.contentEntity.iAwardingPoints = 0;
    entity.contentEntity.sBusiness = "";
    entity.contentEntity.sHours = "";
    entity.contentEntity.sIdPhoto = tConvert.Json2String( pJson["sIdPhoto"]);
    entity.contentEntity.sLicensePhoto = tConvert.Json2String( pJson["sLicensePhoto"]);
    entity.contentEntity.sPosNum = "";
    if( !pJson["sSevicePhone"].isNull()){
        entity.contentEntity.sSevvicePhone = tConvert.Json2String( pJson["sSevicePhone"]);
    }
    entity.contentEntity.sShopDetailPhoto ="";
    entity.contentEntity.sShopFrontPhoto = tConvert.Json2String( pJson["sShopFrontPhoto"]);
    if( !pJson["sShopType"].isNull()){
        entity.contentEntity.sShopType = tConvert.Json2String( pJson["sShopType"]);
    }

    entity.contentEntity.iAvgConsume = 0;
    entity.contentEntity.iProrate = 0;

    ArrayToString(pJson["sIdPhoto"],entity.contentEntity.sIdPhoto);
    ArrayToString(pJson["sLicensePhoto"],entity.contentEntity.sLicensePhoto);
    ArrayToString(pJson["sShopFrontPhoto"],entity.contentEntity.sShopFrontPhoto);
    ArrayToString(pJson["sOtherCerPhoto"],entity.contentEntity.sOtherCerPhoto);

    entity.iState = 0;
    if( !IsShopPerfect(entity)){
        entity.iState = 2;
    }

    //添加
    iRet = ShopInfoDAL::GetInstance()->Insert( entity ) ;
    if ( 0 != iRet) {
        appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::CreateShop error");
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }




    pResponse["iRet"] = 0;
    pResponse["sQrCodeID"] = entity.sPrimaryKey;
    pResponse["sAccount"] = entity.contentEntity.sAccount;
    pResponse["sShopName"] = entity.contentEntity.sShopName;
    pResponse["sShopPhone"] = entity.contentEntity.sShopPhone;

    appendlog(TTDLogger::LOG_INFO,"ShopInfoBLL::CreateShop success platform:%s pwd:%s", entity.sPrimaryKey.c_str(), sOrgPwd.c_str());

    if( entity.contentEntity.sCtiy != "美支付供应商" ){
        //发送商家开通通知
        string sMsg = entity.sPrimaryKey;
        sMsg += ",";
        sMsg += sOrgPwd;

        string sType = "11";
        if( !IsShopPerfect(entity)){
            sType = "16";//开通平台但要补全资料的通知
        }
        SendPhoneNotifyCode(entity.contentEntity.sShopPhone, sMsg,sType);
        SendNotify(entity.contentEntity.lUserId,entity.sPrimaryKey,sOrgPwd);

        //申请平台通过时，让平台主关注自己的平台号
        if(entity.contentEntity.iVerify == 1){
            bool bRet = Attention(entity.contentEntity.lUserId,tConvert.StrToLong( entity.sPrimaryKey));
            if( !bRet){
                appendlog(TTDLogger::LOG_ERROR,"ShopInfoBLL::Update Attention err [PlatformId:%s][lUserId:%ld]", entity.sPrimaryKey.c_str(),entity.contentEntity.lUserId);
            }
        }

    }

    return 0;

}

//登陆
int ShopInfoBLL::Login(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if(  (pJson["sPrimaryKey"].isNull()   && pJson["sAccount"].isNull() ) || pJson["sPassWord"].isNull() ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    string sCond ;
    int iType ;
    if(! pJson["sPrimaryKey"].isNull() ){
        sCond = tConvert.Json2String(pJson["sPrimaryKey"]);
        iType = 1;
    }else{
        sCond = tConvert.Json2String(pJson["sAccount"]);
        iType = 2;
    }
    string sPwd =  tConvert.Json2String(pJson["sPassWord"]);
    ShopInfoEntity entity;
    iRet = ShopInfoDAL::GetInstance()->GetEntity( sCond,iType, entity);

    if( entity.contentEntity.sPassWord !=sPwd ){
        RetJsonMsg::SetRetMsg(-6 ,pResponse);
        return -6;
    }
    if( entity.contentEntity.iVerify != 1 && entity.contentEntity.iVerify != 3){
        RetJsonMsg::SetRetMsg(-103 ,pResponse);
        return -103;
    }
    if( entity.iState == 1 ){
        RetJsonMsg::SetRetMsg(-1 ,pResponse,"此账号违规操作，暂时冻结");
        return -1;
    }
    if( entity.iState == 2 ){//资料未补全
        RetJsonMsg::SetRetMsg(-1 ,pResponse,"登录失败，请前往APP补全店铺资料");
        return -1;
    }
    pResponse["lPlatformId"] = entity.sPrimaryKey;
    pResponse["sShopName"] = entity.contentEntity.sShopName;
    pResponse["sCity"] = entity.contentEntity.sCtiy;
    pResponse["lPlatformId"] = entity.sPrimaryKey;


    RetJsonMsg::SetRetMsg(iRet ,pResponse);
    return 0;
}

//获取个代开的店
int ShopInfoBLL::ShopsByPerAgent(const  Json::Value  & pJson ,Json::Value & pResponse)
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lUserId"].isNull()    ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lUserId = tConvert.Json2Long(pJson["lUserId"]) ;
    int iMCount= 0, iGYCount = 0;
    ShopInfoList shopLst;
    ShopInfoDAL::GetInstance()->Shops(lUserId,2, shopLst);
    ShopInfoList::iterator it = shopLst.begin();
    for( ; it != shopLst.end() ; it++){
        ShopInfoEntity & entity = *it;
        if( entity.contentEntity.sCtiy == "美支付供应商"){
            iGYCount ++;
        }else {
            iMCount ++;
        }
    }
    pResponse["iMCount"] = iMCount;
    pResponse["iGYCount"] = iGYCount;
    RetJsonMsg::SetRetMsg(0 ,pResponse);
    return 0;

}



//归档
int ShopInfoBLL::Doc( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;
    string sCond ;int iType;
    if( ! pJson["lPlatformId"].isNull()  ){
        sCond = tConvert.Json2String( pJson["lPlatformId"] );
        iType = 1;
    }else{
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    ShopInfoEntity  entity;
    iRet =  ShopInfoDAL::GetInstance()->GetEntity( sCond ,iType,entity);
    if ( 0 == iRet){
        pResponse["entity"]["lPlatformId"] = entity.sPrimaryKey;
        pResponse["entity"]["sShopName"] = entity.contentEntity.sShopName;
        pResponse["entity"]["sPassWord"] = entity.contentEntity.sPassWord;
        pResponse["iRet"] = iRet;
        return iRet;
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


int ShopInfoBLL::AddBank( const  Json::Value  & pJson ,Json::Value & pResponse )
{

    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull() || pJson["sName"].isNull() ||
            pJson["sAccount"].isNull()  || pJson["sBranch"].isNull() || pJson["sBank"].isNull() ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    BankEntity bank;
    bank.lPlatformId = tConvert.Json2Long( pJson["lPlatformId"] );
    bank.sName = tConvert.Json2String( pJson["sName"] );
    bank.sAccount = tConvert.Json2String( pJson["sAccount"] );
    bank.sBranch = tConvert.Json2String( pJson["sBranch"] );
    bank.sBank = tConvert.Json2String( pJson["sBank"] );
    iRet = mBankDal.replace(bank);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

int ShopInfoBLL::GetBank( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    BankEntity bank;
    iRet = mBankDal.GetEntity( lPlatformId,bank);

    if ( 0 == iRet){
        pResponse["entity"]["lPlatformId"] = tConvert.LongToStr( bank.lPlatformId );
        pResponse["entity"]["sAccount"] = bank.sAccount;
        pResponse["entity"]["sBank"] = bank.sBank;
        pResponse["entity"]["sBranch"] = bank.sBranch;
        pResponse["entity"]["sName"] = bank.sName;

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//修改开关配置
int ShopInfoBLL::Setting( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;int iRet = 0;
    if( pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);

    if( mShopSettingDal.IsExist(lPlatformId)!= 0){
        ShopSetting set;
        set.lPlatformId = lPlatformId;
        set.iCashBack = enum_cashback_close;
        set.iCoinBack = enum_coinback_close;
        set.iRestrict = enum_buyrestrict_false;
        set.iDelivery = enum_delivery_close;
        set.iPark = enum_park_close;
        set.iWifi = enum_wifi_close;

        set.lAddTime = tConvert.GetCurrentTime();
        set.lStartTime = set.lAddTime;
        iRet = mShopSettingDal.Add(set);
    }

    if( iRet == 0){

        if( !pJson["iCashBack"].isNull()){
            int iCashBack = tConvert.Json2Int(pJson["iCashBack"]);
            iRet = mShopSettingDal.UpdateCashBack(lPlatformId,iCashBack);

        }

        if( !pJson["iCoinBack"].isNull()){
            int iCoinBack = tConvert.Json2Int(pJson["iCoinBack"]);
            iRet = mShopSettingDal.UpdateCoinBack(lPlatformId,iCoinBack);

        }

        if( !pJson["sStartTime"].isNull()){
            string sStartTime = tConvert.Json2String( pJson["sStartTime"] );
            long lStartTime = (long)tConvert.StrToTime( sStartTime.c_str());
            iRet = mShopSettingDal.UpdateStartTime(lPlatformId,lStartTime);

        }

        if( !pJson["iRecommend"].isNull()){
            int iRecommend = tConvert.Json2Int( pJson["iRecommend"] );
            iRet = mShopSettingDal.UpdateRecommend(lPlatformId,iRecommend);

        }

        if( !pJson["iRestrict"].isNull()){
            int iRestrict = tConvert.Json2Int( pJson["iRestrict"] );
            iRet = mShopSettingDal.UpdateRestrict(lPlatformId,iRestrict);
        }

        if( !pJson["iDelivery"].isNull()){
            int iDelivery = tConvert.Json2Int( pJson["iDelivery"] );
            iRet = mShopSettingDal.UpdateDelivery(lPlatformId,iDelivery);
        }

        if( !pJson["iPark"].isNull()){
            int iPark = tConvert.Json2Int( pJson["iPark"] );
            iRet = mShopSettingDal.UpdatePark(lPlatformId,iPark);
        }

        if( !pJson["iWifi"].isNull()){
            int iWifi = tConvert.Json2Int( pJson["iWifi"] );
            iRet = mShopSettingDal.UpdateWifi(lPlatformId,iWifi);
        }
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//获取开关配置
int ShopInfoBLL::GetSetting( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    ShopSetting set;
    iRet = mShopSettingDal.GetEntity( lPlatformId,set);

    if ( 0 == iRet){
        pResponse["entity"]["lPlatformId"] = tConvert.LongToStr( set.lPlatformId );
        pResponse["entity"]["iCashBack"] = set.iCashBack;
        pResponse["entity"]["iCoinBack"] = set.iCoinBack;
        pResponse["entity"]["sStartTime"] = tConvert.TimeToStr( set.lStartTime );
        pResponse["entity"]["iRecommend"] = set.iRecommend;
        pResponse["entity"]["iRestrict"] = set.iRestrict;

    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

int ShopInfoBLL::ModifyPhone( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull() || pJson["sShopPhone"].isNull() ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);
    string sShopPhone = tConvert.Json2String(pJson["sShopPhone"]);
    long lUserId = -1;
    iRet = GetUserInfoByPhone(sShopPhone,lUserId);
    if( (iRet == 0) && (lUserId > 0)){
        iRet = ShopInfoDAL::GetInstance()->UpdatePhone(lPlatformId,sShopPhone,lUserId);
    }else{
        iRet = -5;
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

int ShopInfoBLL::GetState( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    int iRet = -1;

    if( pJson["lPlatformId"].isNull()  ){
        RetJsonMsg::SetRetMsg(ERRPOSTPARAM ,pResponse);
        return iRet;
    }
    long lPlatformId = tConvert.Json2Long(pJson["lPlatformId"]);

    int iState = -1;
    iRet = ShopInfoDAL::GetInstance()->GetState(lPlatformId,iState);

    pResponse["iState"] = iState;

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

int ShopInfoBLL::CashBackShops( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    int iPage = tConvert.Json2Long(pJson["iPage"]);
    int iCount = tConvert.Json2Long(pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }
    CashBackShopLst lst;
    iRet = ShopInfoDAL::GetInstance()->GetCashBackShops(iPage,iCount,lst);
    int i = 0;
    for( CashBackShopLst::iterator it = lst.begin(); it != lst.end(); it++){
        CashBackShop & d = *it;
        pResponse["list"][i]["lPlatformId"] = tConvert.LongToStr( d.lPlatformId );
        pResponse["list"][i]["sStartTime"] = tConvert.TimeToStr( d.lStartTime );
        pResponse["list"][i]["sShopPhone"] =  d.sShopPhone ;
        i++;
    }
    if( lst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}
//获取推荐的商家(推荐的商家优先显示，接下来是符合所在地址并按距离排序显示)
int ShopInfoBLL::RecommendShops( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["iPage"].isNull() || pJson["iCount"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    int iPage = tConvert.Json2Long(pJson["iPage"]);
    int iCount = tConvert.Json2Long(pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }
    ShopInfoList lst;
    ShopQueryEntity query;

    if( ! pJson["dGpsLat"].isNull()){
        query.dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);
    }else{
        query.dGpsLat = -1.0;
    }
    if( ! pJson["dGpsLon"].isNull()){
        query.dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);
    }else{
        query.dGpsLon = -1.0;
    }
    if( ! pJson["sCity"].isNull()){
        query.sCtiy = tConvert.Json2String( pJson["sCity"]);
        if( query.sCtiy == "深圳龙华区"){
            query.sCtiy = "深圳龙华新区";
        }
    }else{
        query.sCtiy = "citys";
    }
    if( ! pJson["sShopName"].isNull()){
        query.sShopName = tConvert.Json2String( pJson["sShopName"]);
    }else{
        query.sShopName = "";
    }
    if( ! pJson["sShopType"].isNull()){
        query.sShopType = tConvert.Json2String( pJson["sShopType"]);
    }else{
        query.sShopType = "";
    }

    if( !pJson["bStar"].isNull() ){
        query.bStar = tConvert.Json2Bool(pJson["bStar"]);
    }else{
        query.bStar = false;
    }

    if( !pJson["iAuth"].isNull() ){
        query.iAuth = tConvert.Json2Int(pJson["iAuth"]);
    }else{
        query.iAuth = -1;
    }

    if( !pJson["sTown"].isNull() ){
        query.sTown = tConvert.Json2String(pJson["sTown"]);
    }else{
        query.sTown = "";
    }

    if( !pJson["sArea"].isNull() ){
        query.sArea = tConvert.Json2String(pJson["sArea"]);
        if( query.sArea.find( "龙华") != string::npos){
            query.sArea = "龙华";
        }

    }else{
        query.sArea = "";
    }


    if( !pJson["iEnumDiscount"].isNull() ){
        query.iEnumDiscount = tConvert.Json2Int(pJson["iEnumDiscount"]);
    }else{
        query.iEnumDiscount = -1;
    }
    query.bCityZone = false;
    query.iShowNearby  =1;//1显示
    query.iVerify = 1;//1 审核通过



    int iTotalCount;


    iRet = ShopInfoDAL::GetInstance()->RecommendShops(query,iPage,iCount,lst,iTotalCount);




    if( 0 == iRet ){
        DataLst2Json(lst,pResponse,2);
        //返回总个数
        pResponse["iTotal"]  = iTotalCount;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

int ShopInfoBLL::GetLotsPlatfrom( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull() || pJson["lPlatformIds"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    double dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);
    double dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);
    Json::Value jPlatformIds = pJson["lPlatformIds"];

    ShopInfoList lst;
    for( int i = 0; i < jPlatformIds.size() ; i++){
        ShopInfoEntity entity;
        iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.Json2String(jPlatformIds[i]),1,entity);
        if( iRet == 0){
            entity.contentEntity.lDistance = (long)GetDistance(dGpsLat,dGpsLon,entity.contentEntity.dGpsLat,entity.contentEntity.dGpsLon);
            lst.push_back(entity);
        }
    }
    DataLst2Json(lst,pResponse,1);
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}

//add commtent 处理带token，不加密的情况
int ShopInfoBLL::WriteCommtent2(  const  Json::Value  & pJson  ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["score"].isNull() || pJson["platformId"].isNull() || pJson["userId"].isNull() ||
            pJson["content"].isNull() || pJson["headImage"].isNull() || pJson["nickName"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    CommentEntity entity;
    double dScore = tConvert.Json2Double( pJson["score"]);
    entity.iScore = dScore*10;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = tConvert.Json2Long( pJson["platformId"]);
    entity.lUserId = tConvert.Json2Long( pJson["userId"]);
    entity.sContent = tConvert.Json2String( pJson["content"]);
    entity.sHeadImage = tConvert.Json2String( pJson["headImage"]);
    entity.sNickName = tConvert.Json2String( pJson["nickName"]);


    iRet = mCommentDAL.Add(entity);

    int iAvgSorce = mCommentDAL.GetAvgSorce(entity.lPlatformId,0);
    //更新平均分
    ShopInfoDAL::GetInstance()->UpAvgScore(entity.lPlatformId,iAvgSorce);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//超管写评论 处理带token，不加密的情况
int ShopInfoBLL::WriteCommtentBySuper2( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["score"].isNull() || pJson["platformId"].isNull() ||  pJson["content"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }


    CommentEntity entity;
    double dScore = tConvert.Json2Double( pJson["score"]);
    entity.iScore = dScore*10;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = tConvert.Json2Long( pJson["platformId"]);
    entity.lUserId = 50037;
    entity.sContent = tConvert.Json2String( pJson["content"]);
    entity.sHeadImage = "";
    entity.sNickName = "";

    if( !pJson["pics"].isNull() && pJson["pics"].isArray()){

        entity.sPics =  pJson["pics"].toStyledString();
    }

    if( !pJson["idProduct"].isNull() ){

        entity.idProduct = tConvert.Json2Long( pJson["idProduct"]);
    }else{
        entity.idProduct = 0;
    }

    LongLongMap::iterator it = mSuperCommentTime.find(entity.lPlatformId);
    long lCurTime = tConvert.GetCurrentTime()/1000000;
    if( it != mSuperCommentTime.end()){
        long lLastTime =  it->second;
        if( lCurTime - lLastTime < 60*10){
            RetJsonMsg::SetRetMsg2(-2,pResponse);
            pResponse["msg"] = "发评论太快了";
            return -2;
        }
    }
    mSuperCommentTime[entity.lPlatformId] =lCurTime;


    iRet = mCommentDAL.Add(entity);

    int iAvgSorce = mCommentDAL.GetAvgSorce(entity.lPlatformId,0);
    //更新平均分
    ShopInfoDAL::GetInstance()->UpAvgScore(entity.lPlatformId,iAvgSorce);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//add commtent
int ShopInfoBLL::WriteCommtent( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["iScore"].isNull() || pJson["lPlatformId"].isNull() || pJson["lUserId"].isNull() ||
            pJson["sContent"].isNull() || pJson["sHeadImage"].isNull() || pJson["sNickName"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    CommentEntity entity;
    double dScore = tConvert.Json2Double( pJson["iScore"]);
    entity.iScore = dScore*10;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    entity.lUserId = tConvert.Json2Long( pJson["lUserId"]);
    entity.sContent = tConvert.Json2String( pJson["sContent"]);
    entity.sHeadImage = tConvert.Json2String( pJson["sHeadImage"]);
    entity.sNickName = tConvert.Json2String( pJson["sNickName"]);
    entity.idProduct = 0;

    if( !pJson["sPics"].isNull() && pJson["sPics"].isArray()){

        entity.sPics =  pJson["sPics"].toStyledString();
    }

    iRet = mCommentDAL.Add(entity);

    int iAvgSorce = mCommentDAL.GetAvgSorce(entity.lPlatformId,0);
    //更新平均分
    ShopInfoDAL::GetInstance()->UpAvgScore(entity.lPlatformId,iAvgSorce);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//get commtent 处理带token，不加密的情况
int ShopInfoBLL::GetCommtent2(long lPlatformId,long lAddTime,int iCount ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if( lAddTime == 0){
        lAddTime = 9480400983864318;//未来的时间
    }
    long lNextTime = 9480400983864318;
    CommentEntityLst lst;int iTotal = 0;
    if( iCount > 20){
        iCount = 20;
    }
    mCommentDAL.GetData(lPlatformId,0,lAddTime,iCount,lst,iTotal);
    for( int i = 0 ; i < lst.size(); i++){
        CommentEntity & entity = lst[i];
        pResponse["result"][i]["score"] = tConvert.DoubleToStr( ((double)entity.iScore)/10);
        pResponse["result"][i]["addTime"] = tConvert.TimeToStr( entity.lAddTime/1000000 ).substr(0,10);
        pResponse["result"][i]["platformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["result"][i]["userId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["result"][i]["content"] = entity.sContent ;
        pResponse["result"][i]["headImage"] =  entity.sHeadImage ;
        pResponse["result"][i]["nickName"] =  entity.sNickName ;

        pResponse["result"][i]["pics"] =  entity.sPics ;


        if( entity.lAddTime < lNextTime){
            lNextTime = entity.lAddTime;
        }

    }
    if( lst.size() <= 0){
        pResponse["result"] ;
        pResponse["result"].resize(0);
    }
    pResponse["nextTime"] = tConvert.LongToStr( lNextTime );
    pResponse["total"] = iTotal;
    pResponse["score"] =  GetAvgScore(lPlatformId,0) ;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//get commtent
int ShopInfoBLL::GetCommtent( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["lPlatformId"].isNull() || pJson["lAddTime"].isNull() ||  pJson["iCount"].isNull()
         ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    long lAddTime = tConvert.Json2Long( pJson["lAddTime"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }
    long lProductId = 0;
    if( !pJson["lProductId"].isNull()){
        lProductId = tConvert.Json2Long( pJson["lProductId"]);
    }

    if( lAddTime == 0){
        lAddTime = 9480400983864318;//未来的时间
    }
    CommentEntityLst lst;int iTotal = 0;
    mCommentDAL.GetData(lPlatformId,lProductId,lAddTime,iCount,lst,iTotal);
    for( int i = 0 ; i < lst.size(); i++){
        CommentEntity & entity = lst[i];
        pResponse["list"][i]["iScore"] = tConvert.DoubleToStr( ((double)entity.iScore)/10);
        pResponse["list"][i]["lAddTime"] = tConvert.LongToStr( entity.lAddTime );
        pResponse["list"][i]["lPlatformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["list"][i]["lUserId"] = tConvert.LongToStr( entity.lUserId );
        pResponse["list"][i]["sContent"] = entity.sContent ;
        pResponse["list"][i]["sHeadImage"] =  entity.sHeadImage ;
        pResponse["list"][i]["sNickName"] =  entity.sNickName ;

        pResponse["list"][i]["sPics"].resize(0);
        if( !entity.sPics.empty()){

            tConvert.Str2Json( entity.sPics, pResponse["list"][i]["sPics"]);
        }

    }
    if( lst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }
    pResponse["iTotal"] =  iTotal ;
    pResponse["dScore"] =  GetAvgScore(lPlatformId,lProductId) ;
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


//添加广告
int ShopInfoBLL::AddAdv( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["lPlatformId"].isNull()  ||  pJson["sAdvLink"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    string sAdvLink = tConvert.Json2String( pJson["sAdvLink"]);
    ShopAdv adv;
    adv.iState = enum_show_yes;
    adv.iVerify = enum_verify_un;
    adv.lAddTime = tConvert.GetCurrentTime();
    adv.lPlatformId = lPlatformId;
    adv.sAdvLink = sAdvLink;
    adv.id = atol( (tConvert.LongToStr(lPlatformId) + tConvert.LongToStr( tConvert.GetCurrentTime()/1000000)).c_str());

    ShopAdvLst advlst;int iTotal;
    mAdvDAL.GetData(lPlatformId,1,1,-1,-1,advlst,iTotal);
    if( advlst.size() > 0){
        pResponse["iRet"] = -1;
        pResponse["id"] = 0;
        pResponse["sMsg"] = "现阶段只能添加一个广告";
        return -1;
    }

    if(  0 == mAdvDAL.IsExist(lPlatformId,sAdvLink)){
        iRet = 0;
    }else{

        iRet = mAdvDAL.Add(adv);
    }

    pResponse["id"] = tConvert.LongToStr( adv.id );
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//删除广告
int ShopInfoBLL::DelAdv( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);
    if( 0 != mAdvDAL.IsExist(id)){
        RetJsonMsg::SetRetMsg(-5,pResponse);
        pResponse["sMsg"] = "广告不存在";
        return -5;
    }
    iRet = mAdvDAL.Del(id);
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//更改广告审核状态
int ShopInfoBLL::AdvVerify( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["id"].isNull() || pJson["iVerify"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);
    long iVerify = tConvert.Json2Int( pJson["iVerify"]);
    iRet = mAdvDAL.UpVerify(id,iVerify);

    RetJsonMsg::SetRetMsg(iRet,pResponse);

    return iRet;


}

//查询广告
int ShopInfoBLL::GetAdv( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["iPage"].isNull()  ||  pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }

    long lPlatformId = -1;
    int iVerify = -1;
    int iState = -1;
    string sShopName;

    if( !pJson["lPlatformId"].isNull()){
        lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    }
    if( !pJson["iVerify"].isNull()){
        iVerify = tConvert.Json2Int( pJson["iVerify"]);
    }
    if( !pJson["iState"].isNull()){
        iState = tConvert.Json2Int( pJson["iState"]);
    }
    if(!pJson["sShopName"].isNull()){
        sShopName = tConvert.Json2String( pJson["sShopName"]);
        ShopInfoDAL::GetInstance()->GetPlatformIdByShopName(sShopName,lPlatformId);
    }


    ShopAdvLst lst;int iTotalCount;
    iRet = mAdvDAL.GetData(lPlatformId,iPage,iCount,iVerify, iState,lst,iTotalCount);
    for( int i = 0 ; i < lst.size() ; i++){
        ShopAdv & adv = lst[i];
        pResponse["list"][i]["id"] = tConvert.LongToStr( adv.id );
        pResponse["list"][i]["sAddTime"] = tConvert.TimeToStr( adv.lAddTime/1000000 );
        pResponse["list"][i]["sAdvLink"] = adv.sAdvLink ;
        pResponse["list"][i]["lPlatformId"] = tConvert.LongToStr(adv.lPlatformId) ;
        pResponse["list"][i]["iVerify"] = adv.iVerify ;
        ShopInfoEntity shop;
        ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(adv.lPlatformId),1,shop);
        pResponse["list"][i]["sShopName"] =  shop.contentEntity.sShopName ;
    }
    if( lst.size() <= 0){
        pResponse["list"].resize(0);
    }

    pResponse["iTotal"] = iTotalCount;

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


//添加产品
int ShopInfoBLL::AddProduct( const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["lPlatformId"].isNull()  ||  pJson["sPicLink"].isNull() ||  pJson["sName"].isNull()
         ||  pJson["dPrice"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    string sPicLink = tConvert.Json2String( pJson["sPicLink"]);
    string sName = tConvert.Json2String( pJson["sName"]);
    double dPrice = tConvert.Json2Double( pJson["dPrice"]);
    ShowPro entity;
    entity.dCoin = 0.0;
    entity.dPrice = dPrice;
    entity.iState = enum_show_yes;
    entity.iVerify = enum_verify_un;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = lPlatformId;
    entity.sName = sName;
    entity.sPicLink  = sPicLink;
    entity.id = atol( (tConvert.LongToStr(lPlatformId) + tConvert.LongToStr( tConvert.GetCurrentTime()/1000000)).c_str());

    iRet = mShowDAL.Add(entity);


    pResponse["id"] = tConvert.LongToStr( entity.id );
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}

//添加产品 处理带token，不加密的情况
int ShopInfoBLL::AddProduct2(long lPlatformId, const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert  tConvert;
    int iRet = -1;
    if(   pJson["picLink"].isNull() ||  pJson["same"].isNull()
          ||  pJson["price"].isNull()  ||  pJson["coin"].isNull() ||  pJson["coinType"].isNull()
          ||  pJson["content"].isNull() ||  pJson["expireTime"].isNull() ||  pJson["canot"].isNull()
          ||  pJson["utime"].isNull() ||  pJson["law"].isNull() ||  pJson["detailPics"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    string sPicLink = tConvert.Json2String( pJson["picLink"]);
    string sName = tConvert.Json2String( pJson["same"]);
    double dPrice = tConvert.Json2Double( pJson["price"]);

    double coin = tConvert.Json2Double( pJson["coin"]);
    int coinType = tConvert.Json2Int( pJson["coinType"]);
    string expireTime = tConvert.Json2String( pJson["expireTime"]);
    string utime = tConvert.Json2String( pJson["utime"]);
    string canot = tConvert.Json2String( pJson["canot"]);
    string content = tConvert.Json2String( pJson["content"]);

    string detailPics;
    if( pJson["detailPics"].isArray()){
        Json::Value j = pJson["detailPics"];
        detailPics = j.toStyledString();
    }
    string law;
    if( pJson["law"].isArray()){
        Json::Value j = pJson["law"];
        law = j.toStyledString();
    }
    ShowPro entity;

    entity.dPrice = dPrice;
    entity.iState = enum_show_yes;
    entity.iVerify = enum_verify_un;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = lPlatformId;
    entity.sName = sName;
    entity.sPicLink  = sPicLink;
    entity.dCoin = coin;
    entity.iCoinType = coinType;
    entity.sExpireTime = expireTime;
    entity.sUtime = utime;
    entity.sLaw = law;
    entity.sDetailPics = detailPics;

    entity.sCanot = canot;
    entity.sContent = content;

    entity.id = atol( (tConvert.LongToStr(lPlatformId) + tConvert.LongToStr( tConvert.GetCurrentTime()/1000000)).c_str());

    iRet = mShowDAL.Add(entity);


    pResponse["id"] = tConvert.LongToStr( entity.id );
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;
}

//删除产品
int ShopInfoBLL::DelProduct( const  Json::Value  & pJson ,Json::Value & pResponse ){
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);
    if( 0 != mShowDAL.IsExist(id)){
        RetJsonMsg::SetRetMsg(-5,pResponse);
        pResponse["sMsg"] = "产品不存在";
        return -5;
    }
    iRet = mShowDAL.Del(id);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//更改产品审核状态
int ShopInfoBLL::ProductVerify( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["id"].isNull() || pJson["iVerify"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);
    long iVerify = tConvert.Json2Int( pJson["iVerify"]);
    iRet = mShowDAL.UpVerify(id,iVerify);

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//获取展示产品 处理带token，不加密的情况
int ShopInfoBLL::ShowProduct2(long lPlatformId, int iPage,Json::Value & pResponse )
{

    TConvert  tConvert;
    int iRet = -1;

    int iCount = 10;

    ShopInfoEntity shop;
    iRet = ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(lPlatformId),1,shop);
    if( iRet != 0){
        RetJsonMsg::SetRetMsg(iRet,pResponse);
        return iRet;
    }
    int iDiscount = shop.contentEntity.iOffLineDisCount;
    if(iDiscount <= 0){
        iDiscount = 100;
    }
    ShowProLst lst;int iTotalCount;
    mShowDAL.GetData(lPlatformId,iPage,iCount,enum_verify_yes,enum_show_yes,"",lst,iTotalCount);

    for( int i  =0 ; i < lst.size() ; i++){
        ShowPro & entity = lst[i];
        pResponse["result"][i]["platformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["result"][i]["name"] =  entity.sName ;
        pResponse["result"][i]["picLink"] =  entity.sPicLink ;
        pResponse["result"][i]["price"] =  entity.dPrice ;
        pResponse["result"][i]["coin"] =  entity.dCoin;

        pResponse["result"][i]["coinType"] =  entity.iCoinType;
        pResponse["result"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["result"][i]["canot"] =  entity.sCanot;
        pResponse["result"][i]["content"] =  entity.sContent;
        pResponse["result"][i]["detailPics"] =  entity.sDetailPics;
        pResponse["result"][i]["expireTime"] =  entity.sExpireTime;
        pResponse["result"][i]["law"] =  entity.sLaw;
    }
    if( lst.size() == 0){
        pResponse["result"];
        pResponse["result"].resize(0);
    }


    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//获取展示产品
int ShopInfoBLL::GetProduct( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["iPage"].isNull()  ||  pJson["iCount"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int iCount = tConvert.Json2Int( pJson["iCount"]);
    if( iCount > 20){
        iCount = 20;
    }
    long lPlatformId = -1;
    int iVerify = -1;
    string sName = "",sShopName = "";
    if( !pJson["lPlatformId"].isNull()){
        lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    }
    if( !pJson["iVerify"].isNull()){
        iVerify = tConvert.Json2Int( pJson["iVerify"]);
    }
    if(!pJson["sName"].isNull()){
        sName = tConvert.Json2String( pJson["sName"]);
    }
    if(!pJson["sShopName"].isNull()){
        sShopName = tConvert.Json2String( pJson["sShopName"]);
        if( ShopInfoDAL::GetInstance()->GetPlatformIdByShopName(sShopName,lPlatformId) != 0 ){
            lPlatformId = 999999999999999999999999;
        }
    }



    ShowProLst lst;int iTotalCount;
    iRet = mShowDAL.GetData(lPlatformId,iPage,iCount,iVerify, -1,sName,lst,iTotalCount);
    for( int i = 0 ; i < lst.size() ; i++){
        ShowPro & entity = lst[i];
        pResponse["list"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["list"][i]["sAddTime"] = tConvert.TimeToStr( entity.lAddTime/1000000 );

        if( entity.sPicLink.substr(0,3) == "ttp")
            entity.sPicLink = "h" + entity.sPicLink;
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "jp")
            entity.sPicLink += "g";
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "pn")
            entity.sPicLink += "g";

        pResponse["list"][i]["sPicLink"] = entity.sPicLink ;
        pResponse["list"][i]["dPrice"] =  entity.dPrice; ;
        pResponse["list"][i]["iState"] = entity.iState ;
        pResponse["list"][i]["iVerify"] = entity.iVerify ;
        pResponse["list"][i]["lPlatformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["list"][i]["sName"] =  entity.sName ;
        ShopInfoEntity shop;
        ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(entity.lPlatformId),1,shop);
        pResponse["list"][i]["sShopName"] =  shop.contentEntity.sShopName ;

        pResponse["list"][i]["dCoin"] =  entity.dCoin;
        pResponse["list"][i]["iCoinType"] =  entity.iCoinType;


    }
    if( lst.size() <= 0){
        pResponse["list"].resize(0);
    }

    pResponse["iTotal"] = iTotalCount;
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//获取展示产品 处理带token，不加密的情况
int ShopInfoBLL::GetProduct2( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["page"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    int iPage = tConvert.Json2Int( pJson["page"]);
    int iCount = 10;
    long lPlatformId = -1;
    int iVerify = -1;
    string sName = "",sShopName = "";
    if( !pJson["platformId"].isNull()){
        lPlatformId = tConvert.Json2Long( pJson["platformId"]);
    }
    if( !pJson["verify"].isNull()){
        iVerify = tConvert.Json2Int( pJson["verify"]);
    }
    if(!pJson["name"].isNull()){
        sName = tConvert.Json2String( pJson["name"]);
    }
    if(!pJson["shopName"].isNull()){
        sShopName = tConvert.Json2String( pJson["shopName"]);
        if( ShopInfoDAL::GetInstance()->GetPlatformIdByShopName(sShopName,lPlatformId) != 0 ){
            lPlatformId = 999999999999999999999999;
        }
    }



    ShowProLst lst;int iTotalCount;
    iRet = mShowDAL.GetData(lPlatformId,iPage,iCount,iVerify, -1,sName,lst,iTotalCount);
    for( int i = 0 ; i < lst.size() ; i++){
        ShowPro & entity = lst[i];
        pResponse["result"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["result"][i]["addTime"] = tConvert.TimeToStr( entity.lAddTime/1000000 );

        if( entity.sPicLink.substr(0,3) == "ttp")
            entity.sPicLink = "h" + entity.sPicLink;
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "jp")
            entity.sPicLink += "g";
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "pn")
            entity.sPicLink += "g";

        pResponse["result"][i]["picLink"] = entity.sPicLink ;
        pResponse["result"][i]["price"] =  entity.dPrice; ;
        pResponse["result"][i]["state"] = entity.iState ;
        pResponse["result"][i]["verify"] = entity.iVerify ;
        pResponse["result"][i]["platformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["result"][i]["name"] =  entity.sName ;
        ShopInfoEntity shop;
        ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(entity.lPlatformId),1,shop);
        pResponse["result"][i]["shopName"] =  shop.contentEntity.sShopName ;

        pResponse["result"][i]["coin"] =  entity.dCoin;
        pResponse["result"][i]["coinType"] =  entity.iCoinType;

        pResponse["result"][i]["canot"] =  entity.sCanot;
        pResponse["result"][i]["content"] =  entity.sContent;
        if(entity.sDetailPics.empty()){
            entity.sDetailPics = "[]";
        }
        tConvert.Str2Json(entity.sDetailPics,pResponse["result"][i]["detailPics"]);
        pResponse["result"][i]["expireTime"] =  entity.sExpireTime;
        if(entity.sLaw.empty()){
            entity.sLaw = "[]";
        }
        tConvert.Str2Json(entity.sLaw,pResponse["result"][i]["law"]);


    }
    if( lst.size() <= 0){
        pResponse["result"].resize(0);
    }

    pResponse["total"] = iTotalCount;
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}



//获取展示产品 处理带token，不加密的情况
int ShopInfoBLL::GetProductByToken( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if(  pJson["page"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    int iPage = tConvert.Json2Int( pJson["page"]);
    int iCount = 10;
    int iVerify = -1;
    string sName = "";

    if( !pJson["verify"].isNull()){
        iVerify = tConvert.Json2Int( pJson["verify"]);
    }
    if(!pJson["name"].isNull()){
        sName = tConvert.Json2String( pJson["name"]);
    }



    ShowProLst lst;int iTotalCount;
    iRet = mShowDAL.GetData(lPlatformId,iPage,iCount,iVerify, -1,sName,lst,iTotalCount);
    for( int i = 0 ; i < lst.size() ; i++){
        ShowPro & entity = lst[i];
        pResponse["result"][i]["id"] = tConvert.LongToStr( entity.id );
        pResponse["result"][i]["addTime"] = tConvert.TimeToStr( entity.lAddTime/1000000 );

        if( entity.sPicLink.substr(0,3) == "ttp")
            entity.sPicLink = "h" + entity.sPicLink;
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "jp")
            entity.sPicLink += "g";
        if( entity.sPicLink.substr(entity.sPicLink.length() -2) == "pn")
            entity.sPicLink += "g";

        pResponse["result"][i]["picLink"] = entity.sPicLink ;
        pResponse["result"][i]["price"] =  entity.dPrice; ;
        pResponse["result"][i]["state"] = entity.iState ;
        pResponse["result"][i]["verify"] = entity.iVerify ;
        pResponse["result"][i]["platformId"] = tConvert.LongToStr( entity.lPlatformId );
        pResponse["result"][i]["name"] =  entity.sName ;
        ShopInfoEntity shop;
        ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(entity.lPlatformId),1,shop);
        pResponse["result"][i]["shopName"] =  shop.contentEntity.sShopName ;

        pResponse["result"][i]["coin"] =  entity.dCoin;
        pResponse["result"][i]["coinType"] =  entity.iCoinType;

        pResponse["result"][i]["canot"] =  entity.sCanot;
        pResponse["result"][i]["content"] =  entity.sContent;
        if(entity.sDetailPics.empty()){
            entity.sDetailPics = "[]";
        }
        tConvert.Str2Json(entity.sDetailPics,pResponse["result"][i]["detailPics"]);
        pResponse["result"][i]["expireTime"] =  entity.sExpireTime;
        if(entity.sLaw.empty()){
            entity.sLaw = "[]";
        }
        tConvert.Str2Json(entity.sLaw,pResponse["result"][i]["law"]);

        pResponse["result"][i]["utime"] =  entity.sUtime;
        pResponse["result"][i]["verify"] =  entity.iVerify;


    }
    if( lst.size() <= 0){
        pResponse["result"].resize(0);
    }

    pResponse["total"] = iTotalCount;
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//修改展示产品
int ShopInfoBLL::UpProduct( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);


    string sPicLink;
    if(  !pJson["sPicLink"].isNull()){
        sPicLink = tConvert.Json2String( pJson["sPicLink"]);
    }

    string sName;
    if(  !pJson["sName"].isNull()){
        sName = tConvert.Json2String( pJson["sName"]);
    }

    double dPrice = -1.0;
    if(  !pJson["dPrice"].isNull()){
        dPrice = tConvert.Json2Double( pJson["dPrice"]);
        if( dPrice <=0 || dPrice > 99999999){
            RetJsonMsg::SetRetMsg(-21,pResponse);
            return -21;
        }
    }


    iRet = mShowDAL.Update(id,sPicLink,sName,dPrice,-1.0,-1,"","","","","","");

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//修改展示产品
int ShopInfoBLL::UpProduct2( long lPlatformId, const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    long id = tConvert.Json2Long( pJson["id"]);


    string sPicLink;
    if(  !pJson["picLink"].isNull()){
        sPicLink = tConvert.Json2String( pJson["picLink"]);
    }

    string sName;
    if(  !pJson["name"].isNull()){
        sName = tConvert.Json2String( pJson["name"]);
    }

    double dPrice = -1.0;
    if(  !pJson["price"].isNull()){
        dPrice = tConvert.Json2Double( pJson["price"]);
        if( dPrice <0 || dPrice > 99999999){
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;
        }
    }


    int coinType = -1;
    if(  !pJson["coinType"].isNull()){
        coinType = tConvert.Json2Int( pJson["coinType"]);
    }
    double coin = 1.0;
    if(  !pJson["coin"].isNull()){
        coin = tConvert.Json2Double( pJson["coin"]);
        if( coin <0 || coin > 99999999){
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;
        }
    }
    string content;
    if(  !pJson["content"].isNull()){
        content = tConvert.Json2String( pJson["content"]);
    }
    string expireTime;
    if(  !pJson["expireTime"].isNull()){
        expireTime = tConvert.Json2String( pJson["expireTime"]);
    }
    string canot ;
    if(  !pJson["canot"].isNull()){
        canot   = tConvert.Json2String( pJson["canot"]);
    }

    string utime ;
    if(  !pJson["utime"].isNull()){
        utime   = tConvert.Json2String( pJson["utime"]);
    }
    string law ;
    if(  !pJson["law"].isNull() && pJson["law"].isArray()){
        Json::Value j =  pJson["law"];
        law   = j.toStyledString();
    }
    string detailPics ;
    if(  !pJson["detailPics"].isNull() && pJson["detailPics"].isArray()){
        Json::Value j =  pJson["detailPics"];
        detailPics   = j.toStyledString();
    }


    iRet = mShowDAL.Update(id,sPicLink,sName,dPrice,coin,coinType,canot,content,detailPics,expireTime,utime,law);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//检查查看密码
int ShopInfoBLL::Check( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["lPlatformId"].isNull() || pJson["sPassWord"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);

    ShopSetting setting;
    mShopSettingDal.GetEntity(lPlatformId,setting);
    iRet = -6;
    if( setting.sCheckPwd.empty() && Md5ToLower("T888888") == sPassWord){
        iRet = 0;
    }else if( setting.sCheckPwd == sPassWord){
        iRet = 0;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


//Modify查看密码
int ShopInfoBLL::ModifyCheck( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["lPlatformId"].isNull() || pJson["sPassWord"].isNull()  || pJson["sNewPassWord"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long lPlatformId = tConvert.Json2Long( pJson["lPlatformId"]);
    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);
    string sNewPassWord = tConvert.Json2String( pJson["sNewPassWord"]);

    ShopSetting setting;
    iRet = mShopSettingDal.GetEntity(lPlatformId,setting);

    ShopInfoEntity shopinfo;
    int iType = 0;//原密码类型默认

    if( !pJson["iType"].isNull()){
        iType = tConvert.Json2Int( pJson["iType"]);
    }

    if( 1 == iType){//密码类型是登陆密码
        ShopInfoDAL::GetInstance()->GetEntity( tConvert.LongToStr(lPlatformId),1, shopinfo);
    }

    string sMd5 = Md5ToLower("T888888");

    bool bAdd = false;

    if( iRet == ERRNOTDATA ){



        if(  (0 == iType) && (sMd5 == sPassWord )){
            bAdd = true;
        }else if( (1 == iType) && (sPassWord == shopinfo.contentEntity.sPassWord )){//密码类型是登陆密码
            bAdd = true;
        }

        if( bAdd){
            ShopSetting set;
            set.lPlatformId = lPlatformId;
            set.iCashBack = enum_cashback_close;
            set.iCoinBack = enum_coinback_close;
            set.iRestrict = enum_buyrestrict_false;
            set.iDelivery = enum_delivery_close;
            set.iPark = enum_park_close;
            set.iWifi = enum_wifi_close;

            set.lAddTime = tConvert.GetCurrentTime();
            set.lStartTime = set.lAddTime;
            set.sCheckPwd = sNewPassWord;
            iRet = mShopSettingDal.Add(set);
        }else{
            iRet = -6;
        }
    }else {

        if( (0 == iType) && (setting.sCheckPwd == sPassWord )){
            bAdd = true;
        }else if( (1 == iType) && (sPassWord == shopinfo.contentEntity.sPassWord )){//密码类型是登陆密码
            bAdd = true;
        }

        if( bAdd){
            iRet = mShopSettingDal.UpdateCheck(lPlatformId, sNewPassWord);
        }else{
            iRet = -6;
        }
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


//设置APP secret
int ShopInfoBLL::SetSecret( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["sAppKey"].isNull() || pJson["sAppSecret"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sAppKey = tConvert.Json2String( pJson["sAppKey"]);
    string sAppSecret = tConvert.Json2String( pJson["sAppSecret"]);
    iRet = mKeySecretDAL.IsExist(sAppKey);
    if( 0 == iRet){
        iRet = mKeySecretDAL.UpSecret(sAppKey, sAppSecret);
    }else{
        iRet = -5;
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}

//验证key和secret，然后返回平台号
int ShopInfoBLL::GetIdBySecret( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["sAppKey"].isNull() || pJson["sAppSecret"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    string sAppKey = tConvert.Json2String( pJson["sAppKey"]);
    string sAppSecret = tConvert.Json2String( pJson["sAppSecret"]);

    string sSecret;long lPlatformId = 0;
    iRet = mKeySecretDAL.Get(sAppKey,sSecret , lPlatformId);
    if( sAppSecret != sSecret){
        iRet = 0;
        pResponse["lPlatformId"] = tConvert.LongToStr(lPlatformId);

    }else{
        iRet = -6;
    }



    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


//某个时间点之前各个区的开通联盟商家数（0个的不算）
int ShopInfoBLL::CityShopCount( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["sEndTime"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pResponse["list"];
    pResponse["list"].resize(0);
    string sEndTime = tConvert.Json2String( pJson["sEndTime"]) + " 23:59:59";

    CommonStructVector out;
    ShopInfoDAL::GetInstance()->CityShopCount((long)tConvert.StrToTime(sEndTime.c_str())*1000000,out);

    int i = 0;
    for(CommonStructVector::iterator it = out.begin(); it != out.end(); it++){
        CommonStruct & en =*it;
        pResponse["list"][i]["sCity"] = en.v1;
        pResponse["list"][i]["iCount"] = en.v2;
        i++;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;



}


//获取专题列表
int ShopInfoBLL::GetObjects( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    pResponse["list"];
    pResponse["list"].resize(0);

    SubjectLst subjects;
    iRet = mSubjectDAL.GetAll(subjects);

    int i = 0;
    for(SubjectLst::iterator it = subjects.begin(); it != subjects.end(); it++){
        Subject & en =*it;
        pResponse["list"][i]["Id"] = tConvert.LongToStr( en.Id );
        pResponse["list"][i]["lAddTime"] = tConvert.TimeToStr( en.lAddTime/1000000 );
        pResponse["list"][i]["iState"] =  en.iState ;
        pResponse["list"][i]["sBigBottonPic"] =  en.sBigBottonPic ;
        pResponse["list"][i]["sBigTopPic"] =  en.sBigTopPic ;
        pResponse["list"][i]["sSmallTopPic"] =  en.sSmallTopPic ;
        pResponse["list"][i]["sSmallBottonPic"] =  en.sSmallBottonPic ;
        pResponse["list"][i]["sRemark"] =  en.sRemark ;
        pResponse["list"][i]["sName"] =  en.sName ;

        i++;
    }

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;
}


//获取专题店铺
int ShopInfoBLL::GetShopsByObjest( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;

    if(  pJson["Id"].isNull() || pJson["iPage"].isNull() || pJson["iCount"].isNull()  ||
         pJson["dGpsLat"].isNull() || pJson["dGpsLon"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }


    pResponse["list"];
    pResponse["list"].resize(0);
    int Id  = tConvert.Json2Int( pJson["Id"]) ;


    ShopQueryEntity pQueryEntity;



    pQueryEntity.dGpsLat = tConvert.Json2Double( pJson["dGpsLat"]);

    pQueryEntity.dGpsLon = tConvert.Json2Double( pJson["dGpsLon"]);


    int iPage = tConvert.Json2Int( pJson["iPage"]);
    int piCount =  tConvert.Json2Int( pJson["iCount"] );
    if( piCount > 20){
        piCount = 20;
    }
    int  piIndex = (iPage -1 )* piCount;

    pQueryEntity.iShowNearby  =1;//1显示
    pQueryEntity.iVerify = 1;//1 审核通过

    if( !pJson["sCity"].isNull() ){
        pQueryEntity.sCtiy = tConvert.Json2String(pJson["sCity"]);
    }else{
        pQueryEntity.sCtiy = "citys";
    }

    if( Id == 1){
        pQueryEntity.iEnumDiscount = 0;
    }else if( Id == 3){
        pQueryEntity.sShopType = "美食";
    }



    ShopInfoList  plsTmp;
    int iTotalCount;

    iRet = ShopInfoDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

    if( plsTmp.size()== 0){
        pQueryEntity.sCtiy = "citys";

        iRet = ShopInfoDAL::GetInstance()->GetList(pQueryEntity,piIndex,piCount,plsTmp,iTotalCount);

    }



    if( 0 == iRet ){
        DataLst2Json(plsTmp,pResponse,2);
        //返回总个数
        pResponse["iTotal"]  = iTotalCount;
    }
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;

}


//获取快速评论
int ShopInfoBLL::GetQuikMark( const  Json::Value  & pJson ,Json::Value & pResponse )
{

    if(  pJson["lUserId"].isNull()  ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }

    ifstream infile;
    infile.open("/tiyoume/MepayHttp/quikmark.txt");   //将文件流对象与文件连接起来

    pResponse["list"];
    pResponse["list"].resize(0);

    string s;int i = 0;
    while(getline(infile,s))
    {
        pResponse["list"][i++] = s;
    }
    infile.close();             //关闭文件输入流

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}


//获取联盟商家分类
int ShopInfoBLL::GetClass( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if(  pJson["sCity"].isNull() || pJson["sProvince"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iRet = -1;
    string sCity = tConvert.Json2String(pJson["sCity"]);
    string sProvince = tConvert.Json2String(pJson["sProvince"]);


    ifstream infile;
    infile.open("/tiyoume/MepayHttp/jilin.txt");   //将文件流对象与文件连接起来
    string s;
    while(getline(infile,s))
    {
        if( sCity.find(s.c_str()) != string::npos){
            sProvince = "吉林省";
            break;
        }
    }
    infile.close();             //关闭文件输入流





    NearCalssLst nearClass;
    iRet = mClassDAL.GetAll(sProvince,nearClass);

    int i = 0;
    for(NearCalssLst::iterator it = nearClass.begin(); it != nearClass.end(); it++){
        NearCalss & en =*it;

        pResponse["list"][i]["Id"] = tConvert.LongToStr( en.Id );
        pResponse["list"][i]["sBigPic"] =  en.sBigPic ;
        pResponse["list"][i]["sSmallPic"] =  en.sSmallPic ;
        pResponse["list"][i]["sAction"] =  en.sAction ;
        pResponse["list"][i]["sName"] =  en.sName ;

        i++;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}


//获取个代开的供应商
int ShopInfoBLL::OpenbyPerAgent( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if(  pJson["sAccount"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    pResponse["list"].resize(0);
    TConvert tConvert;
    int iRet = -1;
    string sAccount = tConvert.Json2String(pJson["sAccount"]);
    long lPerAgentId = 0;
    iRet = GetUserInfoByPhone(sAccount,lPerAgentId);
    if( lPerAgentId > 0){
        ShopInfoList shops;
        iRet = ShopInfoDAL::GetInstance()->OpenByPerAgent(lPerAgentId,shops);


        if( 0 == iRet ){
            DataLst2Json(shops,pResponse,1);
        }

    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}

//特殊促销配置
int ShopInfoBLL::PromotionConfig( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if(  pJson["sPlatformId"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert tConvert;
    int iRet = -1;
    string sPlatformId = tConvert.Json2String(pJson["sPlatformId"]);
    int iCoinPercent = tConvert.Json2Int( pJson["iCoinPercent"]);
    int iPromotionDiscount = tConvert.Json2Int( pJson["iPromotionDiscount"]);

    iRet = ShopInfoDAL::GetInstance()->PromotionConfig(sPlatformId,iCoinPercent,iPromotionDiscount);
    RetJsonMsg::SetRetMsg(iRet,pResponse);
    return iRet;


}


//根据平台号获取代理信息
int ShopInfoBLL::GetAgentInfoByPlatformId( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    string sPlatformId = tConvert.Json2String(pJson["lPlatformId"]);

    bool bAgent = false;

    ShopInfoEntity shopEntity;
    ShopInfoDAL::GetInstance()->GetEntity(sPlatformId,1,shopEntity);

    ZoneAgentList zonelst;
    ZoneAgentDAL::GetInstance()->GetZoneByPhone(shopEntity.contentEntity.sShopPhone,zonelst);


    string sCity;
    for( int i = 0; i < zonelst.size(); i++){
        ZoneAgentEntity & tmp =  zonelst[i];
        sCity += tmp.contentEntity.sCity + ",";
    }
    if( !sCity.empty()){
        sCity = sCity.substr(0,sCity.length() -1);
    }

    if( zonelst.size() > 0){
        bAgent =true;
    }


    pResponse["bAgent"] = bAgent;
    pResponse["lAgentId"] = tConvert.LongToStr(shopEntity.contentEntity.lUserId);
    pResponse["sAgentPhone"] = shopEntity.contentEntity.sShopPhone;
    pResponse["sAgentZone"] = sCity;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;


}


//获取属于我商店(收款记录)
int ShopInfoBLL::MyShop( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    long userId = tConvert.Json2Long(pJson["userId"]);

    int iType = 1;
    ShopInfoList  plsTmp;
    int iRet  = ShopInfoDAL::GetInstance()->GetMyShop(userId, iType,plsTmp);

    Json::Value & jResult = pResponse["result"];
    int iIndex = 0;
    for( ShopInfoList::iterator it = plsTmp.begin(); it != plsTmp.end() ; it++){
        ShopInfoEntity & en = *it;
        jResult[iIndex]["sPrimaryKey"] = en.sPrimaryKey;
        jResult[iIndex]["sShopFrontPhoto"] = en.contentEntity.sShopFrontPhoto;
        jResult[iIndex]["sShopName"] = en.contentEntity.sShopName;
        jResult[iIndex]["sAddress"] = en.contentEntity.sAddress;
        jResult[iIndex]["sShopPhone"] = en.contentEntity.sShopPhone;
        jResult[iIndex]["sAccount"] = en.contentEntity.sAccount;

    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//获取属于我授权商店(收款记录)
int ShopInfoBLL::MyImpowerShop( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert tConvert;
    long userId = tConvert.Json2Long(pJson["userId"]);

    ShopInfoList  plsTmp;
    int iRet  = ShopInfoDAL::GetInstance()->MyImpowerShop(userId,plsTmp);

    Json::Value & jResult = pResponse["result"];
    int iIndex = 0;
    for( ShopInfoList::iterator it = plsTmp.begin(); it != plsTmp.end() ; it++){
        ShopInfoEntity & en = *it;
        jResult[iIndex]["sPrimaryKey"] = en.sPrimaryKey;
        jResult[iIndex]["sShopFrontPhoto"] = en.contentEntity.sShopFrontPhoto;
        jResult[iIndex]["sShopName"] = en.contentEntity.sShopName;
        jResult[iIndex]["sAddress"] = en.contentEntity.sAddress;
        jResult[iIndex]["sShopPhone"] = en.contentEntity.sShopPhone;
        jResult[iIndex]["sAccount"] = en.contentEntity.sAccount;

    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

/*计算两点距离
  * return : 两点距离(米)
  * */
double ShopInfoBLL::GetDistance(double pdLat1,double pdLong1 , double pdLat2,double pdLong2) {
    double PI = 3.14159265358979323; // 圆周率
    double R = 6371229; // 地球的半径

    double x, y, distance;
    x = (pdLong2 - pdLong1) * PI * R * cos(((pdLat1 + pdLat2) / 2) * PI / 180) / 180;
    y = (pdLat2 - pdLat1) * PI * R / 180;
    distance = hypot(x, y);

    return distance;
}
//排序 ByDistance
bool ShopInfoBLL::SortByDistance(const ShopInfoEntity & f , const ShopInfoEntity & s){
    if(f.contentEntity.lDistance < s.contentEntity.lDistance){
        return true;
    }else{
        return false;
    }
}
//排序 ByAddTime
bool ShopInfoBLL::SortByAddTime(const ShopInfoEntity & f , const ShopInfoEntity & s){
    if(f.lAddTime > s.lAddTime){
        return true;
    }else{
        return false;
    }
}


void ShopInfoBLL::DataLst2Json(  ShopInfoList & pLst,Json::Value & pResponse , int piFilt){
    int iCount = 0;
    TConvert tConvert;
    while(iCount <  pLst.size()){

        ShopInfoEntity & entity = pLst[ iCount ];

        pResponse["list"][iCount ]["iTicketScale"] = GetGiveCoin( entity.contentEntity.iOffLineDisCount) ;
        if( piFilt == 1){
            pResponse["list"][iCount ]["lAddTime"] = tConvert.LongToStr(entity.lAddTime);
            // pResponse["list"][iCount ]["lAddTime"] = tConvert.TimeToStr(entity.lAddTime/1000000);
            pResponse["list"][iCount ]["lServiceUserId"] =  tConvert.LongToStr( entity.contentEntity.lServiceUserId);
            pResponse["list"][iCount ]["sCompanyName"] = entity.contentEntity.sCompanyName;
            pResponse["list"][iCount ]["sShopPhone"] =  entity.contentEntity.sShopPhone;
            pResponse["list"][iCount ]["sShopDetailPhoto"] = entity.contentEntity.sShopDetailPhoto;
            pResponse["list"][iCount ]["sAddress"] = entity.contentEntity.sAddress;
            pResponse["list"][iCount ]["dGpsLat"] = entity.contentEntity.dGpsLat;
            pResponse["list"][iCount ]["dGpsLon"] = entity.contentEntity.dGpsLon;
            pResponse["list"][iCount ]["iOffLineDisCount"] =  entity.contentEntity.iOffLineDisCount;
            pResponse["list"][iCount ]["sLicensePhoto"] = entity.contentEntity.sLicensePhoto;
            pResponse["list"][iCount ]["sIdPhoto"] = entity.contentEntity.sIdPhoto;
            pResponse["list"][iCount ]["sHours"] =  entity.contentEntity.sHours;
            pResponse["list"][iCount ]["sPosNum"] = entity.contentEntity.sPosNum;
            pResponse["list"][iCount ]["lPerAgentId"] = tConvert.LongToStr( entity.contentEntity.lPerAgentId);

            pResponse["list"][iCount ]["sPerAgentPhone"] = mUserIdPhoneMap[entity.contentEntity.lPerAgentId];
            pResponse["list"][iCount ]["iAwardingPoints"] =  entity.contentEntity.iAwardingPoints;
            pResponse["list"][iCount ]["sSevvicePhone"] = entity.contentEntity.sSevvicePhone;
            pResponse["list"][iCount ]["iAvgConsume"] =  entity.contentEntity.iAvgConsume;
            pResponse["list"][iCount ]["iShowNearby"] = entity.contentEntity.iShowNearby;
            pResponse["list"][iCount ]["iProrate"] = entity.contentEntity.iProrate;
            pResponse["list"][iCount ]["lUserId"] =  tConvert.LongToStr(  entity.contentEntity.lUserId );



        }else if( piFilt == 2){

        }
        ShopSetting setting;
        setting.iRestrict = -1;
        mShopSettingDal.GetEntity(tConvert.StrToLong( entity.sPrimaryKey ) , setting);
        if( setting.iRestrict == 1 && entity.contentEntity.iCoinPercent > 0){
            pResponse["list"][iCount ]["iTicketScale"] = entity.contentEntity.iCoinPercent;
        }


        pResponse["list"][iCount ]["sPrimaryKey"] = entity.sPrimaryKey;
        pResponse["list"][iCount ]["sAccount"] = entity.contentEntity.sAccount;
        pResponse["list"][iCount ]["sShopName"] = entity.contentEntity.sShopName;
        if( entity.contentEntity.sShopType == "无" ){
            entity.contentEntity.sShopType = "其他";
        }
        pResponse["list"][iCount ]["sShopType"] = entity.contentEntity.sShopType;

        pResponse["list"][iCount ]["sShopFrontPhoto"] =  entity.contentEntity.sShopFrontPhoto;
        pResponse["list"][iCount ]["sBusiness"] = entity.contentEntity.sBusiness;
        pResponse["list"][iCount ]["sCtiy"] =  entity.contentEntity.sCtiy;
        pResponse["list"][iCount ]["lDistance"] =  tConvert.LongToStr(  entity.contentEntity.lDistance );
        if( entity.contentEntity.lDistance < 1000){
            pResponse["list"][iCount ]["sDistance"] =  tConvert.LongToStr(  entity.contentEntity.lDistance ) + "米";

        }else{
            pResponse["list"][iCount ]["sDistance"] =  tConvert.LongToStr(  entity.contentEntity.lDistance /1000) + "千米";
        }

        pResponse["list"][iCount ]["iVerify"] = entity.contentEntity.iVerify;
        pResponse["list"][iCount ]["iAuth"] = entity.contentEntity.iAuth;
        pResponse["list"][iCount ]["sOtherCerPhoto"] = entity.contentEntity.sOtherCerPhoto;
        pResponse["list"][iCount ]["sTradeMark"] = entity.contentEntity.sTradeMark;
        pResponse["list"][iCount ]["iScore"] =GetAvgScore(tConvert.StrToLong(entity.sPrimaryKey),0);

        pResponse["list"][iCount]["iCoinType"] =  entity.contentEntity.iCoinType;

        pResponse["list"][iCount]["bPerfect"] =  IsShopPerfect(entity);



        iCount++;
    }
    if( pLst.size() <= 0){
        pResponse["list"] ;
        pResponse["list"].resize(0);
    }

}
//更改关注
bool ShopInfoBLL::Attention(const long & plUserId, const long & plPlatformId)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["lUserId"]= tConvert.LongToStr( plUserId);
    jsonValue["lPlatformId"]= tConvert.LongToStr( plPlatformId );

    //URL
    string sUrl = "http://" + TBLL::GetInstance()->GetMechatIP() + ":23241/?sBusiness=UserRegister&sMethod=Attention";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    //  appendlog(TTDLogger::LOG_DEBUG,"UserDataBLL::IsUserPhoneCode sReturn=%s",sReturn.c_str());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return false;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return false;
    }

    return true;
}


void ShopInfoBLL::SetDefault( ShopInfoEntity & entity)
{
    entity.iState = -1;
    entity.contentEntity.dGpsLat  = -1.0;
    entity.contentEntity.dGpsLon = -1.0;
    entity.contentEntity.iAvgConsume = -1;
    entity.contentEntity.iAwardingPoints = -1;
    entity.contentEntity.iOffLineDisCount = -1;
    entity.contentEntity.iProrate = -1;
    entity.contentEntity.iShowNearby = -1;
    entity.contentEntity.iVerify = -1;
    entity.contentEntity.lDistance = -1;
    entity.contentEntity.lPerAgentId = -1;
    entity.contentEntity.lServiceUserId  = -1;
    entity.contentEntity.lUserId = -1;
    entity.contentEntity.iAuth = -1;

}



//发送商家开通通知 by sms
bool ShopInfoBLL::SendPhoneNotifyCode(const string& psPhone, const string& psMsg,string sType)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["sPhone"]=psPhone;
    jsonValue["sType"]= sType;
    jsonValue["sMsg"]=psMsg;
    //URL
    string sUrl = "http://"+ TBLL::GetInstance()->GetPhonesmsIP() +":23240/?sBusiness=phonesms&sMethod=CodeTTD";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    if( sReturn == ""){
        sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    }
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return false;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return false;
    }

    return true;
}

//验证短信验证码
bool ShopInfoBLL::IsUserPhoneCode(const string& psPhone, const string& psCode)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["sPhone"]=psPhone;
    jsonValue["sCode"]=psCode;

    //URL
    string sUrl = "http://"+TBLL::GetInstance()->GetPhonesmsIP()+":23240/?sBusiness=phonesms&sMethod=UseCode";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    appendlog(TTDLogger::LOG_DEBUG,"UserDataBLL::IsUserPhoneCode sReturn=%s",sReturn.c_str());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return false;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return false;
    }

    return true;
}


//发送商家开通通知,by Im
bool ShopInfoBLL::SendNotify(long lToUserId, const string& sPlatformId, const string & sPwd)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue,jsonContent;
    jsonContent["sTitle"] = "开通了商家通知";
    jsonContent["iSubType"] = 1;// 1 开通店
    jsonContent["平台号"] = sPlatformId;
    jsonContent["密码"] = sPwd;
    jsonValue["lSendId"]= tConvert.LongToStr(28);
    jsonValue["iTarget"]= 2;//2 组播或单播
    jsonValue["sContent"]=jsonContent;
    jsonValue["sRevIds"][0]= tConvert.LongToStr( lToUserId );
    jsonValue["iType"]=2;//2 消息
    //URL
    string sUrl = "http://"+ TBLL::GetInstance()->GetMechatIP() +":23241/?sBusiness=Notice&sMethod=Send";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    if( sReturn == ""){
        sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    }
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return false;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return false;
    }

    return true;
}

//发送联盟商家信息更新通知,by Im
bool ShopInfoBLL::SendModifyShopNotice( long lPlatformId, const string & sShopName, const string & sShopAddr
                                        ,int iCoinType, int iDiscount)
{
    return true;
    if( sShopAddr.empty() && sShopName.empty() && iCoinType== -1 && iDiscount == -1){
        return true;
    }
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue,jsonContent;
    jsonContent["sTitle"] = "联盟商家信息更新";
    jsonContent["iSubType"] = 10;//
    if( !sShopName.empty()){
        jsonContent["更名为"] = sShopName;
    }
    if( iDiscount != -1){
        jsonContent["赠送购物币比例更新为"] =  "送" +  tConvert.IntToStr( GetGiveCoin(iDiscount)) + "%购物币";
    }
    if( !sShopAddr.empty()){
        jsonContent["店铺地址更新为"] = sShopAddr;
    }
    if( iCoinType != -1){
        if( enum_cointype_private == iCoinType){
            jsonContent["赠送币种更新为"] = "专属购物币";
        }else{
            jsonContent["赠送币种更新为"] = "普通购物币";
        }
    }


    jsonValue["lSendId"]= tConvert.LongToStr(28);
    jsonValue["iTarget"]= 2;//2 组播或单播
    jsonValue["sContent"]=jsonContent;
    jsonValue["sRevIds"][0]= tConvert.LongToStr( lPlatformId );
    jsonValue["iType"]=2;//2 消息


    Json::Value jPost,jRespons;
    jPost["lPlatformId"] =  tConvert.LongToStr(lPlatformId);
    if( 0 == Request::PlatformFans(TBLL::GetInstance()->GetMechatIP(),jPost,jRespons )){//是否要考虑粉丝太多引起卡顿的情况

        Json::Value jFans = jRespons["list"];
        for(int i = 0; i < jFans.size() ; i++)
        {
            jsonValue["sRevIds"][i] = jFans[i]["lUserId"];
        }
    }



    string sUrl = "http://"+ TBLL::GetInstance()->GetMechatIP() +":23241/?sBusiness=Notice&sMethod=Send";

    jRespons.clear();
    Request::CppHttpPost(sUrl,jsonValue,jRespons);

    return true;

}

//通过手机获取用户信息
int ShopInfoBLL::GetUserInfoByPhone(const string & psPhone,long & plUserId)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["sAccount"]=psPhone;
    //URL
    string sUrl = "http://" + TBLL::GetInstance()->GetMechatIP() + ":23241/?sBusiness=UserQuery&sMethod=GetUserInfo";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return -1;
    }
    plUserId = tConvert.Json2Int(jsonReturn["entity"]["lUserId"]);

    return 0;

}

//通过用户获取手机信息
int ShopInfoBLL::GetUserInfoById(long  lUserId, string & sPhone)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue,jsonReturn;
    jsonValue["lUserId"]= tConvert.LongToStr(lUserId );
    //URL
    string sUrl = "http://" + TBLL::GetInstance()->GetMechatIP() + ":23241/?sBusiness=UserQuery&sMethod=GetUserInfo";

    //获取HTTP请求
    int iRet = Request::CppHttpPost(sUrl,jsonValue,jsonReturn);
    if( iRet == 0){
        sPhone = tConvert.Json2String(jsonReturn["entity"]["sAccount"]);
    }
    return iRet;

}

//是否正确的验证码
int ShopInfoBLL::UserPhoneCode(const string & psPhone, const string & psCode)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue;
    jsonValue["sPhone"]=psPhone;
    jsonValue["sCode"]=psCode;

    //URL
    string sUrl = "http://" + TBLL::GetInstance()->GetPhonesmsIP() + ":23240/?sBusiness=phonesms&sMethod=UseCode";

    //获取HTTP请求
    PostsAes postsAes;
    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
    Json::Value jsonReturn;
    Json::Reader reader;
    if (!reader.parse(sReturn, jsonReturn)){
        return -1;
    }
    if (0 != tConvert.Json2Int(jsonReturn["iRet"])) {
        return -1;
    }

    return 0;

}


//从商家帐号提取拼音
string ShopInfoBLL::GetPinFromAccount(string sAccount)
{
    int len = sAccount.length();
    string sPin;
    for(int i = 0 ; i < len ; i++){
        char c = sAccount[i];
        if( c < 0x30 || c > 0x39){
            sPin.push_back(c);
        }else {
            break;
        }
    }
    return sPin;
}


//发起调用C#服务端
int ShopInfoBLL::Requst(const string & sUrl, const Json::Value & jsonPost, Json::Value & jsonRespons)
{
    TConvert tConvert;int iRet = -1;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),sAesKey.size());
    string sAesPostData = myAES.Encrypt(jsonPost.toStyledString(),4);
    string sRealPostData = "sCondition=" + sAesPostData;


    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    string sAesReturn;
    TCurl tCurl;

    iRet =tCurl.Posts(sUrl,httpHeaderVector,sRealPostData,sAesReturn,NULL);

    if( 0 == iRet ){
        string sReturn =  myAES.Decrypt(sAesReturn,4);
        Json::Reader reader;
        if (!reader.parse(sReturn, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["iRet"] );
    }
    return iRet ;
}

//获取商城产品
int ShopInfoBLL::GetProduct(int iPage, long lUserId, string sSearch,Json::Value & jsonRespons)
{
    TConvert tConvert;
    TCurl tCurl;

    string sUrl;

    sUrl = "http://" + TBLL::GetInstance()->GetLeduiShop() + "/shop/productService/getProductListByText/";
    sUrl += tConvert.IntToStr(iPage) + "/";
    sUrl += tConvert.LongToStr(lUserId) + "/";
    sUrl +=  tConvert.URLEncode(sSearch);

    string sResponse;
    int iRet =tCurl.Get(sUrl,sResponse);
    if( 0 == iRet ){
        Json::Reader reader;
        if (!reader.parse(sResponse, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["state"] );
    }
    return iRet;
}

//得到赠送购物币比例
int ShopInfoBLL::GetGiveCoin(int iDiscount){
    return (100 - iDiscount) * 10 / 2;
}

double ShopInfoBLL::GetAvgScore(long lPlatfromId,long lProductId)
{
    return (double)mCommentDAL.GetAvgSorce(lPlatfromId,lProductId)/10;
}


//long ShopInfoBLL::GetPlatformId(const string & sToken)
//{
//    Json::Value jsonValue;
//    TConvert tConvert;
//    int iRet = tConvert.Str2Json(sToken,jsonValue );
//    if( jsonValue["lPlatformId"].isNull()){
//        return -1;
//    }
//    return tConvert.Json2Long( jsonValue["lPlatformId"] );
//}


