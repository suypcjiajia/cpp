#include "UserRelationDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//创建表
int UserRelationDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT NOT NULL,"+
            "lToUserId BIGINT NOT NULL,"+
            "iType INT NOT NULL,"+
            "sUserMark VARCHAR(50) NOT NULL default '',"+
            "sToUserMark VARCHAR(50) NOT NULL default '',"+
            "UNIQUE KEY(lUserId,lToUserId),"+
            "constraint FK_usrrel_luserid foreign key(lUserId) references user_datadal(lUserId)," +
            "constraint FK_usrrel_ltouserid foreign key(lToUserId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int UserRelationDAL::IsExist(const long & plUserId, const long & plToUserId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where (lUserId='"+ tConvert.LongToStr(plUserId) +  "' and lToUserId='" +
            tConvert.LongToStr(plToUserId) +"') or (lUserId='"+ tConvert.LongToStr(plToUserId) +  "' and lToUserId='" +
            tConvert.LongToStr(plUserId) +"') limit 1;";

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

int UserRelationDAL::AddData( const UserRelationEntity & entity)
{
    TConvert tConvert;
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"( lUserId,lAddTime,lUpTime,iState," +
            "lToUserId,iType)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.lUserId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lToUserId)+"','" +
            tConvert.IntToStr(entity.iType)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int UserRelationDAL::UpdateType(const long & plUserId, const long & plToUserId , int piType)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iType ='" + tConvert.IntToStr(piType) + "'";
    sSql += " where lUserId='"+ tConvert.LongToStr(plUserId) +"' and lToUserId='" +  tConvert.LongToStr(plToUserId) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int UserRelationDAL::UpdateMark(const long & plUserId, const long & plToUserId , int iWho,const string & sMark)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    if( iWho == 1){
        sSql += ",sUserMark ='" + mutlidal.RealEscapeString(sMark) + "'";
    }else{
        sSql += ",sToUserMark ='" + mutlidal.RealEscapeString(sMark) + "'";
    }
    sSql += " where lUserId='"+ tConvert.LongToStr(plUserId) +"' and lToUserId='" +  tConvert.LongToStr(plToUserId) + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//查询数据
int UserRelationDAL::GetData( const UserRelationQueryEntity & pQueryEntity , UserRelationEntityList & plstOut, int & piTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lUserId,lAddTime,lUpTime,lToUserId,"
                  "iType,sUserMark,sToUserMark";


    sSql +=  " from "+msTableName;

    string sCond = " where iState!=333 and ";
    if(  pQueryEntity.lUserId >= 0){
        sCond += " (lUserId= '" + tConvert.LongToStr( pQueryEntity.lUserId) + "' or";


       sCond += " lToUserId= '" + tConvert.LongToStr( pQueryEntity.lToUserId) + "') and";

    }

    if(  pQueryEntity.iType >= 0){
        sCond += " iType= '" + tConvert.IntToStr( pQueryEntity.iType) + "' and";
    }

    if(  pQueryEntity.lUpTime >= 0){
        sCond += " lUpTime> '" + tConvert.LongToStr( pQueryEntity.lUpTime) + "' and";
    }

    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;


    sSql += " order by lUpTime ";

    //sSql+=" limit "+tConvert.IntToStr(piIndex)+","+tConvert.IntToStr(piCount);
    appendlog(TTDLogger::LOG_DEBUG, "%s",sSql.c_str());


    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            UserRelationEntity entity;
            int iIndex = 0;

            entity.lUserId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);
            entity.sUserMark =  row[iIndex++];
            entity.sToUserMark = row[iIndex++];

            plstOut.push_back(entity);

        }
        sSql = "select count(*)  from " + msTableName ;
        sSql  +=  sCond;

        GetQueryTotalCount(sSql,piTotalCount);
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//ret : -5 -数据不存在  0 -成功
int UserRelationDAL::GetEntity(const long & plUserId, const long & plToUserId,UserRelationEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lUserId,lAddTime,lToUserId,"
                  "iType,sUserMark,sToUserMark";


    sSql +=  " from "+msTableName;

    string sCond = " where iState!=333 and ";
    if(  plUserId > 0 && plToUserId > 0){
        sCond += " (lUserId= '" + tConvert.LongToStr( plUserId) + "' and";
        sCond += " lToUserId= '" + tConvert.LongToStr(plToUserId) + "') or ";
        sCond += " (lUserId= '" + tConvert.LongToStr( plToUserId) + "' and";
        sCond += " lToUserId= '" + tConvert.LongToStr(plUserId) + "')";

    }else{
        return iRet;
    }
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

            entity.lUserId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);
            entity.sUserMark =  row[iIndex++];
            entity.sToUserMark = row[iIndex++];
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int UserRelationDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(psQuery);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        piCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            piCount = atoi(row[0]);
        }
    }
    return iRet;
}
