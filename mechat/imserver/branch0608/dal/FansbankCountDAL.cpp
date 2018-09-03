#include "FansbankCountDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

int FansBankCountDAL::GetValueForLock(string id,string type, string & value){


    string sSql =   "select count from " + msTableName  + " where id=" + mMulti.RealEscapeString( id ) + " and " +
            " type=" + mMulti.RealEscapeString( type ) + " for update ";

    int iRet = -1;
    MYSQL_RES* result = NULL;
    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                value = row[0] ;
        }
    }
    FreeResult( result );

    return iRet;
}

int FansBankCountDAL::SetValue(string id,string type, string value)
{
    string sSql = "update "+ msTableName+" set count='" + mMulti.RealEscapeString(value)+ " where id='" +mMulti.RealEscapeString( id) + " and " +
            " type=" + mMulti.RealEscapeString( type );

    return BaseQuery(sSql);

}
