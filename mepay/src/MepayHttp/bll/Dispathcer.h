#ifndef DISPATHCER_H
#define DISPATHCER_H

#include "dal/ShopInfoDAL.h"
#include "dal/CouponDAL.h"
#include "dal/CouponCodeDAL.h"
///联盟商家调度
///
///
class Dispathcer
{
public:
    Dispathcer();
    void Run();
private:
    static void* GpsToZoneThread(void*ptr);
    //优惠券调度线程，处理过期订单，退款申请等等
    static void* CouponThread(void*ptr);
    void RealSGpsToZone();

   int GpsToZone(double dLat, double dLon, string & sZone);

   //处理过期订单，退款申请等等
   void RealCouponThread();


   //处理:申请退款已超过3天的验证码,系统自动退款
   void Backing3DayCoupon();
   //处理:已过期未退款验证码,系统自动结算给商家
   void TimeOutCoupon();
   //处理:已过期未退款验证码,系统自动退款
   void TimeOutCouponBack();

private:
   string  msUrl = "http://apis.map.qq.com/ws/geocoder/v1/?";
   TConvert mtConvert;

   CouponOrderDAL mCouponOrderDAL;
   CouponCodeDAL mCouponCodeDAL;
};
#endif
