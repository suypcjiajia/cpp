#include <utility>
#include "CouponBLL.h"
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
CouponBLL* CouponBLL::mInstance = NULL;
CouponBLL* CouponBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new CouponBLL;
    }
    return mInstance;
}


//处理带token，不加密的情况
int CouponBLL::Process2(EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse)
{
    int iRet = -1;
    TConvert tConvert;

    string sUrl = pEvhtp->GetUrl();


    if( sUrl.find( "/coupon/productDetail") != string::npos ){

        iRet = ProductDetail(pJson,pResponse);

    }
    return iRet;
}


int CouponBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;
    if( "GetMyCoupon" == psMethod){

        iRet = GetMyCoupon(pJson,pResponse);

    }else if( "Order" == psMethod){

        iRet = Order(pJson,pResponse);

    }else if( "Detail" == psMethod){
        iRet = Detail(pJson,pResponse);
    }else if( "IsTimeOut" == psMethod){
        iRet = IsTimeOut(pJson,pResponse);
    }else if( "ReqBack" == psMethod){
        iRet = ReqBack(pJson,pResponse);
    }else if( "WriteCommtent" == psMethod){
        iRet = WriteCommtent(pJson,pResponse);
    }else if( "ProductDetail" == psMethod){
        iRet = ProductDetail(pJson,pResponse);
    }else if( "Check" == psMethod){
        iRet = Check(pJson,pResponse);
    }else if( "Agree" == psMethod){
        iRet = Agree(pJson,pResponse);
    }else if( "RejBack" == psMethod){
        iRet = RejBack(pJson,pResponse);
    }else if( "GetCouponByPlatfrom" == psMethod){
        iRet = GetCouponByPlatfrom(pJson,pResponse);
    }else if( "CancelBack" == psMethod){
        iRet = CancelBack(pJson,pResponse);
    }else if( "ProductTimeOut" == psMethod){
        iRet = ProductTimeOut(pJson,pResponse);
    }else if( "Loop" == psMethod){
        iRet = Loop(pJson,pResponse);
    }else if( "TypeInfo" == psMethod){
        iRet = TypeInfo(pJson,pResponse);
    }else if( "GetPageByType" == psMethod){
        iRet = GetPageByType(pJson,pResponse);
    }

    return iRet;
}



//初始化
int CouponBLL::Init()
{
    return 0;
}

