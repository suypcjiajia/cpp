#include "HeadDal.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"






int HeadDAL::GetGroupId(LongLst & lGroupIds, int iIndex)
{

    TConvert tConvert;
    // SQL
    string sSql = "select lGroupId from group_data limit ";
    sSql += tConvert.IntToStr(iIndex);
    sSql += ",10";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                if (NULL == row[0]) {

                } else {
                    lGroupIds.push_back( atol(row[0]));
                }
                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return 0;

}


//一个群的所有成员(只包括群成员)
int HeadDAL::GetRealMemberById( long  lGroupId,LongLst & lMemberIds)
{
    TConvert tConvert;

    // SQL
    string sSql = "select lMemberId";


    sSql +=  " from group_member";

    string sCond = " where  ";
    sCond += " lGroupId= '" + tConvert.LongToStr( lGroupId) + "' and ";
    sCond += " iType= '" + tConvert.LongToStr( enum_membertype_member) + "'";
    sSql += sCond;

    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    int iRet = -1;

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex = 0;

            long lMemberId = atol( row[iIndex++]);

            LongLst::iterator it = lMemberIds.begin();
            bool b = false;
            for( ; it != lMemberIds.end() ; it++){
                 if( *it == lMemberId){
                    b = true ;
                    break;
                 }
            }
            if(!b)
                lMemberIds.push_back( lMemberId );

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}



int HeadDAL::GetUserHeadUrl(long lUserId, string & sHeadImage)
{

    TConvert tConvert;
    // SQL
    string sSql = "select sHeadImage from user_datadal" ;
    sSql += " where iState<>'333' ";

    sSql += " and sPrimaryKey = '" + tConvert.LongToStr( lUserId ) + "' ";



    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    int iRet = -1;

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex = 0;

            sHeadImage =   row[iIndex++];

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


int HeadDAL::UpGroupHead(long lGroupId, string sHeadUrl)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update group_data set lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sGroupHead ='" + mutlidal.RealEscapeString(sHeadUrl) + "'";

    sSql += " where lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";



    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}





