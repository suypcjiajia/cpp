#include "CouponDAL.h"
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

string CouponOrderDAL::GetSqlAdd( const Coupon& entity  )
{
    string sSql   = "insert into " +  msTableName +"(" + msAllColumn + ")" +
            "VALUES( '"+entity.orderNo+"','" +
            mtConvert.LongToStr(entity.userId)+"','" +
            mtConvert.LongToStr(entity.addTime)+"','" +
            mtConvert.LongToStr(entity.idPlatform)+"','" +
            mtConvert.LongToStr(entity.idProduct)+"','" +
            entity.expireTime+"','" +
            entity.payTime+"','" +
            entity.endTime+"','" +
            mtConvert.DoubleToStr(entity.price)+"','" +
            mtConvert.DoubleToStr(entity.coin)+"','" +
            mtConvert.IntToStr(entity.num)+
            "')";
    return sSql;

}

int CouponOrderDAL::Add( const Coupon& entity  )
{

    string sSql   = GetSqlAdd( entity  );

    return BaseQuery(sSql);
}


int CouponOrderDAL::GetMyCoupon(long lUserId,string sStates, int iPage ,int iCount,CouponLst & lst)
{
    int iRet = -1;
    MYSQL_RES* result;

    if( iPage <=0 )
        iPage = 1;
    int iIndex = (iPage-1)*iCount;
    string sSql = "select " + mDetailSql + " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey and a.idProduct=d.id and b.state in("  + sStates  +  ") and a.userId=" + mtConvert.LongToStr(lUserId);

    sSql += sCond;
    sSql += " group by b.orderNo,b.state order by a.addTime desc ";
    sSql += " limit " + mtConvert.IntToStr(iIndex) + "," + mtConvert.IntToStr(iCount);


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            Coupon entity;

            RowToEntity(row,entity);

            lst.push_back( entity );
        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}


//获取申请退款已超过3天的验证码
int CouponOrderDAL::GetReqBacking3DayCoupon(int iPage ,int iCount,CouponLst & lst)
{
    int iRet = -1;
    MYSQL_RES* result;

    if( iPage <=0 )
        iPage = 1;
    int iIndex = (iPage-1)*iCount;

    string sSql = "select " + mDetailSql + " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey "
                   "and a.idProduct=d.id and b.state=3 and b.mark=0 and from_unixtime( (unix_timestamp(reqTime)+3600*24*3))<now() and reqTime!=''";

    sSql += sCond;
    sSql += "  order by a.addTime desc ";
    sSql += " limit " + mtConvert.IntToStr(iIndex) + "," + mtConvert.IntToStr(iCount);


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            Coupon entity;

            RowToEntity(row,entity);

            lst.push_back( entity );
        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}



