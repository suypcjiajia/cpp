#ifndef COUPONCODE_DAL_H
#define COUPONCODE_DAL_H


#include <string>
#include <vector>
#include <deque>
#include <hash_map>
#include "TMutex.hpp"
#include "dal/TMysqlDAL.h"
#include "bll/ZoneAgnetBLL.h"
#include "dal/Basedal.h"
using namespace std;


//创建优惠券验证码
class CouponCodeDAL:public BaseDal
{

public:

    CouponCodeDAL(){
        msTableName = "coupon_code";
        msAllColumn = " orderNo,state,backTime,consumTime,reqTime,code,reason,rejReason ";
    }

public:

    string GetSqlAdd( const Coupon& entity  );

    //增加
    int Add( const Coupon& entity );

    //state 一至多个验证码
    int UpdateState(const string & sOrder, const string & sCodes, int state);

    //更新退款原因，一至多个验证码
    int UpdateReason(const string & sOrder, const string & sCodes, const string & sReason);

    //update 一至多个验证码
    string GetSqlUpdate(const string & sOrder, const string & sCodes, const Coupon& forUpdate);
    //update 一至多个验证码
    string GetSqlUpdate( const string & sCodes, const Coupon& forUpdate);
    //update 一至多个验证码
    int Update(const string & sOrder, const string & sCodes, const Coupon& forUpdate);
    //update 一至多个验证码
    int Update( const string & sCodes, const Coupon& forUpdate);

    //更新拒绝退款原因，一至多个验证码
    int UpdateRejReason(const string & sOrder, const string & sCodes, const string & sReason);

    //撤销退款
    int CancleBack(const string & sOrder);

    //将一张订单下符合iOrgState状态的验证码转成iDestState状态
    int StateToState(const string & sOrder, int iOrgState, int iDestState);


private:
    TMutex mMutex;//线程锁


};

#endif // COUPONCODE_DAL_H
