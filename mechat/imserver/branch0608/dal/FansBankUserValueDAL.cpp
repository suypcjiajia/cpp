#include "FansBankUserValueDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "KVlaueEntity.h"
FansBankUserValueDAL::FansBankUserValueDAL()
{
    msTableName = "fansbank_u_value";
    msAllColumn = "select userId,value,type,upTime from ";
}

//update用户价值
int FansBankUserValueDAL::UpValue(long lUserId, double dValue,int iType)
{
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();

    // SQL
    string sSql = "update "+ msTableName+" set value=" + tConvert.DoubleToStr(dValue) + ",upTime=" + tConvert.LongToStr(lCurTime)
            +  " where userId=" + tConvert.LongToStr(lUserId) + " and type=" + tConvert.IntToStr(iType);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//Increase对像下面粉丝的用户价值
int FansBankUserValueDAL::IncreaseValue(double dValue, long id,int iType)
{
    //update fansbank_u_value set value=4.00 where userid in( select luserid from user_datadal where lPushManId=51688) and type=2
    //update fansbank_u_value set value=4.00 where userid in( select luserid from user_datadal where lCurrentPlatformId=100068) and type=1
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();

    // SQL
    string sSql = "update "+ msTableName+" set value=value+" + tConvert.DoubleToStr(dValue) + ",upTime=" + tConvert.LongToStr(lCurTime)
            + " and type=" + tConvert.IntToStr(iType)  +  " where userId in(select luserid from user_datadal where " + tConvert.LongToStr(id) ;

    if( iType == enum_kvalue_type_push){
        sSql += " lPushManId="+ tConvert.LongToStr(id);
    }else{
        sSql += " lCurrentPlatformId="+ tConvert.LongToStr(id);
    }
    sSql += ")";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}



//update用户价值
//sUserids a,b,c format
int FansBankUserValueDAL::UpValuelots(string sUserids, double dValue,int iType)
{
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();

    // SQL
    string sSql = "update "+ msTableName+" set value=" + tConvert.DoubleToStr(dValue) + ",upTime=" + tConvert.LongToStr(lCurTime)
            +  " where userId in(" + sUserids + ") and type=" + tConvert.IntToStr(iType);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int FansBankUserValueDAL::IsExist(long  lUserId, int iType)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where userId="+ tConvert.LongToStr(lUserId)
            + " and type=" + tConvert.IntToStr(iType);

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        iRet = -4;
        return iRet;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        iRet = 0; //exist
    }else if( NULL != result ){
        iRet = 1;//no exist
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

int FansBankUserValueDAL::Add(long lUserId,double dValue  ,int iType )
{
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( userId,value,type,upTime)" +
            "VALUES( '"+
            tConvert.LongToStr( lUserId )+"','" +
            tConvert.DoubleToStr( dValue)+"','" +
            tConvert.IntToStr(iType) + "','" +
            tConvert.LongToStr( lCurTime )+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//获取平台粉丝的用户均价
int FansBankUserValueDAL::GetPlatformAvg(string sShopPhone, long Id,int iType,double & dAvg)
{
    TConvert tConvert;
    if( sShopPhone.empty()){
        sShopPhone = "1111111111";
    }

    string sSql =   "select avg(a.value)  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and b.lCurrentPlatformId=" + tConvert.LongToStr(Id)
            + " and a.type="  + tConvert.IntToStr(iType)  + " and b.sAccount!=" + sShopPhone ;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                if( row[0] == NULL ){

                }else{
                  int iIndex = 0;
                  dAvg = atof( row[iIndex++] );
                }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//获取推荐人粉丝均价
int FansBankUserValueDAL::GetPushAvg(long Id,int iType,double &  dAvg)
{
    TConvert tConvert;
    // SQL

    string sSql =   "select avg(a.value)  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and b.lPushManId=" + tConvert.LongToStr(Id)+
            + " and a.type="  + tConvert.IntToStr(iType);;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                if( row[0] == NULL){

                }else{
                  int iIndex = 0;
                  dAvg = atof( row[iIndex++] );
                }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}




int FansBankUserValueDAL::GetCount(long Id,int iType, int & iCount)
{

    string sSql =   "select count(*)  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and ";


    if( enum_kvalue_type_platform  == iType){
        sSql += "b.lCurrentPlatformId=" + mtConvert.LongToStr(Id);
    }else{
        sSql += "b.lPushManId=" + mtConvert.LongToStr(Id);
    }
    sSql += "  and a.type="  + mtConvert.IntToStr(iType);
    return GetQueryTotalCount(sSql,iCount);

}

//获取一批价值的用户
int FansBankUserValueDAL::GetValueUser(string sSellPhone,long Id, int iType,bool bLow,int iIndex, int iNum, UserValueLst & lst)
{

    string sSql =   "select a.userId,a.value  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and ";


    if( enum_kvalue_type_platform  == iType){
        sSql += "b.lCurrentPlatformId=" + mtConvert.LongToStr(Id);
    }else{
        sSql += "b.lPushManId=" + mtConvert.LongToStr(Id);
    }
    sSql += " and a.type="  + mtConvert.IntToStr(iType);
    sSql += " and b.sAccount!=" + sSellPhone;
    sSql += " and b.iHot=1 ";
    sSql += " order by a.value ";
    if( bLow){
        sSql += " asc ";
    }else{
        sSql += " desc ";
    }
    sSql += " limit " + mtConvert.IntToStr(iIndex) + "," + mtConvert.IntToStr(iNum);

    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql, &result);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                  iRet = 0;
                  int iIndex = 0;
                  UserValueEntity entity;
                  entity.lUserId = atol( row[iIndex++] );
                  entity.dValue = atof( row[iIndex++] );
                  lst.push_back(entity);

        }
    }
    // 释放内存
    FreeResult( result );

    return iRet;
}


int FansBankUserValueDAL::DeIncreem(long lUserId,double dDeValue)
{
    TConvert tConvert;


    string sSql = "update "+ msTableName+" set value=value-"  + tConvert.DoubleToStr(dDeValue)  + " where userId=" + tConvert.LongToStr(lUserId) ;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


