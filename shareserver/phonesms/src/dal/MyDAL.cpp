#include "MyDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//创建表
int MyDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "ip VARCHAR(24) NOT NULL ,"+
            "first VARCHAR(24) NOT NULL,"+
            "last VARCHAR(24) NOT NULL,"+
            "count BIGINT NOT NULL,"+
            "primary key(ip)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int MyDAL::IncreemOne( const string ip)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;
    string sTime = tConvert.GetCurrentDayTime();
    string sSql;
    // SQL
    sSql = "update  "+this->msTableName+" set count=count+1,last='" + sTime + "' where ip='" + ip + "'"   ;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int MyDAL::UpFirstTime(const string ip)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;

    string sSql;
    // SQL
    sSql = "update  "+this->msTableName+" set first=last,count=0 "  + " where ip='" + ip + "'"   ;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int MyDAL::AddData( const string ip)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;
    string sTime = tConvert.GetCurrentDayTime();
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( ip,first,last,count)" +
            "VALUES( '"+
            ip+"','" +
            sTime + "','" +
            sTime + "','" +
            tConvert.IntToStr(1)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int MyDAL::GetEntity(const string ip, int & count, string & first ,string & last)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select count,first,last ";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " ip='" + ip + "'";



    sSql += sCond;

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        iRet = -22;
        return iRet;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        if ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            count = atoi( row[iIndex++]);
            first  = row[iIndex++];
            last = row[iIndex++];
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}




