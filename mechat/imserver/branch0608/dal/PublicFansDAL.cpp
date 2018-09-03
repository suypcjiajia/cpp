#include "PublicFansDAL.h"

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
 * 公众号粉丝数据访问
 * @author suyp
 * @modify suyp
 */



/**
 * 创建表格
 * @param
 * @return
 */
int PublicFansDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "Id BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iType int not null default 0,"+
            "lFansId BIGINT NOT NULL," +
            "iCount int NOT NULL default 0,"
            "constraint FK_publicfans_lfansid foreign key(lFansId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    return BaseQuery(sSql);

}

/**
 * 是否存在
 * @param Id 公众号 lFansId 粉丝
 * @return 0 存在
 */
int PublicFansDAL::IsExist(long  Id,long lFansId)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where Id="+ tConvert.LongToStr(Id) +
            "  and lFansId=" +  tConvert.LongToStr(lFansId)  +" limit 1;";

    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);


    if ( NULL != result && result->row_count > 0) {
        iRet = 0; //exist
    }else if( NULL != result ){
        iRet = 1;//no exist
    }
    FreeResult(result);

    return iRet;

}


/**
 * 添加一条记录
 * @param entity
 * @return 0 sucess
 */
int PublicFansDAL::AddData( const PublicFansEntity & entity)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( Id,lAddTime,lUpTime," +
            "iType,lFansId,iCount)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.Id)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iType) + "','" +
            tConvert.IntToStr(entity.lFansId) + "','" +
            tConvert.IntToStr(entity.iCount) +
            "')";

    return BaseQuery(sSql);

}


/**
 * 获取一条记录
 * @param id 公众号 lFansId 粉丝
 * @param entity
 * @return 0 sucess
 */
int PublicFansDAL::GetEntity(long  id,long lFansId,PublicFansEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "Id,lAddTime,lUpTime,iType,lFansId,iCount";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += " Id=" + tConvert.LongToStr( id) + " and lFansId=" + tConvert.LongToStr(lFansId) ;


    sSql += sCond;

    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        if ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            entity.Id = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.iType =  atoi( row[iIndex++] );
            entity.lFansId = atoi( row[iIndex++] );
            entity.iCount = atoi(row[iIndex++]);

            iRet = 0;


        }
    }

    FreeResult(result);

    return iRet;

}

/**
 * 增加消费次数
 * @param id 公众号 lFansId 粉丝
 * @param iIncreen 增加的消费次数
 * @return 0 sucess
 */
int PublicFansDAL::UpdateCount(long Id, long lFansId, int iIncreen)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",iCount=iCount+" + tConvert.IntToStr(iIncreen) ;

    sSql += " where Id="+ tConvert.LongToStr(Id)  +  " and lFansId=" + tConvert.LongToStr(lFansId);

    return BaseQuery(sSql);

}

/**
 * 变新粉丝类型
 * @param id 公众号 lFansId 粉丝
 * @param iType 粉丝类型
 * @return 0 sucess
 */
int PublicFansDAL::UpdateType(long Id, long lFansId, int iType)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",iType=" + tConvert.IntToStr(iType) ;

    sSql += " where Id="+ tConvert.LongToStr(Id)  +  " and lFansId=" + tConvert.LongToStr(lFansId);

    return BaseQuery(sSql);

}


/**
 * retset消费次数
 * @param id 公众号 lFansId 粉丝
 * @param iCount 次数
 * @return 0 sucess
 */
int PublicFansDAL::ResetCount(long Id, long lFansId)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",iCount=" + tConvert.IntToStr(0) ;

    sSql += " where Id="+ tConvert.LongToStr(Id)  +  " and lFansId=" + tConvert.LongToStr(lFansId);

    return BaseQuery(sSql);

}


/**
 * 获取我消费3次以上的平台
 * @param lUserId 我id
 * @param iIncreen 增加的消费次数
 * @return 0 sucess
 */
int PublicFansDAL::GetMyPublic(long lUserId, PublicFansList & publics)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;

    // SQL
    string sSql = "select Id,lAddTime,iType";


    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += "lFansId=" + tConvert.LongToStr(lUserId) + " and iCount>=3 and iType=0";


    sSql += sCond;

    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            PublicFansEntity entity;

            entity.Id = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.iType =  atoi( row[iIndex++] );

            publics.push_back(entity);


            iRet = 0;


        }
    }

    FreeResult(result);

    return iRet;

}


/**
 * 获到公众号上的消费粉丝数
 * @param Id 公众号id
 * @return 0 sucess iCount 消费粉丝数
 */
int PublicFansDAL::GetConsumer(long Id,int & iCount)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;

    // SQL
    string sSql = "select count(*)";

    sSql +=  " from "+msTableName;

    string sCond = " where  ";

    sCond += "Id=" + tConvert.LongToStr(Id) + " and iCount>=3";


    sSql += sCond;

    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        if ( NULL != (row = mysql_fetch_row(result) )) {

            iCount =  atoi( row[0] );
            iRet = 0;

        }
    }

    FreeResult(result);

    return iRet;

}


/**
 * 获到公众号上的男性消费粉丝数
 * @param Id 公众号id
 * @return 0 sucess userDataLst 消费粉丝
 */
int PublicFansDAL::GetMaleConsumer(long Id ,int & iCount)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;
    iCount = 0;

    // SQL
    string sSql = "select count(*) from user_datadal as a inner join " +msTableName;
    sSql += " as b on a.lUserId=b.lFansId where a.iSex=1 and b.iCount>=3 and b.Id=" + tConvert.LongToStr(Id) ;


    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            iCount = atoi(row[0]);
            iRet = 0;
        }
    }

    FreeResult(result);

    return iRet;
}


/**
 * 获到公众号上的消费粉丝（昵称头像等）
 * @param Id 公众号id
 * @return 0 sucess userDataLst 消费粉丝
 */
int PublicFansDAL::GetConsumer(long Id,int iIndex ,int iCount,UserDataList & userDataLst)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;

    // SQL
    string sSql = "select a.lUserId,a.sNickName, a.sHeadImage,b.Id from user_datadal as a inner join " +msTableName;
    sSql += " as b on a.lUserId=b.lFansId where b.iCount>=3 and b.Id=" + tConvert.LongToStr(Id) ;
    sSql += " order by b.lAddTime desc limit " + tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            UserDataEntity entity;

            entity.lUserId = atol( row[iIndex++]);
            entity.sNickName = row[iIndex++];
            entity.sHeadImage =  row[iIndex++];

            userDataLst.push_back(entity);


            iRet = 0;


        }
    }

    FreeResult(result);

    return iRet;
}



/**
 * 获取近一段时间内的消费粉丝数
 * @param Id 公众号id
 * @param lTime
 * @return 0 sucess iCount 消费粉丝数
 */
int PublicFansDAL::GetCountsConsumer(long Id,long lTime ,int & iCount)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;

    // SQL
    string sSql = "select count(*) from " +msTableName;
    sSql += " where iCount>=3 and lAddTime >" + tConvert.LongToStr(lTime) + " and Id=" + tConvert.LongToStr(Id);

    //appendlog(TTDLogger::LOG_DEBUG, "xxxxxx:%s",sSql.c_str());


    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            iCount = atoi( row[0]);
            iRet = 0;
        }
    }

    FreeResult(result);

    return iRet;

}

