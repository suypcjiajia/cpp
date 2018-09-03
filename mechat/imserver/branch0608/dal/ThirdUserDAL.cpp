

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

#include "dal/ThirdUserDAL.h"


int ThirdUserDAL::Insert( ThirdUserEntity& entity  )
{
    string sSql;
    sSql = "replace into "+ msTableName+" (" + msAllColumn +  ")" +
            "VALUES( '"+entity.addTime+"','" +
            mtConvert.IntToStr(entity.sex)+"','" +
            mMulti.RealEscapeString(entity.headImage)+"','" +
            mtConvert.LongToStr(entity.pushManId)+"','" +
            mtConvert.LongToStr(entity.platformId)+"','" +
            mMulti.RealEscapeString(entity.nickName)+"','" +
            mMulti.RealEscapeString(entity.id)+"','" +
            mtConvert.IntToStr( entity.type)+
            "')";

    return BaseQuery(sSql);
}

int ThirdUserDAL::GetById(string id, ThirdUserEntity & entity)
{
    string sSql = " select " + msAllColumn + " from " +
            msTableName + " where id='" + mMulti.RealEscapeString(id) + "'";

    MYSQL_RES* result;
    BaseQueryResult(sSql,&result);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                RowToEntity(row,entity);
        }
    }

    FreeResult( result );

    return iRet;
}




int ThirdUserDAL::Fetch( string sSql ,ThirdUserLst & lst)
{


    MYSQL_RES* result;
    BaseQueryResult(sSql,&result);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                ThirdUserEntity entity;
                RowToEntity(row,entity);
                lst.push_back(entity);
        }
    }

    FreeResult( result );

    return iRet;

}


int ThirdUserDAL::RowToEntity(MYSQL_ROW row ,ThirdUserEntity & entity)
{
    int iIndex = 0;
    entity.addTime = row[iIndex++] ;
    entity.sex = atoi(row[iIndex++] );
    entity.headImage = row[iIndex++] ;
    entity.pushManId = atol(row[iIndex++]);
    entity.platformId  =  atol(row[iIndex++]);
    entity.nickName = row[iIndex++];
    entity.id =  row[iIndex++] ;
    entity.type =atoi( row[iIndex++]) ;

    return 0;
}
