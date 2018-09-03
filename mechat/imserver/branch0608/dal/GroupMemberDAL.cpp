#include "GroupMemberDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/*
 *群成员mysql访问层
 * */



int GroupMemberDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "Id BIGINT auto_increment,"+
            "lGroupId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "sNickName VARCHAR(128)  default '',"+
            "lMemberId BIGINT NOT NULL," +
            "iJoin INT default 0," +
            "iType INT default 1,"
            "primary key(Id),"+
            "iMsgMode INT default 0," +
            "constraint FK_groupmember_lgroupid foreign key(lGroupId) references group_data(lGroupId)," +
            "constraint FK_groupmember_lmemberid foreign key(lMemberId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int GroupMemberDAL::IsExist( const long & plGroupId,const long & plMemberId )
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lGroupId='"+ tConvert.LongToStr(plGroupId) +
            " and lMemberId="  + tConvert.LongToStr(plMemberId)  +"' limit 1;";

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

int GroupMemberDAL::AddData( const GroupMemberEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( lGroupId,lAddTime,lUpTime," +
            "sNickName,lMemberId,iJoin)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.lGroupId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sNickName) + "','" +
            tConvert.LongToStr(entity.lMemberId) + "','" +
            tConvert.IntToStr(entity.iJoin) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//ret : -5 -数据不存在  0 -成功
int GroupMemberDAL::GetEntity(const long & plGroupId,const long & plMemberId,GroupMemberEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " lGroupId= '" + tConvert.LongToStr( plGroupId) + "'";
    sCond += " and ";
    sCond += " lMemberId= '" + tConvert.LongToStr( plMemberId) + "'";



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
            entity.sNickName = row[iIndex++];
            entity.lMemberId = atol(row[iIndex++]);
            entity.iJoin = atoi(row[iIndex++]);
            entity.iType = atoi(row[iIndex++]);
            iRet = 0;


        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


//我加入的群(包括没关系和群成员)
int GroupMemberDAL::GetMyGroup( const long & lUserId, const long & lUpTime,GroupMemberList & plstOut)
{

    TConvert tConvert;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " lMemberId= '" + tConvert.LongToStr( lUserId) + "' and ";
    sCond += " lUpTime> '" + tConvert.LongToStr( lUpTime) + "'";

    sSql += sCond;

    return Execute(sSql,plstOut);

}


//我加入的群
int GroupMemberDAL::GetMyGroup( const long & lUserId,GroupMemberList & plstOut)
{

    TConvert tConvert;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " lMemberId= '" + tConvert.LongToStr( lUserId) + "' and ";
    sCond += " iType= '" + tConvert.LongToStr( enum_membertype_member) + "'";

    sSql += sCond;

    return Execute(sSql,plstOut);

}


//一个群的所有成员(包括没关系和群成员)
int GroupMemberDAL::GetMemberById(const long & lGroupId,GroupMemberList & plstOut)
{
    TConvert tConvert;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";
    sCond += " lGroupId= '" + tConvert.LongToStr( lGroupId) + "'";
    sSql += sCond;

    return Execute(sSql,plstOut);

}



//一批群的成员(按时间戳)
int GroupMemberDAL::GetMemberBylotsId(GroupUpTimeList gruopUpLst,int iType ,GroupMemberList & plstOut)
{

    TConvert tConvert;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  iType=";
    sCond += tConvert.IntToStr(iType);
    sCond += " and (";

    GroupUpTimeList::iterator it = gruopUpLst.begin();
    for( ; it != gruopUpLst.end() ; it++){
        GroupUpTime & gUpTime = *it;
        sCond += " (lGroupId= '" + tConvert.LongToStr( gUpTime.lGroupId) + "' and ";
        sCond += " lUpTime> '" + tConvert.LongToStr( gUpTime.lUpTime) + "') ";
        sCond += "or";

    }
    sCond = sCond.substr(0, sCond.size() - 2);
    sCond += ")";

    sSql += sCond;

    return Execute(sSql,plstOut);
}

//一个群的所有成员(只包括群成员)
int GroupMemberDAL::GetRealMemberById(const long & lGroupId,GroupMemberList & plstOut)
{
    TConvert tConvert;

    // SQL
    string sSql = "select lGroupId,lAddTime,lUpTime,sNickName,"
                  "lMemberId,iJoin,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";
    sCond += " lGroupId= '" + tConvert.LongToStr( lGroupId) + "' and ";
    sCond += " iType= '" + tConvert.LongToStr( enum_membertype_member) + "'";
    sSql += sCond;

    return Execute(sSql,plstOut);

}

//更新和群的关系
int GroupMemberDAL::UpDataType( long  lGroupId, long  lMemberId, int iType )
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iType ='" + tConvert.IntToStr(iType) + "'";
    sSql += " where lGroupId='"+ tConvert.LongToStr(lGroupId) +"' and lMemberId='" +  tConvert.LongToStr(lMemberId) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int GroupMemberDAL::SetNickName(long lMemberId, long lGroupId, string sNickName)
{

    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sNickName ='" + mutlidal.RealEscapeString(sNickName) + "'";

    sSql += " where lMemberId='"+ tConvert.LongToStr(lMemberId)  + "'";
    sSql += " and  lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int GroupMemberDAL::SetMsgMode(long lMemberId,long lGroupId, int iMsgMode)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",iMsgMode ='" + tConvert.IntToStr(iMsgMode) + "'";

    sSql += " where lMemberId='"+ tConvert.LongToStr(lMemberId)  + "'";
    sSql += " and  lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int GroupMemberDAL::Execute(const string & sSql,GroupMemberList & plstOut)
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


            GroupMemberEntity entity;
            int iIndex = 0;

            entity.lGroupId = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sNickName = row[iIndex++];
            entity.lMemberId = atol( row[iIndex++]);
            entity.iJoin = atoi( row[iIndex++]);
            entity.iType = atoi( row[iIndex++]);

            plstOut.push_back(entity);

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}
