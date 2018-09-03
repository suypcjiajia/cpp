

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

#include "dal/CommunityRemarkDAL.h"


int CommunityRemarkDAL::Insert( CommunityRemarkEntiy& entity  )
{
    string sSql;
    sSql = "insert into "+ msTableName+" (" + msAllColumn +  ")" +
            "VALUES( '"+entity.id+"','" +
            mMulti.RealEscapeString(entity.publishId)+"','" +
            mtConvert.LongToStr(entity.userId)+"','" +
            mMulti.RealEscapeString(entity.nickName)+"','" +
            mMulti.RealEscapeString(entity.headImage)+"','" +
            entity.addTime+"','" +
            mMulti.RealEscapeString(entity.comment)+"','" +
            mtConvert.IntToStr( entity.type)+"','" +
            mtConvert.LongToStr(entity.touserId)+"','" +
            mMulti.RealEscapeString(entity.tonickName)+"','" +
            mMulti.RealEscapeString(entity.toheadImage)+
            "')";

    return BaseQuery(sSql);
}

int CommunityRemarkDAL::GetPage(string id,int page, CommunityRemarkList & lst,int & pagecount)
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select "  + msAllColumn + " from " + msTableName ;
    string sCondition = " where publishId=" + mMulti.RealEscapeString(id) ;
    sSql += sCondition;
    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int sum = 0;
    GetCount(sCondition,sum);
    pagecount = sum/count + 1;


    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

int CommunityRemarkDAL::ToMeHudong(long userId,int page, CommunityRemarkList & lst,int & pagecount)
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select a.id,a.publishId,a.userId,a.nickName,a.headImage,a.addTime,"
                   "a.comment,a.type,a.touserId,a.tonickName,a.toheadImage from "
            + msTableName + " as a inner join community_publish as b ";
    string sCondition = " where a.publishId=b.id and b.userId=" + mtConvert.LongToStr(userId) +
            " and whoClear not like '%" + mtConvert.LongToStr(userId) + "%' ";
    sSql += sCondition;
    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int sum = 0;
    GetCount(sCondition,sum);
    pagecount = sum/count + 1;

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

int CommunityRemarkDAL::ClearHudong(long userId)
{
    string sSql = "update " + msTableName ;
    sSql += " set whoClear =concat(whoClear,'" + mtConvert.LongToStr(userId) + ",')";

    return BaseQuery(sSql);
}

int CommunityRemarkDAL::Fetch( string sSql ,CommunityRemarkList & lst)
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
                CommunityRemarkEntiy entity;
                RowToEntity(row,entity);
                lst.push_back(entity);
        }
    }

    con->FreeResult( result );

    return iRet;

}


int CommunityRemarkDAL::RowToEntity(MYSQL_ROW row ,CommunityRemarkEntiy & entity)
{
    int iIndex = 0;
    entity.id = row[iIndex++] ;
    entity.publishId = row[iIndex++] ;
    entity.userId =atol( row[iIndex++]) ;
    entity.nickName = row[iIndex++];
    entity.headImage = row[iIndex++];
    entity.addTime = row[iIndex++];
    entity.comment = row[iIndex++] ;
    entity.type =atoi( row[iIndex++]) ;
    entity.touserId =atol( row[iIndex++]) ;
    entity.tonickName = row[iIndex++];
    entity.toheadImage = row[iIndex++];
    return 0;
}
