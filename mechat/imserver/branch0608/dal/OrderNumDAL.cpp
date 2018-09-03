#include "OrderNumDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *红包订单号数据
 */


//创建表
int OrderNumDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sOrderNum VARCHAR(32) NOT NULL ,"+
            "sTransferID VARCHAR(64) NOT NULL,"+
            "primary key(sOrderNum)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int OrderNumDAL::AddData( const string & sOrderNum, const string & sTransferID)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "replace into "+this->msTableName+"( sOrderNum,sTransferID)" +
            "VALUES( '"+
            tMultiMysqlDAL.RealEscapeString(sOrderNum)+"','" +
            tMultiMysqlDAL.RealEscapeString(sTransferID)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int OrderNumDAL::GetTransferID(const string & sOrderNum, string & sTransferID)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL tMultiMysqlDAL;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sTransferID ";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " sOrderNum= '" + tMultiMysqlDAL.RealEscapeString( sOrderNum) + "'";



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
            sTransferID =  row[iIndex++];
            iRet = 0;
        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}




