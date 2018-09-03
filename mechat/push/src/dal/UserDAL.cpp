#include "UserDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "dal/RedisConnect.h"
#include "Base64EnDecode.h"


const   string UserDAL::sPushIp = "PushIp";
const   string UserDAL::sPhoneType = "PhoneType";
const   string UserDAL::sToken = "Token";
const string UserDAL::sTime = "Time";

//创建表
int UserDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "sPushIp VARCHAR(30) NOT NULL,"+
            "iPhoneType INT NOT NULL,"+
            "sToken VARCHAR(128) NOT NULL,"+
            "PRIMARY KEY(lUserId)"+
            ") ENGINE=NDBCluster DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//更新连接用户信息
int UserDAL::Replace( PushUserEntity& pEntity  )
{
    TConvert tConvert;

    RedisConnect::GetInstance()->Hset(tConvert.LongToStr(pEntity.lUserId),UserDAL::sPushIp,pEntity.sPushIp);
    RedisConnect::GetInstance()->Hset(tConvert.LongToStr(pEntity.lUserId),UserDAL::sPhoneType,tConvert.IntToStr(pEntity.iPhoneType));
    Base64EnDecode base64;
    string sBaseToken = base64.Base64Encode((const unsigned char*)pEntity.sToken.c_str() , pEntity.sToken.length());
    RedisConnect::GetInstance()->Hset(tConvert.LongToStr(pEntity.lUserId),UserDAL::sToken,sBaseToken);
    RedisConnect::GetInstance()->Hset(tConvert.LongToStr(pEntity.lUserId),UserDAL::sTime,tConvert.LongToStr(pEntity.lAddTime/1000000));

    return 0;
//    string sSql;
//    // SQL
//    sSql = "replace into "+this->msTableName+"( lUserId,lAddTime,sPushIp,iPhoneType,sToken )" +
//            "VALUES( '"+
//            tConvert.LongToStr(pEntity.lUserId)+"','" +
//            tConvert.LongToStr(pEntity.lAddTime)+"','" +
//            pEntity.sPushIp+"','" +
//            tConvert.IntToStr(pEntity.iPhoneType)+"','" +
//            pEntity.sToken+
//            "')";
//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -1;
//    }
//    return con->Query(sSql);
}


int UserDAL::GetPushIp(long lUserId,string & sPushIp)
{

    TConvert tConvert;

    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(lUserId),UserDAL::sPushIp,sPushIp);
    return 0;
}

int UserDAL::GetToken(long lUserId,string & sToken)
{
    TConvert tConvert;

    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(lUserId),UserDAL::sToken,sToken);
    Base64EnDecode base64;int baselen;
    sToken = base64.Base64Decode(sToken.c_str(),sToken.length(),baselen);
    return 0;

}

int UserDAL::GetType(long lUserId,int & iPhoneType)
{
    TConvert tConvert;

    string sTmp;
    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(lUserId),UserDAL::sPhoneType,sTmp);
    iPhoneType = tConvert.StrToInt(sTmp);
    return 0;
}

//查询连接用户信息
int UserDAL::GetEntity(const long plUserId, PushUserEntity& pEntity )
{
    TConvert tConvert;

    string sTmp;
    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(plUserId),UserDAL::sPushIp,pEntity.sPushIp);
    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(plUserId),UserDAL::sPhoneType,sTmp);
    pEntity.iPhoneType = tConvert.StrToInt(sTmp);
    RedisConnect::GetInstance()->Hget(tConvert.LongToStr(plUserId),UserDAL::sToken,pEntity.sToken);
    Base64EnDecode base64;int baselen;
    pEntity.sToken = base64.Base64Decode(pEntity.sToken.c_str(),pEntity.sToken.length(),baselen);
    pEntity.lUserId = plUserId;
    return 0;
    // SQL
//    string sSql = "select lUserId,lAddTime,sPushIp,iPhoneType,sToken "
//                  " from " + this->msTableName + " ";
//    sSql += " where lUserId = '" + tConvert.LongToStr(plUserId) + "' ";

//    int iRet = -1;
//    MYSQL_RES* result;
//    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
//    if( con == NULL){
//        return -1;
//    }
//    result =  con->QueryResult(sSql);

//    if ( NULL != result ) {
//        iRet = 0;
//        MYSQL_ROW row = NULL;
//        row = mysql_fetch_row( result );
//        if ( NULL != row ) {
//            while( NULL != row ) {

//                int iIndex = 0;

//                pEntity.lUserId = atol( row[iIndex++] );
//                pEntity.lAddTime = atol( row[iIndex++] );
//                pEntity.sPushIp = row[iIndex++];
//                pEntity.iPhoneType = atoi(row[iIndex++]);
//                pEntity.sToken = row[iIndex++];

//                row = mysql_fetch_row( result );
//            }
//        }
//    }
//    // 释放内存
//    con->FreeResult( result );

//    return iRet;

}
