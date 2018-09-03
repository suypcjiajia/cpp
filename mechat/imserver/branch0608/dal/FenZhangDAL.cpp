

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

#include "dal/FenZhangDAL.h"


int FenZhangDAL::Insert( FenZhangEntiy& entity  )
{
    string sSql;
    sSql = "insert into "+ msTableName+" (" + msAllColumn +  ")" +
            "VALUES( '"+mMulti.RealEscapeString(entity.orderNo)+"','" +
            entity.addTime+"','" +
            mMulti.RealEscapeString(entity.info)+
            "')";

    return BaseQuery(sSql);
}



int FenZhangDAL::Fetch( string sSql ,FenZhangList & lst)
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
                FenZhangEntiy entity;
                RowToEntity(row,entity);
                lst.push_back(entity);
        }
    }

    con->FreeResult( result );

    return iRet;

}


int FenZhangDAL::RowToEntity(MYSQL_ROW row ,FenZhangEntiy & entity)
{
    int iIndex = 0;
    entity.orderNo = row[iIndex++] ;
    entity.addTime = row[iIndex++] ;
    entity.info = atol(row[iIndex++]);

    return 0;
}
