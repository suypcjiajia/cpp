#ifndef COUPON_DAL_H
#define COUPON_DAL_H


#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "bll/ZoneAgnetBLL.h"
#include "dal/Basedal.h"
using namespace std;


//优惠券订单
class CouponOrderDAL:public BaseDal
{

public:

    CouponOrderDAL(){
        msTableName = "coupon_order";
        msAllColumn = " orderNo,userId,addTime,idPlatform,idProduct,expireTime,payTime,endTime,price,coin,num ";
    }

public:

    string GetSqlAdd( const Coupon& entity  );
    //增加
    int Add( const Coupon& entity );


    int GetMyCoupon(long lUserId,string sStates,int iPage ,int iCount,CouponLst & lst);

    int GetCouponByPlatform(long lPlatformId,string sStates, int iPage ,int iCount,CouponLst & lst);

    int Detail(string Order ,CouponLst & lst);

    int GetEntity(string Order,Coupon & entity);

    //获取一个订单下的退款中的验证码
    int GetReqBacking(string Order ,CouponLst & lst);

    int GetByCode(string sCode ,int state,Coupon & entity);

    //获取销量
    int GetSellCount(long idProduct,int & iSellCount);


    //把已过期的未使用的验证码状态改为已过期（过期时时间为空串时不处理）
    void SetTimeOutState();

    //获取申请退款已超过3天的验证码
    int GetReqBacking3DayCoupon(int iPage ,int iCount,CouponLst & lst);

    //获取已过期未退款验证码
    int GetTimeOutCoupon(Coupon & coupon);

private:

    void RowToEntity(MYSQL_ROW row,Coupon & entity);
    TMutex mMutex;//线程锁

    string mDetailSql = " a.orderNo,a.userId,a.addTime,a.idPlatform,a.idProduct,a.expireTime,a.payTime,a.endTime,a.price,a.coin,a.num "
                        ",b.state,b.backTime,b.consumTime,b.reqTime,b.code,b.reason,b.rejReason"
                        ",c.sShopName,c.sShopType,c.sSevvicePhone,c.sAddress,c.dGpsLat,c.dGpsLon,c.iOffLineDisCount,c.iCoinType"
                        ",d.sContent,d.sPicLink,d.sName,d.sSpecification ";


};

#endif // COUPON_DAL_H
