#include "Dispathcer.h"
#include "com/TCurl.h"
#include "bll/TBLL.h"

//联盟商家调度


Dispathcer::Dispathcer()
{

}

void Dispathcer::Run()
{
    if( "true" != TBLL::GetInstance()->GetOpendiaoDu()){//是否打开调度服务
        return;
    }

    pthread_t tid1;
    pthread_create( &tid1, NULL, GpsToZoneThread, (void*)this );

    pthread_t tid2;
    pthread_create( &tid2, NULL, CouponThread, (void*)this );


}

//
void* Dispathcer::GpsToZoneThread(void*ptr)
{
    pthread_detach(pthread_self());

    Dispathcer * p = (Dispathcer*)(ptr);
   // p->RealSGpsToZone();

    pthread_exit(NULL);
}

//优惠券调度线程，处理过期订单，退款申请等等
void* Dispathcer::CouponThread(void*ptr)
{
    pthread_detach(pthread_self());

    Dispathcer * p = (Dispathcer*)(ptr);
    p->RealCouponThread();

    pthread_exit(NULL);
}


//处理过期订单，退款申请等等
void Dispathcer::RealCouponThread()
{
    while(true){
        sleep(10);

        mCouponOrderDAL.SetTimeOutState();

        //Backing3DayCoupon();
        //TimeOutCoupon();
        TimeOutCouponBack();




    }

}


//处理:申请退款已超过3天的验证码,系统自动退款
void Dispathcer::Backing3DayCoupon()
{

    int iRet = 0;

    CouponLst reqBackCodes;
    mCouponOrderDAL.GetReqBacking3DayCoupon(1,1,reqBackCodes);//一条一条来

    if( reqBackCodes.size() == 0)
        return;

    string sCodes;


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/BatchRefund.aspx";
    Json::Value jPost,jRespons;

    jPost["listRefundInfo"].resize(0) ;
    for(int i = 0 ; i < reqBackCodes.size(); i++){
        Coupon & couponcode = reqBackCodes[i];
        jPost["listRefundInfo"][i]["sOrderNo"] = couponcode.couponCode.code;
        sCodes += couponcode.couponCode.code + ",";
    }
    if( !sCodes.empty()){
        sCodes = sCodes.substr(0,sCodes.length() -1);
    }


    //事务处理
    //start
    mCouponCodeDAL.AutoCommit(false);

    Coupon forUpdate;
    forUpdate.couponCode.state = couponcode_state_backed;
    forUpdate.couponCode.reason = "申请超过3天系统自动退款";
    forUpdate.couponCode.backTime = mtConvert.GetCurrentDayTime();
    string sSql = mCouponCodeDAL.GetSqlUpdate(sCodes,forUpdate);
    StringLst batchSql;
    batchSql.push_back(sSql);

    iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


    if( iRet == 0){
        int iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);
        if( iRet  == 0){
            mCouponCodeDAL.Commit();

        }else{
            mCouponCodeDAL.RollBack();

            Coupon forUpdate;
            forUpdate.couponCode.mark = 2;//退款失败
            mCouponCodeDAL.Update(sCodes,forUpdate);
        }
    }
    mCouponCodeDAL.AutoCommit(true);
    //事务处理
    //end

}


//处理:已过期未退款验证码,系统自动结算给商家
void Dispathcer::TimeOutCoupon()
{

    int iRet = 0;

    Coupon coupon;
    if( 0 != mCouponOrderDAL.GetTimeOutCoupon(coupon))//获取已过期未退款验证码
        return ;

    string sCodes;


    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/Settlement.aspx";
    Json::Value jPost,jRespons;

    jPost["sOrderNo"] =coupon.couponCode.code ;
    sCodes = coupon.couponCode.code ;


    //事务处理
    //start
    mCouponCodeDAL.AutoCommit(false);

    Coupon forUpdate;
    forUpdate.couponCode.mark = 1;//已结算给商家
    string sSql = mCouponCodeDAL.GetSqlUpdate(sCodes,forUpdate);
    StringLst batchSql;
    batchSql.push_back(sSql);

    iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


    if( iRet == 0){
        iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);
        if( iRet  == 0){
            mCouponCodeDAL.Commit();

        }else{
            mCouponCodeDAL.RollBack();

            Coupon forUpdate;
            forUpdate.couponCode.mark = 2;//结算失败
            mCouponCodeDAL.Update(sCodes,forUpdate);
        }
    }
    mCouponCodeDAL.AutoCommit(true);
    //事务处理
    //end

}

