#include "UserSignDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


int UserSignDAL::IsExist(long  lUserId, const string & sDay)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lUserId='"+ tConvert.LongToStr(lUserId) + "' and sDay='" +
            sDay +"' limit 1;";

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




//获取未计算的签到
int UserSignDAL::GetSigns(UserSignLst & userSignLst){

    TConvert tConvert;
    // SQL

    string sSql =   "select lUserId,lAddTime,sDay from " + msTableName + " where iState=0 order by lAddTime limit 10";

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                UserSignEntity entity;

                int iIndex = 0;


                entity.lUserId = atol( row[iIndex++] );
                entity.lAddTime = atol( row[iIndex++] );
                entity.sDay =  row[iIndex++];

                userSignLst.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}


//设置签到为已计算状态
int UserSignDAL::SetState( const longs & lUserIds)
{
    TConvert tConvert;

    if( lUserIds.size() == 0)
        return 0;

    string sUserIds = "(";
    for( int i = 0; i < lUserIds.size() ; i++){
        sUserIds += tConvert.LongToStr(lUserIds[i]);
        sUserIds += ",";
    }
    sUserIds = sUserIds.substr(0,sUserIds.length() -1);
    sUserIds += ")";
    // SQL
    string sSql = "update "+ msTableName+" set iState=1 where lUserId in" + sUserIds;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

