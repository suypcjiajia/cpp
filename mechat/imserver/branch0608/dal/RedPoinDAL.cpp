#include "RedPoinDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *红点mysql访问层
 * */



int RedPointDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL," +
            "lLastTime BIGINT NOT NULL,"+
            "iType INT default 0," +
            "sMark varchar(128) default ''," +
            "unique(lUserId,iType,sMark)"
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int RedPointDAL::Replace(long lUserId, long lLastTime,int iType,const string &  sMark )
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "replace into "+this->msTableName+"( lUserId,lLastTime," +
            "iType,sMark)" +
            "VALUES( '"+
            tConvert.LongToStr( lUserId )+"','" +
            tConvert.LongToStr( lLastTime ) +"','" +
            tConvert.IntToStr(iType)+"','" +
            tMultiMysqlDAL.RealEscapeString(sMark) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

long RedPointDAL::GetLastTime(long lUserId, int iType,const string &  sMark)
{
    TConvert tConvert;

    long iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL multi;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lLastTime"
                  " from "+msTableName ;


    sSql += " where lUserId=" +  tConvert.LongToStr(lUserId) + " and iType=" + tConvert.IntToStr(iType) + " and sMark='" + multi.RealEscapeString( sMark ) + "'";



    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex = 0;
            iRet =  atol(row[iIndex++]);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


