#include "UserGradeDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//创建表
int UserGradeDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "iLevel INT NOT NULL default 0,"+
            "primary key(lUserId),"+
            "constraint FK_usergrade_luserid foreign key(lUserId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int UserGradeDAL::Add(const long & lUserId, int & iLevel)
{
    TConvert tConvert;
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"( lUserId,lAddTime,iLevel)" +
            "VALUES( '"+
            tConvert.LongToStr(lUserId)+"','" +
            tConvert.LongToStr(tConvert.GetCurrentTime())+"','" +
            tConvert.IntToStr(iLevel)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int UserGradeDAL::IsExist(const long & lUserId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lUserId='"+ tConvert.LongToStr(lUserId)   +"' limit 1;";

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



int UserGradeDAL::UpdateLevel(const long & lUserId, int & iLevel)
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iLevel =" + tConvert.LongToStr(iLevel) ;
    sSql += " where lUserId="+ tConvert.IntToStr(lUserId);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int UserGradeDAL::GetVip(int iPage, int iCount ,UserGradeList & lst)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    iPage = ( iPage <= 0 ? 1 : iPage);
    int iIndex = (iPage - 1)*iCount;

    // SQL
    string sSql = "select lUserId,lAddTime,iLevel ";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " iLevel=1 ";

    sSql += sCond;

    sSql += " order by lAddTime desc limit ";

    sSql+= tConvert.IntToStr(iIndex);
    sSql += ",";
    sSql+= tConvert.IntToStr(iCount);


    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            UserGradeEntity entity;
            int iIndex = 0;

            entity.lUserId = atol(row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.iLevel = atoi( row[iIndex++]);

            lst.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int UserGradeDAL::GetLevel(const long & lUserId, int & iLevel)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select iLevel";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " lUserId=" + tConvert.LongToStr( lUserId);


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

            iLevel = atol( row[iIndex++]);
            iRet = 0;

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}




