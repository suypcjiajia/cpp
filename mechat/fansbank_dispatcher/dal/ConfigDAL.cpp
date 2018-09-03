#include "ConfigDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

int ConfigDAL::GetSyncUser(long & lValue){

    TConvert tConvert;
    // SQL

    string sSql =   "select userId from " + msTableName ;

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

                int iIndex = 0;
                lValue = atol( row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}

int ConfigDAL::SetSyncUser( long lValue)
{
    TConvert tConvert;

    // SQL
    string sSql = "update "+ msTableName+" set userId='" + tConvert.LongToStr( lValue )+ "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}



int ConfigDAL::GetSyncTimeTrade(long & lValue){

    TConvert tConvert;
    // SQL

    string sSql =   "select syncTimeTrade from " + msTableName ;

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

                int iIndex = 0;
                lValue = atol( row[iIndex++] );
                lValue /= 1000000;

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}

int ConfigDAL::SetSyncTimeTrade( long lValue)
{
    TConvert tConvert;

    // SQL
    string sSql = "update "+ msTableName+" set syncTimeTrade='" + tConvert.LongToStr( lValue*1000000 )+ "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int ConfigDAL::GetDeValueDay(string & sDay){

    TConvert tConvert;
    // SQL

    string sSql =   "select deValueDay from " + msTableName ;

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

                int iIndex = 0;
                sDay =  row[iIndex++];

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}


int ConfigDAL::SetDeValueDay( string sDay)
{
    TConvert tConvert;

    // SQL
    string sSql = "update "+ msTableName+" set deValueDay='" + sDay + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int ConfigDAL::GetConsumeSyncTime(string & sTime)
{
    TConvert tConvert;
    // SQL

    string sSql =   "select consumeSyncTime from " + msTableName ;

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

                int iIndex = 0;
                sTime =  row[iIndex++];

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

int ConfigDAL::SetConsumeSyncTime( string sTime)
{
    TConvert tConvert;

    // SQL
    string sSql = "update "+ msTableName+" set consumeSyncTime='" + sTime + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

