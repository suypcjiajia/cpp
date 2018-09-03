

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

#include "dal/ThirdpayDAL.h"


int ThirdpayDAL::Insert( ThirdpayEntiy& entity  )
{
    string sSql;
    sSql = "insert into "+ msTableName+" (" + msAllColumn +  ")" +
            "VALUES( '"+entity.id+"','" +
            mMulti.RealEscapeString(entity.openid)+"','" +
            mMulti.RealEscapeString(entity.phone)+"','" +
            mtConvert.LongToStr(entity.platformId)+"','" +
            mtConvert.LongToStr(entity.pushId)+"','" +
            entity.addTime+"','" +
            mtConvert.DoubleToStr(entity.money)+"','" +
            mtConvert.IntToStr( entity.thirdType)+
            "')";

    return BaseQuery(sSql);
}


int ThirdpayDAL::GetByPhone(string sPhone, int page,ThirdpayList & lst)
{
    int count = 10;
    int index = (page-1)*count;
    string sSql =  "select "  + msAllColumn + " from" + msTableName ;
    sSql += " where phone=" + mMulti.RealEscapeString(sPhone );
    sSql += " order by addTime desc limit " + mtConvert.IntToStr(index) + "," + mtConvert.IntToStr(count);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

int ThirdpayDAL::Fetch( string sSql ,ThirdpayList & lst)
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
                ThirdpayEntiy entity;
                RowToEntity(row,entity);
                lst.push_back(entity);
        }
    }

    con->FreeResult( result );

    return iRet;

}


int ThirdpayDAL::RowToEntity(MYSQL_ROW row ,ThirdpayEntiy & entity)
{
    int iIndex = 0;
    entity.id = row[iIndex++] ;
    entity.openid = row[iIndex++] ;
    entity.phone = row[iIndex++] ;
    entity.platformId = atol(row[iIndex++]);
    entity.pushId  =  atol(row[iIndex++]);
    entity.addTime = row[iIndex++];
    entity.money = atof( row[iIndex++] );
    entity.thirdType =atoi( row[iIndex++]) ;

    return 0;
}
