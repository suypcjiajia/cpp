#include "PublicInfoDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/**
 *Copyright(C),TTD
 * 公众号信息数据访问
 * @author suyp
 * @modify suyp
 */


/**
 * 创建表格
 * @param
 * @return
 */
int PublicInfoDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sKey VARCHAR(64) NOT NULL," +
            "lSendId BIGINT NOT NULL," +
            "Id BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iType INT NOT NULL," +
            "sContent VARCHAR(2000)  default '',"+
            "sReaded VARCHAR(8000)  default ''," +
            "sRevId VARCHAR(2000)  default ''," +
            "primary key(sKey),"+
            "constraint FK_publicinfo_lsendid foreign key(lSendId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int PublicInfoDAL::AddData( const PublicInfoEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( sKey,lSendId,Id,lAddTime,lUpTime," +
            "iType,sRevId,sContent)" +
            "VALUES( '"+
            entity.sKey + "','" +
            tConvert.LongToStr(entity.lSendId)+"','" +
            tConvert.LongToStr(entity.Id)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iType)+"','" +
            tMultiMysqlDAL.RealEscapeString(entity.sRev)+"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sContent) +
            "')";

    return BaseQuery(sSql);

}


/**
 * 获取一条记录
 * @param sKey 记录Id
 * @param entity
 * @return 0 sucess
 */
int PublicInfoDAL::GetEntity(string sKey,PublicInfoEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select sKey,lSendId,Id,lAddTime,lUpTime,iType,sContent";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " sKey='" + multi.RealEscapeString( sKey) + "'";


    sSql += sCond;

    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        if ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            entity.sKey =  row[iIndex++];
            entity.lSendId = atol( row[iIndex++] );
            entity.Id = atol( row[iIndex++] );
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.iType =  atoi( row[iIndex++] );
            entity.sContent =  row[iIndex++];
            iRet = 0;
        }
    }

    FreeResult(result);

    return iRet;

}


/**
 * 获取未读
 * @param entity
 * @return 0 sucess
 */
int PublicInfoDAL::GetMyInfo( long  lUserId, const longs & ids, long  lAddTime,  int  iCount,PublicInfoList & infos)
{

    TConvert tConvert;

    if(ids.size() == 0 ){
        return 0;
    }

    string sIds;
    for( vector<long>::const_iterator it = ids.begin(); it != ids.end(); it++){
        sIds += tConvert.LongToStr(*it);
        sIds += ",";
    }
    sIds = sIds.substr(0, sIds.length() - 1);

    // SQL
    string sSql = "select sKey ,lSendId, Id,lAddTime,lUpTime,iType,sContent ";


    sSql +=  " from "+msTableName;

    string sCond = " where ";

    sCond += " lSendId!= '" + tConvert.LongToStr( lUserId) + "' and";

    sCond += " Id in(" + sIds + ") and";

    sCond += " sReaded not like '%" + tConvert.LongToStr( lUserId ) + "%' and";

    sCond += " ( sRevId='' || sRevId like '%" + tConvert.LongToStr(lUserId) + "%') and";

    sCond += " lAddTime < '" + tConvert.LongToStr( lAddTime) + "'";

    sSql += sCond;

    sSql += " order by lAddTime desc limit ";

    sSql+= tConvert.IntToStr(iCount);

    appendlog(TTDLogger::LOG_DEBUG, "xxxx:%s",sSql.c_str());

    return Execute(sSql,infos);

}

int PublicInfoDAL::Execute(const string & sSql,PublicInfoList & infos)
{
    MYSQL_RES* result = NULL;

    int iRet = -1;

    iRet = BaseQueryResult(sSql, & result);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            PublicInfoEntity entity;
            int iIndex = 0;

            entity.sKey = row[iIndex++];
            entity.lSendId = atol( row[iIndex++]);
            entity.Id = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.iType = atoi(row[iIndex++]);
            entity.sContent = row[iIndex++];
            entity.sReaded = row[iIndex++];

            infos.push_back(entity);

        }
    }

    FreeResult( result );
    return iRet;

}



int PublicInfoDAL::UpdateReaded(long lUserId,const PublicInfoList & lst)
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


    PublicInfoList::const_iterator it = lst.begin();
    for( ; it != lst.end() ; it++){
        const PublicInfoEntity & entity = *it;
        sSql += entity.sKey + "','";
    }
    sSql = sSql.substr(0,sSql.length() -2);
    sSql +=  ")";

    return BaseQuery(sSql);

}
