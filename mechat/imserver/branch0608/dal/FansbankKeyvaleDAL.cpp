#include "FansbankKeyvaleDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

int FansBankKeyvalueDAL::GetValue(string key, string & value){


    string sSql =   "select fansvalue from " + msTableName  + " where fanskey='" + mMulti.RealEscapeString( key ) + "'";

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

int FansBankKeyvalueDAL::SetValue(string key, string value)
{
    string sSql = "update "+ msTableName+" set fansvalue='" + mMulti.RealEscapeString(value)+ "' where fanskey='" +mMulti.RealEscapeString( key) + "'" ;

    return BaseQuery(sSql);

}


int FansBankKeyvalueDAL::GetAll( CommonStructVector & lst){


    string sSql =   "select fanskey,fansvalue from " + msTableName  ;

    MYSQL_RES* result = NULL;
    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {
            CommonStruct en;

            en.v1 = row[0] ;
            en.v2 = row[1] ;
            lst.push_back(en);
        }
    }
    FreeResult( result );

    return 0;
}

