#include "UserDeviceDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


//更新
int UserDeviceDAL::replace( long  lUserId, const  string & deviceId  )
{
    TConvert tConvert;

    string sSql;
    // SQL
    sSql = "replace  "+this->msTableName+"("   +  msAllColumn +   ")" +
            "VALUES( '"+tConvert.LongToStr(lUserId)+"','" +
             mMulti.RealEscapeString( deviceId )+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int UserDeviceDAL::GetById(long id, UserDeviceEntity & entity)
{
    string sSql = " select " + msAllColumn + " from " +
            msTableName + " where userId='" + mtConvert.LongToStr(id) + "'";

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

int UserDeviceDAL::RowToEntity(MYSQL_ROW row ,UserDeviceEntity & entity)
{
    int iIndex = 0;
    entity.userId = atol(row[iIndex++]) ;
    entity.deviceId = row[iIndex++];

    return 0;
}