//处理:已过期未退款验证码,系统自动退款
void Dispathcer::TimeOutCouponBack()
{
    int iRet = 0;

    Coupon coupon;
    if( 0 != mCouponOrderDAL.GetTimeOutCoupon(coupon))//获取已过期未退款验证码
        return ;

    string sCodes;

    string sUrl = "http://" + TBLL::GetInstance()->GetWalletIP() + "/GetDataInterface/Consume/BatchRefund.aspx";
    Json::Value jPost,jRespons;


    jPost["listRefundInfo"].resize(0) ;
    jPost["listRefundInfo"][0]["sOrderNo"] = coupon.couponCode.code;
    sCodes = coupon.couponCode.code;

    //事务处理
    //start
    mCouponCodeDAL.AutoCommit(false);

    Coupon forUpdate;
    forUpdate.couponCode.state = couponcode_state_backed;
    forUpdate.couponCode.reason = "过期自动退款";
    forUpdate.couponCode.backTime = mtConvert.GetCurrentDayTime();
    string sSql = mCouponCodeDAL.GetSqlUpdate(sCodes,forUpdate);
    StringLst batchSql;
    batchSql.push_back(sSql);

    iRet = mCouponCodeDAL.BatchBaseQuery(batchSql);


    if( iRet == 0){
        int iRet =  Request::WalletHttpPost(sUrl, jPost,jRespons);
        if( iRet  == 0){
            mCouponCodeDAL.Commit();

        }else{
            mCouponCodeDAL.RollBack();

            Coupon forUpdate;
            forUpdate.couponCode.mark = 2;//退款失败
            mCouponCodeDAL.Update(sCodes,forUpdate);
        }
    }
    mCouponCodeDAL.AutoCommit(true);
    //事务处理
    //end

}

//
void Dispathcer::RealSGpsToZone()
{


    while(true){
        ShopInfoList lst;
        ShopInfoDAL::GetInstance()->GetEmptyGpsZone(lst);
        for( int i = 0; i < lst.size(); i++){
            ShopInfoEntity & entity = lst[i];
            string sZone;
            GpsToZone(entity.contentEntity.dGpsLat,entity.contentEntity.dGpsLon,sZone);
            if(  sZone.empty()){

                sZone = "delay";
            }
            ShopInfoDAL::GetInstance()->UpdateGpsZone(entity.sPrimaryKey,sZone);
            usleep(1000*200);
        }
        usleep(1000*10);
    }

}

//http://apis.map.qq.com/ws/geocoder/v1/?location=22.720457,114.050446&get_poi=0&key=5V2BZ-S5F36-3CPSF-M7WO4-IKR4V-VXBVD

int Dispathcer::GpsToZone(double dLat, double dLon, string & sZone)
{
    string sUrl = msUrl;
    sUrl += "location=" + mtConvert.DoubleToStr(dLat) + "," + mtConvert.DoubleToStr(dLon)+
            "&get_poi=0&key=5V2BZ-S5F36-3CPSF-M7WO4-IKR4V-VXBVD";


    string sReturn;Json::Value jsonRespons;
    TCurl tCurl;

    int iRet =tCurl.Get(sUrl,sReturn);


    if( 0 == iRet ){

        Json::Reader reader;
        if (!reader.parse(sReturn, jsonRespons)){
            return -1;
        }
        iRet = mtConvert.Json2Int( jsonRespons["status"] );

        if( iRet == 0){
            string sCity =  mtConvert.Json2String(jsonRespons["result"]["address_component"]["city"]);
            string sDistrict =  mtConvert.Json2String(jsonRespons["result"]["address_component"]["district"]);
            sZone = sCity + sDistrict;

        }
    }
    return iRet ;
}

