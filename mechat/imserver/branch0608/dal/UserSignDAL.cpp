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

//创建每日签到表
int UserSignDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "sDay VARCHAR(12) NOT NULL,"+
            "iState INT NOT NULL default 0,"+
            "PRIMARY KEY(lUserId,sDay)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//更新
int UserSignDAL::Insert( long lUserId, long lAddTime  )
{
    TConvert tConvert;
   // TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( lUserId,lAddTime,sDay)" +
            "VALUES( '"+tConvert.LongToStr(lUserId)+"','" +
            tConvert.LongToStr(lAddTime)+"','" +
            tConvert.TimeToStr(lAddTime/1000000).substr(0,10)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


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

