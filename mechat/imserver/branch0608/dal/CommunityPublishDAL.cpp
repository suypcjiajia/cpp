

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "CommunityPublishDAL.h"

int CommunityPublishDAL::Insert(const CommunityPublishEntiy& entity  )
{
    string sSql;
    sSql = "insert into "+ msTableName+" (" + msAllColumn +  ")" +
            "VALUES( '"+entity.id+"','" +
            mtConvert.LongToStr(entity.userId)+"','" +
            mMulti.RealEscapeString(entity.nickName)+"','" +
            mMulti.RealEscapeString(entity.headImage)+"','" +
            entity.addTime+"','" +
            mtConvert.IntToStr( entity.fineCount)+"','" +
            mtConvert.IntToStr(entity.markCount)+"','" +
            mtConvert.IntToStr(entity.collectCount)+"','" +
            mMulti.RealEscapeString(entity.content)+"','" +
            mMulti.RealEscapeString(entity.pic)+"','" +
            mtConvert.IntToStr(entity.authority)+
            "')";

    return BaseQuery(sSql);

}

int CommunityPublishDAL::GetEntity(string id,CommunityPublishEntiy& entity)
{
    string sSql =  "select "  + msAllColumn + " from " + msTableName ;
    sSql += " where id=" + mMulti.RealEscapeString(id);

    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                RowToEntity(row,entity);

        }
    }

    con->FreeResult( result );

    return iRet;
}

//分页获取首页圈子信息
int CommunityPublishDAL::GetShouYe(long userId,string friends,int page, CommunityPublishList & lst,int & pagecount)
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select "  + msAllColumn + " from " + msTableName ;
    string sCondition = " where (userId=" + mtConvert.LongToStr(userId) + ")";

    if(!friends.empty()){
        sCondition += " or (userId in(" + friends + ") and authority!=1)";
    }


    sSql += sCondition;
    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int sum = 0;
    GetCount(sCondition,sum);
    pagecount = sum/count + 1;



    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

//分页获取我发出的圈子信息
int CommunityPublishDAL::GetMyPuslish(long userId,int page,string condition,CommunityPublishList & lst,int & pagecount )
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select "  + msAllColumn + " from " + msTableName ;
    string sCondition = " where userId=" + mtConvert.LongToStr(userId) ;

    if( !condition.empty()){
        sCondition += " and content like '%" + mMulti.RealEscapeString(condition) + "%'";
    }

    sSql += sCondition;
    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int sum = 0;
    GetCount(sCondition,sum);
    pagecount = sum/count + 1;

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;
}

//分页获取收藏的圈子信息
int CommunityPublishDAL::GetCollection(long userId,int page,CommunityPublishList & lst,int & pagecount )
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select a.id,a.userId,a.nickName,a.headImage,a.addTime,a.fineCount,a.markCount,a.collectCount,a.content,a.pic,a.authority from " + msTableName + " as a inner join community_remark as b " ;
    string sCondition = " where a.id=b.publishId and b.userId=" + mtConvert.LongToStr(userId) ;
    sCondition += " and b.type=2";
    sSql +=  sCondition;

    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int sum = 0;
    string sQueryCount = "select count(*) from " + msTableName + " as a inner join community_remark as b " ;
    sQueryCount += sCondition;
    GetQueryTotalCount(sQueryCount,sum);
    pagecount = sum/count + 1;

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;
}


int CommunityPublishDAL::Fetch( string sSql ,CommunityPublishList & lst)
{


    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                CommunityPublishEntiy entity;
                RowToEntity(row,entity);
                lst.push_back(entity);
        }
    }

    con->FreeResult( result );

    return iRet;

}


int CommunityPublishDAL::RowToEntity(MYSQL_ROW row ,CommunityPublishEntiy & entity)
{
    int iIndex = 0;
    entity.id = row[iIndex++] ;
    entity.userId =atol( row[iIndex++]) ;
    entity.nickName = row[iIndex++];
    entity.headImage = row[iIndex++];
    entity.addTime = row[iIndex++];
    entity.fineCount =atoi( row[iIndex++]) ;
    entity.markCount =atoi( row[iIndex++]) ;
    entity.collectCount =atoi( row[iIndex++]) ;
    entity.content = row[iIndex++];
    entity.pic = row[iIndex++];
    entity.authority =atoi( row[iIndex++]) ;
    return 0;
}
