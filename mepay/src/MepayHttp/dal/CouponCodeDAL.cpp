#include "CouponCodeDAL.h"
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"

string CouponCodeDAL::GetSqlAdd( const Coupon& entity  )
{

    string sSql   = "insert into " +  msTableName +"(" + msAllColumn + ")" +
            "VALUES( '"+entity.orderNo+"','" +
            mtConvert.IntToStr(entity.couponCode.state)+"','" +
            entity.couponCode.backTime+"','" +
            entity.couponCode.consumTime+"','" +
            entity.couponCode.reqTime+"','" +
            entity.couponCode.code+"','" +
            entity.couponCode.reason+"','" +
            entity.couponCode.rejReason+
            "')";
    return sSql;
}

int CouponCodeDAL::Add( const Coupon& entity  )
{

    string sSql   = GetSqlAdd(entity);

    return BaseQuery(sSql);
}

//state 一至多个验证码
int CouponCodeDAL::UpdateState(const string & sOrder, const string & sCodes, int state)
{

    string sSql = "update "+ msTableName+" set state=" + mtConvert.IntToStr(state) +
            " where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and code in(" + mMulti.RealEscapeString(sCodes) + ")";

    return BaseQuery(sSql);
}


//update 一至多个验证码
string CouponCodeDAL::GetSqlUpdate(const string & sOrder, const string & sCodes, const Coupon& forUpdate)
{
    string sSql = "update "+ msTableName+" set orderNo=orderNo ";

    if(! forUpdate.couponCode.reason.empty()){
        sSql += ",reason='" +  mMulti.RealEscapeString(forUpdate.couponCode.reason) + "'";
    }
    if( forUpdate.couponCode.state >=0){
        sSql  += ",state=" +  mtConvert.IntToStr(forUpdate.couponCode.state);
    }
    if(! forUpdate.couponCode.reqTime.empty()){
        sSql += ",reqTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.reqTime) + "'";
    }
    if(! forUpdate.couponCode.consumTime.empty()){
        sSql += ",consumTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.consumTime) + "'";
    }
    if(! forUpdate.couponCode.backTime.empty()){
        sSql += ",backTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.backTime) + "'";
    }

    sSql += " where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and code in(" + mMulti.RealEscapeString(sCodes) + ")";
    return sSql;

}

//update 一至多个验证码
string CouponCodeDAL::GetSqlUpdate( const string & sCodes, const Coupon& forUpdate)
{
    string sSql = "update "+ msTableName+" set orderNo=orderNo ";

    if(! forUpdate.couponCode.reason.empty()){
        sSql += ",reason='" +  mMulti.RealEscapeString(forUpdate.couponCode.reason) + "'";
    }
    if( forUpdate.couponCode.state >=0){
        sSql  += ",state=" +  mtConvert.IntToStr(forUpdate.couponCode.state);
    }
    if(! forUpdate.couponCode.reqTime.empty()){
        sSql += ",reqTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.reqTime) + "'";
    }
    if(! forUpdate.couponCode.consumTime.empty()){
        sSql += ",consumTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.consumTime) + "'";
    }
    if(! forUpdate.couponCode.backTime.empty()){
        sSql += ",backTime='" +  mMulti.RealEscapeString(forUpdate.couponCode.backTime) + "'";
    }
    if(forUpdate.couponCode.mark >=0){
        sSql += ",mark=" +  mtConvert.IntToStr(forUpdate.couponCode.mark);
    }

    sSql += " where  code in(" + mMulti.RealEscapeString(sCodes) + ")";
    return sSql;

}

//update 一至多个验证码
int CouponCodeDAL::Update( const string & sCodes, const Coupon& forUpdate)
{

    string sSql = GetSqlUpdate(sCodes,forUpdate);

    return BaseQuery(sSql);
}

//update 一至多个验证码
int CouponCodeDAL::Update(const string & sOrder, const string & sCodes, const Coupon& forUpdate)
{

    string sSql = GetSqlUpdate(sOrder,sCodes,forUpdate);

    return BaseQuery(sSql);
}

//撤销退款
int CouponCodeDAL::CancleBack(const string & sOrder)
{

    string sSql = "update "+ msTableName+" set state=1"  +
            " where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and state=3";

    return BaseQuery(sSql);
}

//更新退款原因，一至多个验证码
int CouponCodeDAL::UpdateReason(const string & sOrder, const string & sCodes, const string & sReason)
{

    string sSql = "update "+ msTableName+" set reason='" +  mMulti.RealEscapeString(sReason) +
            "' where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and code in(" + mMulti.RealEscapeString(sCodes) + ")";

    return BaseQuery(sSql);
}

//更新拒绝退款原因，一至多个验证码
int CouponCodeDAL::UpdateRejReason(const string & sOrder, const string & sCodes, const string & sReason)
{

    string sSql = "update "+ msTableName+" set rejReason='" +  mMulti.RealEscapeString(sReason) +
            "' where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and code in(" + mMulti.RealEscapeString(sCodes) + ")";

    return BaseQuery(sSql);
}

//将一张订单下符合iOrgState状态的验证码转成iDestState状态
int CouponCodeDAL::StateToState(const string & sOrder, int iOrgState, int iDestState)
{

    string sSql = "update "+ msTableName+" set state='" +  mtConvert.IntToStr(iDestState) +
            "' where orderNo='" +  mMulti.RealEscapeString(sOrder) + "' and state=" + mtConvert.IntToStr(iOrgState);

    return BaseQuery(sSql);
}

