#include "PrivateChatDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
//创建表
int PrivateChatDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sKey VARCHAR(64) NOT NULL primary key," +
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT NOT NULL default 0,"+
            "lToUserId BIGINT NOT NULL,"+
            "iType INT NOT NULL,"+
            "iLkstat INT default 0,"
            "sContent VARCHAR(2000) NOT NULL default '',"+
            "constraint FK_prichat_luserid foreign key(lUserId) references user_datadal(lUserId)," +
            "constraint FK_prichat_ltouserid foreign key(lToUserId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//添加一条私聊
int PrivateChatDAL::AddData( const PrivateChatEntity & entity)
{
    TConvert tConvert;
    string sSql;
    TMultiMysqlDAL multimysql;
    // SQL
    sSql = "Insert into "+this->msTableName+"( sKey,lUserId,lAddTime,lUpTime,iState," +
            "lToUserId,iType,sContent)" +
            "VALUES( '"+
            entity.sKey+"','" +
            tConvert.LongToStr(entity.lUserId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lToUserId)+"','" +
            tConvert.IntToStr(entity.iType)+"','" +
            multimysql.RealEscapeString(entity.sContent) +
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }

    return con->Query(sSql);

}

//更新状态
int PrivateChatDAL::UpdateState(const string &  psKey, int piState)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iState ='" + tConvert.IntToStr(piState) + "'";
    sSql += " where sKey='"+ multi.RealEscapeString( psKey ) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//更新红包状态
int PrivateChatDAL::UpdateLkStat(const string & psKey, EnumLkMoneyStat iState)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iLkstat ='" + tConvert.IntToStr(iState) + "'";
    sSql += " where sKey='"+ multi.RealEscapeString( psKey ) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int PrivateChatDAL::UpdateState(const PrivateChatList & lst, int iState)
{
    if( lst.size() == 0){
        return 0;
    }
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iState ='" + tConvert.IntToStr(iState) + "'";
    sSql += " where sKey in('";

    PrivateChatList::const_iterator it = lst.begin();
    for( ; it != lst.end() ; it++){
        const PrivateChatEntity & entity = *it;
        sSql += entity.sKey + "','";
    }
    sSql = sSql.substr(0,sSql.length() -2);
    sSql +=  ")";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//获取我的聊天
int PrivateChatDAL::GetMyChat( const long & lUserId, const long & lToUserId,const long & lAddTime, const int & iCount,PrivateChatList & plstOut)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey,lUserId,lAddTime,iState,lToUserId,"
                  "iType,sContent,iLkstat ";


    sSql +=  " from "+msTableName;

    string sCond = " where iState=0 and ";

    if( lToUserId == -1){
        sCond += " lToUserId= '" + tConvert.LongToStr( lUserId) + "' and";

    }else{

        //sCond += " (lUserId= '" + tConvert.LongToStr( lUserId) + "' and";
        //sCond += " lToUserId= '" + tConvert.LongToStr( lToUserId) + "') or";
        sCond += " (lUserId= '" + tConvert.LongToStr( lToUserId) + "' and";
        sCond += " lToUserId= '" + tConvert.LongToStr( lUserId) + "') and";

    }
    sCond += " lAddTime < '" + tConvert.LongToStr( lAddTime) + "'";

    sSql += sCond;

    sSql += " order by lAddTime desc limit ";

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


            PrivateChatEntity entity;
            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lUserId = atol(row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.iState = atoi(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iLkstat = (EnumLkMoneyStat)atoi(row[iIndex++]);

            plstOut.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}
//获取未过期最旧的100条红包
int PrivateChatDAL::GetLockyMoney(PrivateChatList & lst)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey,lUserId,lAddTime,iState,lToUserId,"
                  "iType,sContent ";


    sSql +=  " from "+msTableName;

    string sCond = " where iType=5 and iLkstat=0";
    sSql += sCond;

    sSql += " order by lAddTime limit 100";


    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            PrivateChatEntity entity;
            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lUserId = atol(row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.iState = atoi(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);
            entity.sContent = row[iIndex++];

            lst.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


//ret : -5 -数据不存在  0 -成功
int PrivateChatDAL::GetEntity(const string & psKey,PrivateChatEntity  & entity )
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey,lUserId,lAddTime,iState,lToUserId,"
                  "iType,sContent,iLkstat";


    sSql +=  " from "+msTableName;

    string sCond = " where iState!=333 and ";
    sCond += " sKey='" + psKey + "'";
    sSql += sCond;



    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = ERRNOTDATA;;

        if ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lUserId = atol(row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.iState = atoi(row[iIndex++]);
            entity.lToUserId = atol( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iLkstat = (EnumLkMoneyStat)atoi(row[iIndex++]);

            iRet = 0;

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


int PrivateChatDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
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