//获取已过期未退款验证码
int CouponOrderDAL::GetTimeOutCoupon(Coupon & coupon)
{
    int iRet = -1;
    MYSQL_RES* result;

    string sSql = "select " + mDetailSql + " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey "
                   "and a.idProduct=d.id and b.state=7 and mark=0";

    sSql += sCond;
    sSql += "  order by a.addTime ";
    sSql += " limit " + mtConvert.IntToStr(0) + "," + mtConvert.IntToStr(1);


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;


        if( NULL != (row =mysql_fetch_row( result )) ) {

            iRet = 0;

            RowToEntity(row,coupon);
        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}

int CouponOrderDAL::GetCouponByPlatform(long lPlatformId,string sStates, int iPage ,int iCount,CouponLst & lst)
{
    int iRet = -1;
    MYSQL_RES* result;

    if( iPage <=0 )
        iPage = 1;
    int iIndex = (iPage-1)*iCount;
    string sSql = "select " + mDetailSql + " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey and a.idProduct=d.id and b.state in("  + sStates  +  ") and a.idPlatform=" + mtConvert.LongToStr(lPlatformId);

    sSql += sCond;
    sSql += " group by b.orderNo,b.state order by a.addTime desc ";
    sSql += " limit " + mtConvert.IntToStr(iIndex) + "," + mtConvert.IntToStr(iCount);


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            Coupon entity;

            RowToEntity(row,entity);

            lst.push_back( entity );
        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}


int CouponOrderDAL::Detail(string Order ,CouponLst & lst)
{
    int iRet = -1;
    MYSQL_RES* result;


    string sSql = "select " + mDetailSql +  " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey and a.idProduct=d.id  and a.orderNo='" + mMulti.RealEscapeString( Order ) + "'";

    sSql += sCond;


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            Coupon entity;

            RowToEntity(row,entity);

            lst.push_back( entity );

        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}



int CouponOrderDAL::GetByCode(string sCode ,int state,Coupon & entity)
{
    int iRet = -1;
    MYSQL_RES* result;

    string sState;
    if( state != -1){
        sState = " and b.state=" + mtConvert.IntToStr(state);
    }

    string sSql = "select " + mDetailSql +  " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey and a.idProduct=d.id  "
                   "and b.code='" + mMulti.RealEscapeString( sCode ) + "'" + sState;

    sSql += sCond;


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        if( NULL != (row =mysql_fetch_row( result )) ) {

            RowToEntity(row,entity);

        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}

int CouponOrderDAL::GetSellCount(long idProduct,int & iSellCount)
{
    string sWhere = " where idProduct=" + mtConvert.LongToStr(idProduct);
    return GetSum("num",sWhere,iSellCount);

}

//获取一个订单下的退款中的验证码
int CouponOrderDAL::GetReqBacking(string Order ,CouponLst & lst)
{
    int iRet = -1;
    MYSQL_RES* result;


    string sSql = "select " + mDetailSql +  " from "+msTableName  + " as a ";
    sSql += " inner join coupon_code as b inner join shopdal as c inner join showpro as d ";

    string sCond = " where b.state=3 and a.orderNo=b.orderNo and a.idPlatform=c.sPrimaryKey and a.idProduct=d.id  and a.orderNo='" + mMulti.RealEscapeString( Order ) + "'";

    sSql += sCond;


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            Coupon entity;

            RowToEntity(row,entity);

            lst.push_back( entity );

        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}


int CouponOrderDAL::GetEntity(string Order,Coupon & entity)
{
    int iRet = -1;
    MYSQL_RES* result;


    string sSql = "select " + msAllColumn + " from "   +msTableName ;

    string sCond = " where orderNo='" + mMulti.RealEscapeString( Order ) + "'";

    sSql += sCond;


    BaseQueryResult(sSql, &result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        if( NULL != (row =mysql_fetch_row( result )) ) {

            int iIndex = 0;
            entity.orderNo = row[iIndex++];
            entity.userId = atol(row[iIndex++]);
            entity.addTime = atol(row[iIndex++]);
            entity.idPlatform = atol(row[iIndex++]);
            entity.idProduct = atol(row[iIndex++]);
            entity.expireTime = row[iIndex++];
            entity.payTime = row[iIndex++];
            entity.endTime = row[iIndex++];
            entity.price = atof( row[iIndex++] );
            entity.coin = atof( row[iIndex++] );
            entity.num = atoi( row[iIndex++] );

        }

    }
    // 释放内存
    FreeResult( result );
    return iRet;

}


void CouponOrderDAL::RowToEntity(MYSQL_ROW row,Coupon & entity)
{
    int iIndex = 0;

    entity.orderNo = row[iIndex++];
    entity.userId = atol(row[iIndex++]);
    entity.addTime = atol(row[iIndex++]);
    entity.idPlatform = atol(row[iIndex++]);
    entity.idProduct = atol(row[iIndex++]);
    entity.expireTime = row[iIndex++];
    entity.payTime = row[iIndex++];
    entity.endTime = row[iIndex++];    
    entity.price = atof( row[iIndex++] );
    entity.coin = atof( row[iIndex++] );
    entity.num = atoi( row[iIndex++] );

    entity.couponCode.state = atoi( row[iIndex++] );
    entity.couponCode.backTime = row[iIndex++];
    entity.couponCode.consumTime = row[iIndex++];
    entity.couponCode.reqTime = row[iIndex++];
    entity.couponCode.code =  row[iIndex++] ;
    entity.couponCode.reason = row[iIndex++];
    entity.couponCode.rejReason = row[iIndex++];

    entity.shopName = row[iIndex++];
    entity.shopType = row[iIndex++];
    entity.shopPhone = row[iIndex++];
    entity.shopAddr = row[iIndex++];
    entity.dGpsLat = atof( row[iIndex++] );
    entity.dGpsLon = atof( row[iIndex++] );
    entity.iOffLineDisCount = atoi( row[iIndex++] );
    entity.iCoinType = atoi( row[iIndex++] );

    entity.content = row[iIndex++];
    entity.picLink = row[iIndex++];
    entity.name = row[iIndex++];
    entity.name = row[iIndex++];
    entity.specification = row[iIndex++];

}

//把已过期的未使用的验证码状态改为已过期（过期时时间为空串时不处理）
void CouponOrderDAL::SetTimeOutState()
{
    string sCurDay = mtConvert.GetCurrentDay();
    string sSql = "update coupon_order as a,coupon_code as b set b.state=7 "
                  "where a.orderNo=b.orderNo and ((b.state=1 || b.state=5) and a.expireTime!='') and a.expireTime<'" +  sCurDay + "'";
    BaseQuery(sSql);
}
