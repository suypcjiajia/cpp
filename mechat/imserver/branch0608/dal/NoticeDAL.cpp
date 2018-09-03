#include "NoticeDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *系统通知mysql访问层
 * */



int NoticeDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sKey VARCHAR(64) NOT NULL," +
            "lSendId BIGINT NOT NULL," +
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iTarget INT default 1," +
            "sContent VARCHAR(2000)  default '',"+
            "sRevIds VARCHAR(2000)  default ''," +
            "sReaded LongText  default ''," +
            "iType INT default 1," +
            "primary key(sKey),"+
            "constraint FK_notice_lsendid foreign key(lSendId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int NoticeDAL::AddData( const NoticeEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( sKey,lSendId,lAddTime,lUpTime," +
            "sContent,iTarget,iType,sRevIds)" +
            "VALUES( '"+
            entity.sKey + "','" +
            tConvert.LongToStr(entity.lSendId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tMultiMysqlDAL.RealEscapeString(entity.sContent) + "','" +
            tConvert.IntToStr(entity.iTarget)+"','" +
            tConvert.IntToStr(entity.iType)+"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sRevIds) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//ret : -5 -数据不存在  0 -成功
int NoticeDAL::GetEntity(const string & sKey,NoticeEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey ,lSendId,lAddTime,lUpTime,sContent,"
                  "iTarget,sRevIds,sReaded,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " sKey= '" + sKey + "'";

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

            entity.sKey = row[iIndex++];
            entity.lSendId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iTarget = (EnumTarget)atoi(row[iIndex++]);
            entity.sRevIds = row[iIndex++];
            entity.sReaded = row[iIndex++];
            entity.iType = atoi(row[iIndex++]);
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


//获取未读通知
int NoticeDAL::GetMyNotice( const long & lUserId,int iType,NoticeList & Lst)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey ,lSendId,lAddTime,lUpTime,sContent,"
                  "iTarget,sRevIds,sReaded,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

    if( iType != -1){
      sCond += " iType= '" + tConvert.IntToStr( iType) + "' and";
    }

    sCond += " sReaded not like '%" + tConvert.LongToStr( lUserId ) + "%' and";


    sCond += " (iTarget=1 or (iTarget=2 and";

    sCond += " sRevIds  like '%\\\"" + tConvert.LongToStr( lUserId ) + "\\\"%')) ";

    sSql += sCond;

    sSql += " order by lAddTime asc ";

    //appendlog(TTDLogger::LOG_DEBUG,"xxxx:%s",sSql.c_str());

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            NoticeEntity entity;
            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lSendId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iTarget = ( EnumTarget)atoi(row[iIndex++]);
            entity.sRevIds = row[iIndex++];
            entity.sReaded = row[iIndex++];
            entity.iType = atoi(row[iIndex++]);

            Lst.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//获取红点情况
int NoticeDAL::GetCountByLastTime(long lUserId,long lLastTime, int & iCount)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select count(*) ";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

   // sCond += " iType= '" + tConvert.IntToStr( iType) + "' and";

    sCond += " lAddTime> '" + tConvert.LongToStr( lLastTime) + "' and";


    sCond += " (iTarget=1 or (iTarget=2 and";

    sCond += " sRevIds  like '%" + tConvert.LongToStr( lUserId ) + "%')) ";

    sSql += sCond;

    sSql += " order by lAddTime desc limit 1 ";


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

            iCount = atoi(row[iIndex++]);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//获取最后一条记录
int NoticeDAL::GetLastRecord(long lUserId,NoticeEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey ,lSendId,lAddTime,lUpTime,sContent,"
                  "iTarget,sRevIds,sReaded,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

   // sCond += " iType= '" + tConvert.IntToStr( iType) + "' and";

    sCond += " (iTarget=1 or (iTarget=2 and";

    sCond += " sRevIds  like '%" + tConvert.LongToStr( lUserId ) + "%')) ";

    sSql += sCond;

    sSql += " order by lAddTime desc limit 1 ";


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

            entity.sKey = row[iIndex++];
            entity.lSendId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iTarget = ( EnumTarget)atoi(row[iIndex++]);
            entity.sRevIds = row[iIndex++];
            entity.sReaded = row[iIndex++];
            entity.iType = atoi(row[iIndex++]);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}




//更新为已读
int NoticeDAL::UpdateReaded(long lUserId,const NoticeList & lst)
{
    if( lst.size() == 0){
        return 0;
    }
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",sReaded =concat(sReaded,'" + tConvert.LongToStr(lUserId) + ",')";
    sSql += " where sKey in('";


    NoticeList::const_iterator it = lst.begin();
    for( ; it != lst.end() ; it++){
        const NoticeEntity & entity = *it;
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
