#include "GroupDataDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *群资料mysql访问层
 * */

//字段sGroupHead不用

int GroupDataDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lGroupId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "sGroupName VARCHAR(128) NOT NULL default '',"+
            "lMasterId BIGINT NOT NULL," +
            "sGroupHead VARCHAR(1024) NOT NULL default '',"
            "primary key(lGroupId),"+
            "constraint FK_groupdata_lmasterid foreign key(lMasterId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int GroupDataDAL::IsExist(const long & plGroupId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lGroupId='"+ tConvert.LongToStr(plGroupId)   +"' limit 1;";

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

int GroupDataDAL::AddData( const GroupDataEntity & entity)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( lGroupId,lAddTime,lUpTime," +
            "sGroupName,sGroupHead,lMasterId)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.lGroupId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            entity.sGroupName + "','" +
            entity.sGroupHead + "','" +
            tConvert.LongToStr(entity.lMasterId) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//ret : -5 -数据不存在  0 -成功
int GroupDataDAL::GetEntity(const long & plGroupId,GroupDataEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sGroupName,"
                  "lMasterId,sGroupHead";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " lGroupId= '" + tConvert.LongToStr( plGroupId) + "'";



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

            entity.lGroupId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sGroupName = row[iIndex++];
            entity.lMasterId = atol(row[iIndex++]);
            entity.sGroupHead = row[iIndex++];
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


long GroupDataDAL::GetMaxGroupId()
{
    long lRet = -1;
    // SQL
    string sSql = "select max(lGroupId) from "+this->msTableName+" ;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                if (NULL == row[0]) {
                    lRet = 0;
                } else {
                    lRet = atol(row[0]);
                }
                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return lRet;

}


int GroupDataDAL::UpGroupName(long lGroupId, string sNewName)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sGroupName ='" + mutlidal.RealEscapeString(sNewName) + "'";

    sSql += " where lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int GroupDataDAL::UpGroupHead(long lGroupId, string sStr)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sGroupHead ='" + mutlidal.RealEscapeString(sStr) + "'";

    sSql += " where lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int GroupDataDAL::UpMaster(long lGroupId, long lMaserId)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();

    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",lMasterId ='" + tConvert.LongToStr(lMaserId) + "'";

    sSql += " where lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}




int GroupDataDAL::GetGroupIds(longs & lGroupIds, int iIndex, int iCount)
{

    TConvert tConvert;
    // SQL
    string sSql = "select lGroupId from group_data limit ";
    sSql += tConvert.IntToStr(iIndex);
    sSql += ",";
    sSql += tConvert.IntToStr(iCount);

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                if (NULL == row[0]) {

                } else {
                    lGroupIds.push_back( atol(row[0]));
                }
                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return 0;

}
