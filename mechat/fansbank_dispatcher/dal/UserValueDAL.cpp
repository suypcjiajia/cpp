#include "UserValueDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"



int UserValueDAL::IsExist(long  lUserId, int iType)
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

int UserValueDAL::Add(long lUserId,double dValue  ,int iType )
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
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

//获取用户价值
int UserValueDAL::GetUserValue(long lUserId, int iType ,double & dValue){

    TConvert tConvert;
    // SQL

    string sSql =   "select value from " + msTableName
            + " where userId=" + tConvert.LongToStr(lUserId) + " and type=" + tConvert.IntToStr(iType);

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

        if ( NULL != (row == mysql_fetch_row( result )) ) {

                iRet = 0;

                int iIndex = 0;
                dValue = atof( row[iIndex++] );
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}

//增加用户价值
int UserValueDAL::IncreemValue(long lUserId, double dValue,int iType)
{
    TConvert tConvert;
    long lCurTime = tConvert.GetCurrentTime();

    // SQL
    string sSql = "update "+ msTableName+" set value=value+" + tConvert.DoubleToStr(dValue) + ",upTime=" + tConvert.LongToStr(lCurTime)
            +  " where userId=" + tConvert.LongToStr(lUserId) + " and type=" + tConvert.IntToStr(iType);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//对于昨天没签到并且没有消费的用户,每用户贬值0.5元
int UserValueDAL::DeIncreemVeryUser()
{
    TConvert tConvert;

    string sTime =  tConvert.GetYesterDay() + " 00:00:00";
    long lTime = tConvert.StrToTime(sTime.c_str())*1000000;//昨天的第一秒

    string sSql = "update "+ msTableName+" set value=value-0.5 where value>30.0 and upTime<" + tConvert.LongToStr( lTime ) ;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//针对价值小于5.0的用户，加0.05元价值
int UserValueDAL::IncreemVeryUser()
{
    TConvert tConvert;

    string sSql = "update "+ msTableName+" set value=value+0.02 where value<30.0" ;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//小于保低值的用户，设置成保低值
int UserValueDAL::SetLowest()
{

    string sSql = "update "+ msTableName+" set value=2.5 where value<2.5";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//获取平台粉丝的用户均价
int UserValueDAL::GetPlatformAvg(string sShopPhone,long Id,int iType,double & dAvg)
{
    TConvert tConvert;
    // SQL

    string sSql =   "select avg(a.value)  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and b.lCurrentPlatformId=" + tConvert.LongToStr(Id)
            + " and a.type="  + tConvert.IntToStr(iType) + " and b.sAccount!='" + sShopPhone +"'" ;

    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql,&result);
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
    FreeResult( result );

    return iRet;


}


//获取推荐人粉丝均价
int UserValueDAL::GetPushAvg(long Id,int iType,double &  dAvg)
{
    TConvert tConvert;
    // SQL

    string sSql =   "select avg(a.value)  from " + msTableName +
            " as a inner join user_datadal as b where a.userId=b.lUserId and b.lPushManId=" + tConvert.LongToStr(Id)+
            + " and a.type="  + tConvert.IntToStr(iType) + " and b.lUserId!=" + tConvert.LongToStr(Id);

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

