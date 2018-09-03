#include "FriendResquestDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//创建表
int FriendResquestDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT default 0,"+
            "lToUserId BIGINT NOT NULL,"+
            "iAction INT default 0,"+
            "sRemark VARCHAR(50) NOT NULL default '',"+
            "UNIQUE KEY(lUserId,lToUserId),"+
            "constraint FK_frireq_luserid foreign key(lUserId) references user_datadal(lUserId)," +
            "constraint FK_frireq_ltouserid foreign key(lToUserId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int FriendResquestDAL::IsExist(const long & plUserId, const long & plToUserId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lUserId='"+ tConvert.LongToStr(plUserId) +  "' and lToUserId='" +
            tConvert.LongToStr(plToUserId) +"' limit 1;";

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

int FriendResquestDAL::AddData( const FriendResquestEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "replace into "+this->msTableName+"( lUserId,lAddTime,lUpTime,iState," +
            "lToUserId,iAction,sRemark)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.lUserId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lToUserId)+"','" +
            tConvert.IntToStr(entity.iAction)+"','" +
            tMultiMysqlDAL.RealEscapeString(entity.sRemark)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int FriendResquestDAL::UpdateAction(const long & plUserId, const long & plToUserId , int piAction)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iAction ='" + tConvert.IntToStr(piAction) + "'";
    sSql += " where lUserId='"+ tConvert.LongToStr(plUserId) +"' and lToUserId='" +  tConvert.LongToStr(plToUserId) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//查询数据
int FriendResquestDAL::GetData( const FriendResquestQueryEntity & pQueryEntity , int piIndex, int piCount, FriendResquestList & plstOut, int & piTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lUserId,lAddTime,lToUserId,"
                  "iAction,sRemark";


    sSql +=  " from "+msTableName;

    string sCond = " where iState!=333 and ";
    if(  pQueryEntity.lUserId >= 0){
        sCond += " lUserId= '" + tConvert.LongToStr( pQueryEntity.lUserId) + "' and";
    }
    if(  pQueryEntity.lToUserId >= 0){
        sCond += " lToUserId= '" + tConvert.LongToStr( pQueryEntity.lToUserId) + "' and";
    }
    if(  pQueryEntity.lUpdateId >= 0){
        sCond += " lUpTime> '" + tConvert.LongToStr( pQueryEntity.lUpdateId) + "' and";
    }
    if(  pQueryEntity.iAction >= 0){
        sCond += " iAction= '" + tConvert.LongToStr( pQueryEntity.iAction) + "' and";
    }

    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;


    sSql += " order by lAddTime desc";

    sSql+=" limit "+tConvert.IntToStr(piIndex)+","+tConvert.IntToStr(piCount);

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


            FriendResquestEntity entity;
            int iIndex = 0;

            entity.lUserId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iAction = atoi( row[iIndex++]);
            entity.sRemark = row[iIndex++];


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

int FriendResquestDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
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



