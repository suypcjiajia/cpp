#include "UserDataDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//获取一批用户数据
int UserDataDAL::GetUserInfoList( const LongLongMap &  Ids, UserDataList & pList  )
{
    if( Ids.size() == 0){
        return 0;
    }
    TConvert tConvert;
    // SQL
    string sUsers;
    for( LongLongMap::const_iterator it = Ids.begin(); it != Ids.end(); it++){
        sUsers += tConvert.LongToStr(it->first);
        sUsers += ",";
    }
    sUsers = sUsers.substr(0, sUsers.length() - 1);
    string sSql =   "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
                    "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,sEmail,sHeadImage,lPushManId,lPlatformId,sGciID,lCurrentPlatformId,iPhoneAuth from " +
            this->msTableName + " where lUserId in(" + sUsers+ ") order by field(lUserId," + sUsers + ")";

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                UserDataEntity entity;

                int iIndex = 0;

                entity.sPrimaryKey =row[iIndex++];
                entity.lUserId = atol( row[iIndex++] );
                entity.lAddTime = atol( row[iIndex++] );
                entity.lUpTime = atol( row[iIndex++] );
                entity.iState = atoi( row[iIndex++] );
                entity.lUpdateId = atol( row[iIndex++] );

                entity.sAccount = row[iIndex++];
                entity.sPassword = row[iIndex++];
                entity.sNickName = row[iIndex++];
                entity.iSex = atoi( row[iIndex++] );
                entity.sProvince = row[iIndex++];
                entity.sCity = row[iIndex++];
                entity.sSignature = row[iIndex++];
                entity.sEmail = row[iIndex++];
                entity.sHeadImage = row[iIndex++];
                entity.lPushManId = atol( row[iIndex++] );
                entity.lPlatformId = atol( row[iIndex++] );
                entity.sGciID = row[iIndex++];
                entity.lCurrentPlatformId = atol(row[iIndex++]);
                entity.iPhoneAuth = atoi( row[iIndex++] );


                pList.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;
}


int UserDataDAL::Get100FansPlatfrom(  longs & pList  )
{
    string sSql = "select lCurrentPlatformId  from user_datadal group by lCurrentPlatformId having count(*) >=10";
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {

                int iIndex = 0;

                long lCurrentPlatformId =atol( row[iIndex++] );

                pList.push_back(lCurrentPlatformId);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;



}


int UserDataDAL::Get100User(  long lLastId, longs & pList  )
{
    TConvert tConvert;
    string sSql = "select lUserId  from user_datadal where lUserId>" + tConvert.LongToStr(lLastId) + " limit 100";
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {

                int iIndex = 0;

                pList.push_back(atol( row[iIndex++] ));

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int UserDataDAL::LetBindWeixinHot()
{
    string sSql = "update user_datadal set iHot=1 where sWeixinNum!=''";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int UserDataDAL::LetPerfectInfo2Hot()
{
    string sSql = "update user_datadal set iHot=1 where sBirthDay>'1900' and iSex<=1 and sHeadImage!='' and sCity!=''"
                  " and sProvince!='' and sNickName!=''";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int UserDataDAL::LetConsume2Hot(string sWhere)
{
    string sSql = "update user_datadal set iHot=1 where " + sWhere;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}



int UserDataDAL::Get100FansPush(  longs & pList  )
{
    string sSql = "select lPushManId  from user_datadal group by lPushManId having count(*) >=10";
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {

                int iIndex = 0;

                long lCurrentPlatformId = atol(row[iIndex++] );

                pList.push_back(lCurrentPlatformId);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

int UserDataDAL::GetUserIdByPhone(const string & sPhone, long & lUserId,long & lCurrentPlatformId)
{

    TMultiMysqlDAL multi;

    string sSql =   "select lUserId,lCurrentPlatformId from " + this->msTableName + " where sAccount='" + multi.RealEscapeString( sPhone ) + "'";

    int iRet = -1;
    MYSQL_RES* result = NULL;

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
            if( NULL != row ) {
                lUserId = atol(row[0]);
                lCurrentPlatformId = atol(row[1]);
            }
        }
    }
    if( result != NULL)
        con->FreeResult( result );

    return iRet;

}


//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityById(long lUserId,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;TConvert tConvert;
    // SQL
    string sSql = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
                  "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,sEmail,"
                  "sHeadImage,iPhoneAuth,lPushManId,lPlatformId,lCurrentPlatformId,sGciID,sGciPwd,sGciPhone,sQqNum,sWeixinNum"
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";


    sSql += " and lUserId = '" + tConvert.LongToStr(lUserId )+ "' ";

    return ExcutEntity(sSql,pEntity);

}



int UserDataDAL::ExcutEntity(const string  & sSql, UserDataEntity& pEntity){

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
        if ( NULL != (row = mysql_fetch_row( result ))) {
            iRet = 0;


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
            pEntity.sQqNum = row[iIndex++];
            pEntity.sWeixinNum = row[iIndex++];



        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}
