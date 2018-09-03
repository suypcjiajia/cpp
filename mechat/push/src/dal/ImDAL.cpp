#include "ImDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//更新chat状态(已读)
int ImDAL::UpdateChatState(const string &  psKey, int piState)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update private_chatdal  set  lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",iState ='" + tConvert.IntToStr(piState) + "'";
    sSql += " where sKey='"+ multi.RealEscapeString( psKey ) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//更新groupchat已读用户
int ImDAL::UpdateGroupChatReaded(const string &  psKey, long  lUserId)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update group_chat  set  lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",sReaded =concat(sReaded,'" + tConvert.LongToStr(lUserId) + ",')";
    sSql += " where sKey='"+ multi.RealEscapeString( psKey ) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//更新publicchat已读用户
int ImDAL::UpdatePublicChatReaded(const string &  psKey, long  lUserId)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "update public_info  set  sReaded=concat(sReaded,'" + tConvert.LongToStr(lUserId) + ",')";
    sSql += " where sKey='"+ multi.RealEscapeString( psKey ) + "'";


    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}


int ImDAL::GetUserInfo(const long & lUserId, UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,sEmail,sHeadImage,iPhoneAuth,lPushManId,lPlatformId,lCurrentPlatformId,sGciID,sGciPwd,sGciPhone"
                  " from user_datadal " ;
    sSql += " where iState<>'333' ";

    sSql += " and lUserId = '" + tConvert.LongToStr(lUserId) + "' ";


    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
             iRet = 0;
            while( NULL != row ) {

                int iIndex = 0;

                pEntity.sPrimaryKey = row[iIndex++];
                pEntity.lUserId = atol( row[iIndex++] );
                pEntity.lAddTime = atol( row[iIndex++] );
                pEntity.lUpTime = atol( row[iIndex++] );
                pEntity.iState = atoi( row[iIndex++] );
                pEntity.lUpdateId = atol( row[iIndex++] );
                pEntity.iSync = atoi( row[iIndex++] );

                pEntity.sAccount = row[iIndex++];
                pEntity.sPassword = row[iIndex++];
                pEntity.sNickName = row[iIndex++];
                pEntity.iSex = atoi( row[iIndex++] );
                pEntity.sProvince = row[iIndex++];
                pEntity.sCity = row[iIndex++];
                pEntity.sSignature = row[iIndex++];
                pEntity.sEmail = row[iIndex++];
                pEntity.sHeadImage = row[iIndex++];
                pEntity.iPhoneAuth = atoi( row[iIndex++] );
                pEntity.lPushManId = atol( row[iIndex++] );
                pEntity.lPlatformId = atol( row[iIndex++] );
                pEntity.lCurrentPlatformId = atol( row[iIndex++] );
                pEntity.sGciID = row[iIndex++];
                pEntity.sGciPwd = row[iIndex++];
                pEntity.sGciPhone = row[iIndex++];

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int ImDAL::GetMsgMode(long lUserId, long lGroupId, int & iMsgMode)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    // SQL
    string sSql = "select iMsgMode"
                  " from group_member " ;
    sSql += " where lMemberId='"+ tConvert.LongToStr(lUserId)  + "'";
    sSql += " and  lGroupId='"+ tConvert.LongToStr(lGroupId)  + "'";


    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row  = mysql_fetch_row( result ) )) {
             iRet = 0;

             int iIndex = 0;
             iMsgMode = atoi( row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

