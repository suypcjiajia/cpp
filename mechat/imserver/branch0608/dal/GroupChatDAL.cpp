#include "GroupChatDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *群聊mysql访问层
 * */



int GroupChatDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sKey VARCHAR(64) NOT NULL," +
            "lSendId BIGINT NOT NULL," +
            "lGroupId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iType INT NOT NULL," +
            "sContent VARCHAR(2000)  default '',"+
            "iLkstat INT default 0," +
            "sReaded VARCHAR(8000)  default ''," +
            "primary key(sKey),"+
            "constraint FK_groupchat_lsendid foreign key(lSendId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int GroupChatDAL::AddData( const GroupChatEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( sKey,lSendId,lGroupId,lAddTime,lUpTime," +
            "iType,sContent)" +
            "VALUES( '"+
            entity.sKey + "','" +
            tConvert.LongToStr(entity.lSendId)+"','" +
            tConvert.LongToStr(entity.lGroupId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iType)+"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sContent) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//ret : -5 -数据不存在  0 -成功
int GroupChatDAL::GetEntity(const string & sKey,GroupChatEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select sKey ,lSendId, lGroupId,lAddTime,lUpTime,iType,sContent,"
                  "iLkstat,sReaded";


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
            entity.lGroupId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.iType = atoi(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iLkstat = atoi(row[iIndex++]);
            entity.sReaded = row[iIndex++];
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}



//获取未读群聊
int GroupChatDAL::GetMyChat( const long & lUserId, const vector<long> & GroupIdLst,const long & lAddTime, const int & iCount,GroupChatList & ChatLst)
{

    TConvert tConvert;

    if(GroupIdLst.size() == 0 ){
        return 0;
    }

    string sGroupIds;
    for( vector<long>::const_iterator it = GroupIdLst.begin(); it != GroupIdLst.end(); it++){
        sGroupIds += tConvert.LongToStr(*it);
        sGroupIds += ",";
    }
    sGroupIds = sGroupIds.substr(0, sGroupIds.length() - 1);

    // SQL
    string sSql = "select sKey ,lSendId, lGroupId,lAddTime,lUpTime,iType,sContent,"
                  "iLkstat,sReaded";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

    sCond += " lGroupId in(" + sGroupIds + ") and";

    sCond += " sReaded not like '%" + tConvert.LongToStr( lUserId ) + "%' and";

    sCond += " lAddTime < '" + tConvert.LongToStr( lAddTime) + "'";

    sSql += sCond;

    sSql += " order by lAddTime desc limit ";

    sSql+= tConvert.IntToStr(iCount);

    return Execute(sSql,ChatLst);

}

//获取未读群聊(去掉加入群之前的聊天)
int GroupChatDAL::GetMyChat(const long & lUserId,const GroupMemberList & memberTimeLst,const long & lAddTime, const int & iCount,GroupChatList & ChatLst)
{
    TConvert tConvert;

    if(memberTimeLst.size() == 0 ){
        return 0;
    }

    string sGroupIds;

    for( GroupMemberList::const_iterator it = memberTimeLst.begin(); it != memberTimeLst.end(); it++){

        const GroupMemberEntity &  memberTime = *it;
        sGroupIds += "(";
        sGroupIds += "lGroupId=";
        sGroupIds += tConvert.LongToStr( memberTime.lGroupId);
        sGroupIds += " and lAddTime>";
        sGroupIds += tConvert.LongToStr(memberTime.lUpTime);
        sGroupIds += " ) or";

    }
    sGroupIds = sGroupIds.substr(0, sGroupIds.length() - 2);

    // SQL
    string sSql = "select sKey ,lSendId, lGroupId,lAddTime,lUpTime,iType,sContent,"
                  "iLkstat,sReaded";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

    sCond += " (" + sGroupIds + ") and";

    sCond += " sReaded not like '%" + tConvert.LongToStr( lUserId ) + "%' and";

    sCond += " lAddTime < '" + tConvert.LongToStr( lAddTime) + "'";

    sSql += sCond;

    sSql += " order by lAddTime desc limit ";

    sSql+= tConvert.IntToStr(iCount);

    return Execute(sSql,ChatLst);

}


int GroupChatDAL::UpdateReaded(long lUserId,const GroupChatList & lst)
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


    GroupChatList::const_iterator it = lst.begin();
    for( ; it != lst.end() ; it++){
        const GroupChatEntity & entity = *it;
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


int GroupChatDAL::Execute(const string & sSql,GroupChatList & chatLst)
{
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    int iRet = -1;

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            GroupChatEntity entity;
            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lSendId = atol( row[iIndex++]);
            entity.lGroupId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.iType = atoi(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.iLkstat = atoi(row[iIndex++]);
            entity.sReaded = row[iIndex++];

            chatLst.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}