//获取我的券
int CouponBLL::GetMyCoupon(const  Json::Value  & pJson,
                           Json::Value  & pResponse)
{

    int iRet = -1;

    if( pJson["userId"].isNull() || pJson["page"].isNull() || pJson["count"].isNull()
            || pJson["state"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    int page  =   tConvert.Json2Int(pJson["page"]);
    int count  =   tConvert.Json2Int(pJson["count"]);
    string state  =   tConvert.Json2String( pJson["state"]);




    CouponLst  lst;

    iRet = mCouponOrderDAL.GetMyCoupon( userId,state,page,count,lst);

    int iCount = 0;
    while( iCount < lst.size()){
        Coupon  & entity = lst[iCount];
        pResponse["result"][iCount]["orderNo"] = entity.orderNo;
        pResponse["result"][iCount]["payTime"] =  entity.payTime;
        pResponse["result"][iCount]["idPlatform"] = tConvert.LongToStr( entity.idPlatform) ;
        pResponse["result"][iCount]["idProduct"] = tConvert.LongToStr(entity.idProduct);
        pResponse["result"][iCount]["expireTime"] = entity.expireTime;        
        pResponse["result"][iCount]["shopName"] =  entity.shopName;
        pResponse["result"][iCount]["shopType"] =  GetShopType( entity.shopType );


        pResponse["result"][iCount]["content"] =  entity.content;
        pResponse["result"][iCount]["name"] =  entity.name;
        pResponse["result"][iCount]["picLink"] =  entity.picLink;
        pResponse["result"][iCount]["state"] =  entity.couponCode.state;
        pResponse["result"][iCount]["backTime"] = entity.couponCode.backTime;
        pResponse["result"][iCount]["specification"] = entity.specification;

        iCount++;
    }
    if( lst.size() <= 0){
        pResponse["result"] ;
        pResponse["result"].resize(0);
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;
}


//下单
int CouponBLL::Order(const  Json::Value  & pJson,
                     Json::Value & pResponse)
{
    int iRet = -1;

    if( pJson["userId"].isNull() || pJson["idProduct"].isNull() || pJson["idPlatform"].isNull()
            || pJson["num"].isNull() || pJson["orderNo"].isNull() || pJson["payType"].isNull()
            || pJson["password"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    long idProduct  =   tConvert.Json2Long(pJson["idProduct"]);
    int idPlatform  =   tConvert.Json2Long(pJson["idPlatform"]);
    int num  =   tConvert.Json2Int( pJson["num"]);
    int payType = tConvert.Json2Int( pJson["payType"]);
    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);
    string passWord  =   tConvert.Json2String( pJson["password"]);

    ShowPro showEntity;
    mShowDAL.GetEntity(idProduct,showEntity);

    if( (!showEntity.sExpireTime.empty() &&  showEntity.sExpireTime < tConvert.GetCurrentDay())
            || showEntity.iState == 333 ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        pResponse["msg"] = "商品已失效";
        return -15;
    }

    string sPhone;
    GetUserInfo(userId,sPhone);

    ShopInfoEntity shopInfo;
    ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(idPlatform),1,shopInfo);


    Coupon coupon;
    coupon.addTime = tConvert.GetCurrentTime();
    coupon.idPlatform = idPlatform;
    coupon.idProduct = idProduct;
    coupon.num = num;
    coupon.orderNo = orderNo;
    coupon.payTime = tConvert.TimeToStr( coupon.addTime/1000000 );
    coupon.couponCode.state = couponcode_state_canuser;
    coupon.userId = userId;
    coupon.expireTime = showEntity.sExpireTime;
    coupon.price = showEntity.dPrice;
    coupon.coin = showEntity.dCoin;


    //事务处理
    //start
    mCouponOrderDAL.AutoCommit(false);

    StringLst batchSql;
    string sSql = mCouponOrderDAL.GetSqlAdd( coupon );
    batchSql.push_back(sSql);


    StringLst sOrerNos;

    Json::Value jCode;

    for( int i = 0 ; i < num; i++){

        coupon.couponCode.code =   tConvert.LongToStr(userId) +  tConvert.LongToStr( tConvert.GetCurrentTime()%1000000000 ) ;
        sSql = mCouponCodeDAL.GetSqlAdd(coupon);
        batchSql.push_back(sSql);

        sOrerNos.push_back(coupon.couponCode.code);

        jCode[i] =  coupon.couponCode.code;

    }

    sSql =  mShowDAL.GetSqlIncreemSallNum(idProduct,num);
    batchSql.push_back(sSql);

    iRet = mCouponOrderDAL.BatchBaseQuery(batchSql);


    if(0 == iRet ){

        if( 0 == WalletOrder(sOrerNos,showEntity.dPrice,showEntity.dCoin,payType,idPlatform,sPhone,pResponse)){


            string sPayOrderNo = tConvert.Json2String( pResponse["sPayOrderNo"] );

            pResponse.clear();

            if( 0 == WalletPay(sPayOrderNo,passWord,showEntity.iCoinType,pResponse)){

            }else{
                mCouponOrderDAL.RollBack();
                mCouponOrderDAL.AutoCommit(true);
                return -1;//返回钱包服务的错误
            }

        }else{
            mCouponOrderDAL.RollBack();
            mCouponOrderDAL.AutoCommit(true);
            return -1;//返回钱包服务的错误
        }
    }

    mCouponOrderDAL.Commit();
    mCouponOrderDAL.AutoCommit(true);

    pResponse.clear();

    pResponse["result"]["codes"] = jCode;
    pResponse["result"]["shopName"] = shopInfo.contentEntity.sShopName;
    pResponse["result"]["content"] = showEntity.sContent;
    pResponse["result"]["name"] = showEntity.sName;
    pResponse["result"]["picLink"] = showEntity.sPicLink;
    pResponse["result"]["expireTime"] =showEntity.sExpireTime;
    pResponse["result"]["orderNo"] =orderNo;
    pResponse["result"]["idPlatform"] = tConvert.LongToStr(idPlatform );
    pResponse["result"]["idProduct"] = tConvert.LongToStr( idProduct );
    pResponse["result"]["shopAddr"] = shopInfo.contentEntity.sAddress ;
    pResponse["result"]["specification"] = showEntity.sContent ;


    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//获取一张券的详情
int CouponBLL::Detail(const  Json::Value  & pJson,
                      Json::Value & pResponse)
{
    int iRet = -1;

    if( pJson["userId"].isNull() || pJson["orderNo"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);

    CouponLst lst;
    iRet = mCouponOrderDAL.Detail(orderNo,lst);




    if( lst.size() > 0 ){


        Coupon  & entity = lst[0];

        string sPhone;
        GetUserInfo(entity.userId,sPhone);

        ShowPro showpro;
        mShowDAL.GetEntity(entity.idProduct,showpro);
        pResponse["result"]["orderNo"] = entity.orderNo;
        pResponse["result"]["payTime"] = entity.payTime ;
        pResponse["result"]["idPlatform"] = tConvert.LongToStr( entity.idPlatform) ;
        pResponse["result"]["idProduct"] = tConvert.LongToStr(entity.idProduct);
        pResponse["result"]["expireTime"] = entity.expireTime;
        pResponse["result"]["shopName"] =  entity.shopName;
        pResponse["result"]["shopType"] =  GetShopType( entity.shopType );
        pResponse["result"]["shopPhone"] =  entity.shopPhone;
        pResponse["result"]["shopAddr"] =  entity.shopAddr;
        pResponse["result"]["content"] =  entity.content;
        pResponse["result"]["name"] =  entity.name;
        pResponse["result"]["picLink"] =  entity.picLink;
        pResponse["result"]["price"] =  entity.price*entity.num;
        pResponse["result"]["coin"] =  entity.coin*entity.num;
        pResponse["result"]["num"] =  entity.num;
        pResponse["result"]["coinType"] =  showpro.iCoinType;
        pResponse["result"]["telPhone"] =  sPhone;
        pResponse["result"]["isDel"] =  showpro.iState;
        pResponse["result"]["specification"] =  showpro.sSpecification;




        int iCount =  0;
        while( iCount < lst.size()){
            Coupon  & entity = lst[iCount];

            pResponse["result"]["code"][iCount]["code"] =  entity.couponCode.code;
            pResponse["result"]["code"][iCount]["backTime"] =  entity.couponCode.backTime;
            pResponse["result"]["code"][iCount]["consumTime"] =  entity.couponCode.consumTime;
            pResponse["result"]["code"][iCount]["reason"] =  entity.couponCode.reason;
            pResponse["result"]["code"][iCount]["rejReason"] =  entity.couponCode.rejReason;
            pResponse["result"]["code"][iCount]["reqTime"] =  entity.couponCode.reqTime;
            pResponse["result"]["code"][iCount]["state"] =  entity.couponCode.state;

            pResponse["result"]["state"] =  entity.couponCode.state;

            iCount++;
        }

        CommentEntity comment;
        mCommentDAL.GetOneById(entity.idPlatform,entity.idProduct,userId,comment);
        pResponse["result"]["score"] =  (double)comment.iScore/10;
        pResponse["result"]["markContent"] =  comment.sContent;
        pResponse["result"]["pics"].resize(0);
        if(comment.sPics.empty()){
            comment.sPics = "[]";
        }
        tConvert.Str2Json(comment.sPics,pResponse["result"]["pics"]);


    }else{
        iRet = -5;
    }


    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//超时信息
int CouponBLL::IsTimeOut(const  Json::Value  & pJson,
                         Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["orderNo"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);

    Coupon entity;
    iRet = mCouponOrderDAL.GetEntity(orderNo,entity);

    pResponse["result"]["auto"] = IsAutoTimeOut(entity);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//产品是否过期
int CouponBLL::ProductTimeOut(const  Json::Value  & pJson,
        Json::Value & pResponse)
{
    int iRet = -1;

    if(  pJson["idProduct"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long idProduct  =   tConvert.Json2Long( pJson["idProduct"]);

    ShowPro entity;
    iRet = mShowDAL.GetEntity(idProduct,entity);

    if( !entity.sExpireTime.empty() &&  entity.sExpireTime < tConvert.GetCurrentDay()){
        RetJsonMsg::SetRetMsg2(-1,pResponse,"商品已失效");
        return -1;
    }


    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}


//申请退款
int CouponBLL::ReqBack(const  Json::Value  & pJson,
                       Json::Value & pResponse)
{
    int iRet = -1;

    if( pJson["userId"].isNull() || pJson["code"].isNull() || pJson["orderNo"].isNull()
            || pJson["reason"].isNull() || pJson["password"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    Json::Value jCode = pJson["code"];//多个验证码
    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);
    string reason  =   tConvert.Json2String( pJson["reason"]);
    string password  =   tConvert.Json2String( pJson["password"]);

    if( jCode.size() <= 0){
        RetJsonMsg::SetRetMsg2(-21,pResponse);
        return -21;
    }


    Coupon entity;
    iRet = mCouponOrderDAL.GetEntity(orderNo,entity);

    if( entity.userId != userId){
        RetJsonMsg::SetRetMsg2(-21,pResponse);
        return -21;
    }

    string sCodes;
    for( int i = 0 ; i < jCode.size(); i++){
        sCodes += tConvert.Json2String(jCode[i]) + ",";

        Coupon c;
        mCouponOrderDAL.GetByCode(tConvert.Json2String(jCode[i]),-1,c);
        if( !(c.couponCode.state == couponcode_state_canuser
                || c.couponCode.state == couponcode_state_reqback)){//码必须可以使用or拒绝退款，才可以退款
            RetJsonMsg::SetRetMsg2(-21,pResponse);
            return -21;
        }
    }
    sCodes = sCodes.substr(0,sCodes.length() -1);

    Coupon  forUpdate;
    forUpdate.couponCode.reason = reason;
    forUpdate.couponCode.reqTime = tConvert.GetCurrentDayTime();

//    if( IsAutoTimeOut(entity)){

//        string sPhone;
//        GetUserInfo(userId,sPhone);
//        if( 0 != CheckPayPassWord(sPhone,password,pResponse)){
//            return -1;//返回钱包的错误
//        }
//        pResponse.clear();

//        forUpdate.couponCode.state = couponcode_state_backing;


//        iRet = mCouponCodeDAL.Update(orderNo,sCodes,forUpdate);


//    }else{

        //现在，只要消费费申请退款就马上退

        forUpdate.couponCode.state = couponcode_state_backed;
        forUpdate.couponCode.backTime = forUpdate.couponCode.reqTime;

        //事务处理
        //start
        mCouponCodeDAL.AutoCommit(false);

        string sSql  = mCouponCodeDAL.GetSqlUpdate(orderNo,sCodes,forUpdate);
        StringLst batchSql;
        batchSql.push_back(sSql);

        iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


        if( 0 == iRet ){
            if( 0 !=WalletBack(jCode,password,pResponse)){
                mCouponCodeDAL.RollBack();
                mCouponCodeDAL.AutoCommit(true);
                return -1;//返回钱包服务的错误
            }
            mCouponCodeDAL.Commit();
        }

        mCouponCodeDAL.AutoCommit(true);

        //事务处理
        //end


  //  }


    pResponse.clear();

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//cancel退款
int CouponBLL::CancelBack(const  Json::Value  & pJson,
                          Json::Value & pResponse)
{
    int iRet = -1;

    if( pJson["userId"].isNull() || pJson["orderNo"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long userId  =   tConvert.Json2Long(pJson["userId"]);
    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);


    Coupon entity;
    iRet = mCouponOrderDAL.GetEntity(orderNo,entity);

    if( entity.userId != userId){
        RetJsonMsg::SetRetMsg2(-21,pResponse);
        return -21;
    }

    iRet = mCouponCodeDAL.CancleBack(orderNo);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//add commtent
int CouponBLL::WriteCommtent( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    TConvert  tConvert;
    int iRet = -1;
    if( pJson["score"].isNull() || pJson["idPlatform"].isNull() || pJson["idProduct"].isNull() || pJson["orderNo"].isNull() || pJson["userId"].isNull() ||
            pJson["content"].isNull() || pJson["headImage"].isNull() || pJson["nickName"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    CommentEntity entity;
    double dScore = tConvert.Json2Double( pJson["score"]);
    entity.iScore = dScore*10;
    entity.lAddTime = tConvert.GetCurrentTime();
    entity.lPlatformId = tConvert.Json2Long( pJson["idPlatform"]);
    entity.idProduct = tConvert.Json2Long( pJson["idProduct"]);
    entity.lUserId = tConvert.Json2Long( pJson["userId"]);
    entity.sContent = tConvert.Json2String( pJson["content"]);
    entity.sHeadImage = tConvert.Json2String( pJson["headImage"]);
    entity.sNickName = tConvert.Json2String( pJson["nickName"]);

    string orderNo = tConvert.Json2String( pJson["orderNo"]);

    if( !pJson["pics"].isNull() && pJson["pics"].isArray()){

        entity.sPics =  pJson["pics"].toStyledString();
    }

    iRet = mCommentDAL.Add(entity);

    mCouponCodeDAL.StateToState(orderNo,couponcode_state_tomark,couponcode_state_bemarked);

    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}


//验证使用验证码
int CouponBLL::Check( const  Json::Value  & pJson ,Json::Value & pResponse )
{

    int iRet = -1;

    if( pJson["idPlatform"].isNull() || pJson["code"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long idPlatform = tConvert.Json2Long( pJson["idPlatform"] );
    string code =  tConvert.Json2String( pJson["code"] );


    Coupon  entity;
    iRet = mCouponOrderDAL.GetByCode(code,-1,entity);


    if( (iRet != 0) || (entity.idPlatform  !=  idPlatform) ){
        RetJsonMsg::SetRetMsg2(-303,pResponse);
        return -303;
    }

    if( entity.couponCode.state == couponcode_state_timeout){
        RetJsonMsg::SetRetMsg2(-301,pResponse);
        return -301;
    }


    if( entity.couponCode.state == couponcode_state_tomark || entity.couponCode.state ==couponcode_state_bemarked){
        RetJsonMsg::SetRetMsg2(-302,pResponse);
        return -302;
    }

    if( entity.couponCode.state == couponcode_state_backing  ){
        RetJsonMsg::SetRetMsg2(-1,pResponse,"验证码正在退款中，不能使用");
        return -1;
    }

    if( entity.couponCode.state == couponcode_state_backed  ){
        RetJsonMsg::SetRetMsg2(-1,pResponse,"验证码已退款，不能使用");
        return -1;
    }




    //事务处理
    //start
    mCouponCodeDAL.AutoCommit(false);
    Coupon  forUpdate;
    forUpdate.couponCode.state = couponcode_state_tomark;
    forUpdate.couponCode.consumTime = tConvert.GetCurrentDayTime();
    string sSql = mCouponCodeDAL.GetSqlUpdate(entity.orderNo,code,forUpdate);
    StringLst batchSql;
    batchSql.push_back(sSql);

    iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


    if( 0 == iRet ){
        if(  0 != WalletSettlement(code,pResponse)){
            mCouponCodeDAL.RollBack();
            mCouponCodeDAL.AutoCommit(true);
            return -1;//返回钱包服务的错误
        }
    }
    mCouponCodeDAL.Commit();
    mCouponCodeDAL.AutoCommit(true);
    //事务处理
    //end

    SendCheckNotice(entity.userId,entity.name,code,entity.price*((100 - entity.iOffLineDisCount) * 10 / 2)/100,entity.iCoinType);

    pResponse.clear();
    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;

}

//同意退款
int CouponBLL::Agree( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;
    if(  pJson["orderNo"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);


    CouponLst lst;string sCodes;
    Json::Value jCode;

    mCouponOrderDAL.GetReqBacking(orderNo,lst);
    for(int i = 0; i < lst.size() ; i++){
        Coupon  & entity = lst[i];
        sCodes += entity.couponCode.code + ",";
        jCode[i] = entity.couponCode.code;



    }
    sCodes = sCodes.substr(0,sCodes.length() -1);


    //事务处理
    //start
    mCouponCodeDAL.AutoCommit(false);
    Coupon  forUpdate;
    forUpdate.couponCode.state = couponcode_state_backed;
    forUpdate.couponCode.backTime = tConvert.GetCurrentDayTime();
    string sSql = mCouponCodeDAL.GetSqlUpdate(orderNo,sCodes,forUpdate);
    StringLst batchSql;
    batchSql.push_back(sSql);
    iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


    if( 0 == iRet ){
        if( 0 !=WalletBack(jCode,"",pResponse)){
            mCouponCodeDAL.RollBack();
            mCouponCodeDAL.AutoCommit(true);
            return -1;
        }
    }
    mCouponCodeDAL.Commit();
    mCouponCodeDAL.AutoCommit(true);
    //事务处理
    //end



    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;


}


//拒绝退款
int CouponBLL::RejBack( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    if(  pJson["orderNo"].isNull() || pJson["rejReason"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    string orderNo  =   tConvert.Json2String( pJson["orderNo"]);
    string rejReason  =   tConvert.Json2String( pJson["rejReason"]);


    CouponLst lst;string sCodes;

    mCouponOrderDAL.GetReqBacking(orderNo,lst);
    for(int i = 0; i < lst.size() ; i++){
        Coupon  & entity = lst[i];
        sCodes += entity.couponCode.code + ",";

    }
    sCodes = sCodes.substr(0,sCodes.length() -1);

    mCouponCodeDAL.UpdateRejReason(orderNo,sCodes,rejReason);
    mCouponCodeDAL.UpdateState(orderNo,sCodes,couponcode_state_reqback);

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}


//获取一个平台下的券
int CouponBLL::GetCouponByPlatfrom( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;

    if( pJson["idPlatform"].isNull() || pJson["page"].isNull() || pJson["count"].isNull()
            || pJson["state"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }

    long idPlatform  =   tConvert.Json2Long(pJson["idPlatform"]);
    int page  =   tConvert.Json2Int(pJson["page"]);
    int count  =   tConvert.Json2Int(pJson["count"]);
    string state  =   tConvert.Json2String( pJson["state"]);


    CouponLst  lst;

    iRet = mCouponOrderDAL.GetCouponByPlatform( idPlatform,state,page,count,lst);

    int iCount = 0;
    while( iCount < lst.size()){
        Coupon  & entity = lst[iCount];
        pResponse["result"][iCount]["orderNo"] = entity.orderNo;
        pResponse["result"][iCount]["payTime"] =  entity.payTime;
        pResponse["result"][iCount]["idPlatform"] = tConvert.LongToStr( entity.idPlatform) ;
        pResponse["result"][iCount]["idProduct"] = tConvert.LongToStr(entity.idProduct);
        pResponse["result"][iCount]["expireTime"] = entity.expireTime;
        pResponse["result"][iCount]["shopName"] =  entity.shopName;
        pResponse["result"][iCount]["shopType"] =  GetShopType( entity.shopType );


        pResponse["result"][iCount]["content"] =  entity.content;
        pResponse["result"][iCount]["name"] =  entity.name;
        pResponse["result"][iCount]["picLink"] =  entity.picLink;
        pResponse["result"][iCount]["state"] =  entity.couponCode.state;

        pResponse["result"][iCount]["backTime"] =  entity.couponCode.backTime;
        pResponse["result"][iCount]["consumTime"] =  entity.couponCode.consumTime;
        pResponse["result"][iCount]["reqTime"] =  entity.couponCode.reqTime;
        pResponse["result"][iCount]["reason"] =  entity.couponCode.reason;
        pResponse["result"][iCount]["rejReason"] =  entity.couponCode.rejReason;
        pResponse["result"][iCount]["specification"] =  entity.specification;
        string sPhone;
        GetUserInfo(entity.userId,sPhone);

        pResponse["result"][iCount]["telPhone"] =  sPhone;

        iCount++;
    }
    if( lst.size() <= 0){
        pResponse["result"] ;
        pResponse["result"].resize(0);
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//首页商品轮播
int CouponBLL::Loop( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;

    if( pJson["town"].isNull() || pJson["area"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    string town  =   tConvert.Json2String( pJson["town"]);
    string area  =   tConvert.Json2String( pJson["area"]);

    if( area == "龙华区"){
        area = "龙华新区";
    }

    ShowProLst shows;
    mShowDAL.GetLoop(town,area,shows);

    Json::Value & jProducts = pResponse["result"]["products"];
    jProducts.resize(0);
    int iCount = 0;
    while( iCount < shows.size()){
        ShowPro  & entity = shows[iCount];
        jProducts[iCount]["name"] = entity.sName;
        jProducts[iCount]["idPlatform"] = tConvert.LongToStr( entity.lPlatformId) ;
        jProducts[iCount]["idProduct"] = tConvert.LongToStr(entity.id);
        jProducts[iCount]["picLink"] = entity.sPicLink;
        jProducts[iCount]["price"] = entity.dPrice;
        jProducts[iCount]["coin"] = entity.dCoin;
        jProducts[iCount]["coinType"] = entity.iCoinType;

        iCount++;

        if( iCount == 9){
            break;
        }
    }
    bool bShow = false;
    bool bBtn = false;
    if( shows.size() <= 0){
        pResponse["result"]["products"] ;
        pResponse["result"]["products"].resize(0);
    }
    if( shows.size() > 9){
        bShow = true;
        bBtn = true;
    }else if( shows.size() <=3){
        bShow = false;
        bBtn = false;
    }else{
        bShow = true;
        bBtn = false;
    }
    pResponse["result"]["show"] = bShow;
    pResponse["result"]["btn"] = bBtn;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//商品分类数量
int CouponBLL::TypeInfo( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;

    if( pJson["town"].isNull() || pJson["area"].isNull() ){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    string town  =   tConvert.Json2String( pJson["town"]);
    string area  =   tConvert.Json2String( pJson["area"]);

    if( area == "龙华区"){
        area = "龙华新区";
    }

    StringLongStdMap typeinfo;
    mShowDAL.TypeInfo(town,area,typeinfo);
    AppendShopType(typeinfo);
    Json::Value & jtype = pResponse["result"]["type"];
    jtype.resize(0);

    int iOtherNum = 0;
    int iTotal = 0;
    for( StringLongStdMap::iterator it = typeinfo.begin(); it != typeinfo.end(); it++){
        iTotal += it->second;
        if( it->first == "其他"){
            iOtherNum =it->second;
        }
    }

    int iCount = 0;
    jtype[iCount]["name"] = "全部";
    jtype[iCount]["num"] = int( iTotal );
    iCount++;
    for( StringLongStdMap::iterator it = typeinfo.begin(); it != typeinfo.end(); it++){
        if( it->first == "其他"){
            continue;
        }
        jtype[iCount]["name"] = it->first;
        jtype[iCount]["num"] = int( it->second );
        iCount++;
    }
    jtype[iCount]["name"] = "其他";
    jtype[iCount]["num"] = int( iOtherNum );
    iCount++;

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}

//商品分区分类分页
int CouponBLL::GetPageByType( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;

    if( pJson["town"].isNull() || pJson["area"].isNull() || pJson["area"].isNull()
            || pJson["gpsLat"].isNull() || pJson["gpsLon"].isNull() || pJson["page"].isNull()){
        RetJsonMsg::SetRetMsg2(-15,pResponse);
        return -15;
    }
    string town  =   tConvert.Json2String( pJson["town"]);
    string area  =   tConvert.Json2String( pJson["area"]);

    if( area == "龙华区"){
        area = "龙华新区";
    }

    string type;
    if( !pJson["type"].isNull()){
        type  =   tConvert.Json2String( pJson["type"]);
        if( type == "全部")
            type.clear();
    }
    int sort = -1;
    if( !pJson["sort"].isNull()){
        sort  =   tConvert.Json2Int( pJson["sort"]);
    }
    double gpsLat  =   tConvert.Json2Double( pJson["gpsLat"]);
    double gpsLon  =   tConvert.Json2Double( pJson["gpsLon"]);
    int page  =   tConvert.Json2Int( pJson["page"]);
    ShowProLst shows;
    mShowDAL.GetPageByType(town,area,gpsLat,gpsLon,page,type,sort,shows);
    Json::Value & jProducts = pResponse["result"]["products"];
    jProducts.resize(0);
    int iCount = 0;
    while( iCount < shows.size()){
        ShowPro  & entity = shows[iCount];
        jProducts[iCount]["name"] = entity.sName;
        jProducts[iCount]["idPlatform"] = tConvert.LongToStr( entity.lPlatformId) ;
        jProducts[iCount]["idProduct"] = tConvert.LongToStr(entity.id);
        jProducts[iCount]["picLink"] = entity.sPicLink;
        jProducts[iCount]["price"] = entity.dPrice;
        jProducts[iCount]["coin"] = entity.dCoin;
        jProducts[iCount]["coinType"] = entity.iCoinType;

        iCount++;
    }

    RetJsonMsg::SetRetMsg2(0,pResponse);
    return 0;

}


//获取产品详情
int CouponBLL::ProductDetail( const  Json::Value  & pJson ,Json::Value & pResponse )
{
    int iRet = -1;
    if( pJson["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    long idProduct = tConvert.Json2Long( pJson["id"]);

    ShowPro entity;
    iRet =  mShowDAL.GetEntity(idProduct,entity);


    ShopInfoEntity shopInfo;
    ShopInfoDAL::GetInstance()->GetEntity(tConvert.LongToStr(entity.lPlatformId),1,shopInfo);


    int iSellCount = 0;
    mCouponOrderDAL.GetSellCount(idProduct,iSellCount);

    pResponse["result"]["coin"] = entity.dCoin;
    pResponse["result"]["price"] = entity.dPrice;
    pResponse["result"]["id"] = tConvert.LongToStr( entity.id );
    pResponse["result"]["canot"] = entity.sCanot;
    pResponse["result"]["content"] = entity.sContent;
    pResponse["result"]["name"] = entity.sName;

    if( entity.sDetailPics.empty()){
        entity.sDetailPics = "[]";
    }
    Json::Value jDetailPics;
    tConvert.Str2Json( entity.sDetailPics , jDetailPics);

    pResponse["result"]["detailPics"] = jDetailPics;

    pResponse["result"]["expireTime"] = entity.sExpireTime;

    if( entity.sLaw.empty()){
        entity.sLaw = "[]";
    }
    Json::Value jLaw;
    tConvert.Str2Json(entity.sLaw,jLaw);
    pResponse["result"]["law"] = jLaw;

    pResponse["result"]["name"] = entity.sName;
    pResponse["result"]["picLink"] = entity.sPicLink;
    pResponse["result"]["utime"] = entity.sUtime;

    pResponse["result"]["address"] = shopInfo.contentEntity.sAddress;
    pResponse["result"]["shopName"] = shopInfo.contentEntity.sShopName;
    pResponse["result"]["shopPhone"] = shopInfo.contentEntity.sSevvicePhone;
    pResponse["result"]["gpsLat"] = shopInfo.contentEntity.dGpsLat;
    pResponse["result"]["gpsLon"] = shopInfo.contentEntity.dGpsLon;
    pResponse["result"]["idPlatform"] = tConvert.LongToStr( entity.lPlatformId );

    pResponse["result"]["sellCount"] = iSellCount;

    pResponse["result"]["coinType"] =  entity.iCoinType;

    pResponse["result"]["specification"] =  entity.sSpecification;



    RetJsonMsg::SetRetMsg2(iRet,pResponse);
    return iRet;


}

bool CouponBLL::IsAutoTimeOut(const Coupon & entity)
{
    long lCurTime = tConvert.GetCurrentTime()/1000000;
    long lPayTime = tConvert.StrToTime( entity.payTime.c_str());

    bool bTimeOut = false;
    if( lCurTime - lPayTime > 60*30){//30分钟内可以自动退款
        bTimeOut = true;
    }
    return bTimeOut;

}

//检验支付密码,调用桌斌
int CouponBLL::CheckPayPassWord( string sPhone , string sPassWord,Json::Value & jResponse)
{

    string sUrl = "http://"  + TBLL::GetInstance()->GetWalletIP() +"/GetDataInterface/FansTrade/VerifyPayPassWord.aspx";

    Json::Value jPost;
    jPost["sPayPassWord"] = sPassWord;
    jPost["sPhone"] = sPhone;

    int iRet = Request::WalletHttpPost(sUrl,jPost,jResponse);

    Replace(jResponse);

    return iRet;
}

int CouponBLL::WalletOrder(const StringLst & sOrderNos,double dPrice,double dCoin,int iPayType,long lPlatformId,const string & sPayerPhone, Json::Value & jRespons)
{

    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/BatchCreateOrder.aspx";

    Json::Value jPost;

    for( int i = 0; i < sOrderNos.size(); i++){
        jPost["listRecOrderInfo"][i]["dMoney"] = dPrice;
        jPost["listRecOrderInfo"][i]["dNotDividedMoney"] = 0.0;
        jPost["listRecOrderInfo"][i]["iPayType"] = iPayType;
        jPost["listRecOrderInfo"][i]["lPlatformId"] =tConvert.LongToStr(lPlatformId);
        jPost["listRecOrderInfo"][i]["sPayerPhone"] = sPayerPhone;
        jPost["listRecOrderInfo"][i]["sRemark"] = "商品券支付";
        jPost["listRecOrderInfo"][i]["dShopCoin"] = dCoin;
        jPost["listRecOrderInfo"][i]["iRecType"] = 0;
        jPost["listRecOrderInfo"][i]["sOrderNo"] = sOrderNos[i];
    }

    int iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);

    string s = jRespons.toStyledString();

    Replace(jRespons);

    return iRet;


}


int CouponBLL::WalletPay(string sPayOrderNo,string sPassWord,int iCoinType,Json::Value & jRespons)
{
    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/PayByPayOrderNo.aspx";
    Json::Value jPost;

    jPost["sPayOrderNo"] =sPayOrderNo ;
    jPost["sPayPassWord"] =sPassWord ;
    jPost["iShopCoinType"] = iCoinType;
    int iRet = Request::WalletHttpPost(sUrl, jPost,jRespons);
    string s = jRespons.toStyledString();
    Replace(jRespons);
    return iRet ;

}


//结算
int CouponBLL::WalletSettlement(string sOrderNo,Json::Value & jRespons)
{
    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/Settlement.aspx";
    Json::Value jPost;

    jPost["sOrderNo"] =sOrderNo ;
    int iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);
    Replace(jRespons);
    return iRet;

}

//退款
int CouponBLL::WalletBack(const  Json::Value & jCode,string sPassWord,Json::Value & jRespons)
{
    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/BatchRefund.aspx";
    Json::Value jPost;

    jPost["listRefundInfo"].resize(0) ;
    for(int i = 0 ; i < jCode.size(); i++){
        jPost["listRefundInfo"][i]["sOrderNo"] = jCode[i];
    }
    if( !sPassWord.empty()){
        jPost["sPayPassWord"] =sPassWord ;
    }//sPayPassWord

    int iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);
    Replace(jRespons);;
    return iRet;

}

//取用户信息
int CouponBLL::GetUserInfo(long lUserId,string & sPhone)
{

    TConvert tConvert;
    Json::Value jsonValue;
    Json::Value jsonReturn;
    jsonValue["lUserId"]= tConvert.LongToStr( lUserId );
    string sUrl = "http://" + TBLL::GetInstance()->GetMechatIP() + ":23241/?sBusiness=UserQuery&sMethod=GetUserInfo";

    int iRet = Request::CppHttpPost(sUrl,jsonValue,jsonReturn);

    sPhone = tConvert.Json2String(jsonReturn["entity"]["sAccount"]);

    return iRet;

}


void CouponBLL::Replace(Json::Value & jsonRespons)
{
    TConvert tConvert;
    string err =  tConvert.Json2String( jsonRespons["err"]);
    if( err.empty()){
        err = "服务器异常";
    }
    jsonRespons.removeMember("err");
    jsonRespons["msg"] = err;
    if(jsonRespons["iRet"].isNull() ){
        jsonRespons["state"] = -1000000;
    }else{
        jsonRespons["state"] = jsonRespons["iRet"];
    }
    jsonRespons.removeMember("iRet");
}

int CouponBLL::GetShopType(const string & sShopType)
{
    if( sShopType == "电影"){
        return 2;
    }
    if( sShopType == "酒店"){
        return 3;
    }
    if( sShopType == "KTV"){
        return 4;
    }
    if( sShopType == "美容美发"){
        return 5;
    }
    if( sShopType == "美食"){
        return 6;
    }
    if( sShopType == "其他"){
        return 7;
    }
    if( sShopType == "生活服务"){
        return 8;
    }
    if( sShopType == "健康养生"){
        return 9;
    }
    return 0;

}

void CouponBLL::AppendShopType(StringLongStdMap & typenum)
{

    NearCalssLst nearclass;
    mClassDAL.GetAllCalss(nearclass);
    NearCalssLst::iterator it = nearclass.begin();
    for( ; it != nearclass.end(); it++){
        NearCalss & c = *it;
        if( typenum.find(c.sName) == typenum.end()){

           // typenum.insert (std::make_pair<std::string,long>(c.sName.c_str(),0));
            if( c.sName == "中国人寿"){
                continue;
            }
            typenum[c.sName] = 0;
        }

    }

}


//发送验证成功通知,by Im
bool CouponBLL::SendCheckNotice( long lUserId, const string & sConntent, const string & sCode, int iCoin, int iCoinType)
{
    //参数转换
    TConvert tConvert;
    Json::Value jsonValue,jsonContent,jRespons;
    jsonContent["sTitle"] = "商品券验证成功";
    jsonContent["iSubType"] = 11;//
    jsonContent["商品详情"] = sConntent;//
    jsonContent["商品券验证码"] = sCode;//

    string sCoinType = "购物卡";
    if( iCoinType == enum_cointype_private ){
        sCoinType = "专属购物卡";
    }
    jsonContent["赠送"] =   tConvert.IntToStr(iCoin) + sCoinType;//



    jsonValue["lSendId"]= tConvert.LongToStr(28);
    jsonValue["iTarget"]= 2;//2 组播或单播
    jsonValue["sContent"]=jsonContent;
    jsonValue["sRevIds"][0]= tConvert.LongToStr( lUserId );
    jsonValue["iType"]=2;//2 消息



    string sUrl = "http://"+ TBLL::GetInstance()->GetMechatIP() +":23241/?sBusiness=Notice&sMethod=Send";

    Request::CppHttpPost(sUrl,jsonValue,jRespons);

    return true;

}

