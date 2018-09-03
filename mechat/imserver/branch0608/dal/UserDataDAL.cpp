#include "UserDataDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "KVlaueEntity.h"
#include "TypeDef.h"

UserDataDAL::UserDataDAL(){
    msTableName = "user_datadal";
    msAllColumn = "select sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,"
                  "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,"
                  "sEmail,sHeadImage,lPushManId,lPlatformId,sGciID,lCurrentPlatformId,"
                  "iPhoneAuth,sQqNum,sWeixinNum,sAliNum from ";
    msAll = " sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync,"
            "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,sEmail,"
            "sHeadImage,iPhoneAuth,lPushManId,lPlatformId,lCurrentPlatformId,sGciID,sGciPwd,sGciPhone,sQqNum,sWeixinNum,sAliNum,sBirthDay ";
}

//创建表
int UserDataDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sPrimaryKey VARCHAR(50) NOT NULL,"+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT NOT NULL,"+
            "lUpdateId BIGINT NOT NULL,"+
            "iSync INT NOT NULL,"+

            "sAccount VARCHAR(50) NOT NULL,"+
            "sPassword VARCHAR(50) NOT NULL,"+
            "sNickName VARCHAR(50) NOT NULL,"+
            "iSex INT NOT NULL,"+
            "sProvince VARCHAR(50) NOT NULL,"+
            "sCity VARCHAR(50) NOT NULL,"+
            "sSignature VARCHAR(50) NOT NULL,"+
            "sEmail VARCHAR(50) NOT NULL,"+
            "sHeadImage VARCHAR(200) NOT NULL,"+
            "iPhoneAuth INT NOT NULL default 0,"+
            "lPushManId BIGINT NOT NULL default 0,"+
            "lPlatformId BIGINT NOT NULL default 0,"+
            "lCurrentPlatformId BIGINT NOT NULL default 0,"+
            "sQqNum VARCHAR(50) NOT NULL default '',"+
            "sWeixinNum VARCHAR(50) NOT NULL default '',"+
            "sAliNum VARCHAR(50) NOT NULL default '',"+
            "sGciID VARCHAR(50) NOT NULL default '',"+
            "sGciPwd VARCHAR(50) NOT NULL default '',"+
            "sGciPhone VARCHAR(50) NOT NULL default '',"+
            "sBirthDay DATETIME NOT NULL default '1900-01-01 00:00:00',"+
            "PRIMARY KEY(sPrimaryKey),UNIQUE KEY (lUserId),UNIQUE KEY (sAccount)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//添加
int UserDataDAL::Insert( UserDataEntity& pEntity  )
{
    TConvert tConvert;
    pEntity.iState = 0;
    pEntity.iSync = 0;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
            "sAccount,sPassword,sNickName,iSex,sProvince,sCity,sSignature,sEmail,sHeadImage,iPhoneAuth,lPushManId,lPlatformId, lCurrentPlatformId,"
            "sQqNum,sWeixinNum,sAliNum,sGciID,sGciPwd,sGciPhone,iHot,sBirthDay)" +
            "VALUES( '"+pEntity.sPrimaryKey+"','" +
            tConvert.LongToStr(pEntity.lUserId)+"','" +
            tConvert.LongToStr(pEntity.lAddTime)+"','" +
            tConvert.LongToStr(pEntity.lUpTime)+"','" +
            tConvert.IntToStr(pEntity.iState)+"','" +
            tConvert.LongToStr(pEntity.lUpdateId)+"','" +
            tConvert.IntToStr(pEntity.iSync)+"','" +

            tMultiMysqlDAL.RealEscapeString(pEntity.sAccount)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sPassword)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sNickName)+"','" +
            tConvert.IntToStr(pEntity.iSex)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sProvince)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sCity)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sSignature)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sEmail)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sHeadImage)+"','" +
            tConvert.IntToStr(pEntity.iPhoneAuth)+"','" +
            tConvert.LongToStr(pEntity.lPushManId)+"','" +
            tConvert.LongToStr(pEntity.lPlatformId)+"','" +
            tConvert.LongToStr(pEntity.lCurrentPlatformId)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sQqNum)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sWeixinNum)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sAliNum)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sGciID)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sGciPwd)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sGciPhone)+"','" +
            tConvert.IntToStr(pEntity.iHot)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sBirthDay)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//删除
int UserDataDAL::Delete( const string& psPrimaryKey )
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL tMultiMysqlDAL;

    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",iState = '333' ";
    sSql += "where sPrimaryKey = '"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }

    return con->Query(sSql);
}
//修改
int UserDataDAL::Update(  const string& sPrimaryKey,const UserDataEntity& entity )
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    if ( entity.sAccount != "-1nochange" ) {
        sSql += ",sAccount = '" + tMultiMysqlDAL.RealEscapeString(entity.sAccount) + "' ";
    }
    if ( entity.sPassword != "-1nochange" ) {
        sSql += ",sPassword = '" + tMultiMysqlDAL.RealEscapeString(entity.sPassword) + "' ";
    }
    if ( entity.sNickName != "-1nochange" ) {
        sSql += ",sNickName = '" + tMultiMysqlDAL.RealEscapeString(entity.sNickName) + "' ";
    }
    if ( entity.iSex != -1 ) {
        sSql += ",iSex = '" + tConvert.IntToStr(entity.iSex) + "' ";
    }
    if ( entity.sProvince != "-1nochange" ) {
        sSql += ",sProvince = '" + tMultiMysqlDAL.RealEscapeString(entity.sProvince) + "' ";
    }
    if ( entity.sCity != "-1nochange" ) {
        sSql += ",sCity = '" + tMultiMysqlDAL.RealEscapeString(entity.sCity) + "' ";
    }
    if ( entity.sSignature != "-1nochange" ) {
        sSql += ",sSignature = '" + tMultiMysqlDAL.RealEscapeString(entity.sSignature) + "' ";
    }
    if ( entity.sEmail != "-1nochange" ) {
        sSql += ",sEmail = '" + tMultiMysqlDAL.RealEscapeString(entity.sEmail) + "' ";
    }
    if ( entity.sHeadImage != "-1nochange" ) {
        sSql += ",sHeadImage = '" + tMultiMysqlDAL.RealEscapeString(entity.sHeadImage) + "' ";
    }
    if ( entity.iPhoneAuth != -1 ) {
        sSql += ",iPhoneAuth = '" + tConvert.IntToStr(entity.iPhoneAuth) + "' ";
    }
    if (  entity.sGciID != "-1nochange" ) {
        sSql += ",sGciID = '" + entity.sGciID + "' ";
    }
    if ( entity.lCurrentPlatformId != -1 ) {
        sSql += ",lCurrentPlatformId = '" + tConvert.LongToStr(entity.lCurrentPlatformId) + "' ";
    }
    if (  entity.sBirthDay != "-1nochange" ) {
        sSql += ",sBirthDay='" +tMultiMysqlDAL.RealEscapeString( entity.sBirthDay) + "' ";
    }
    sSql += "where sPrimaryKey = '"+tMultiMysqlDAL.RealEscapeString(sPrimaryKey)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

int UserDataDAL::ModifyPwd(const string& psPrimaryKey, const string & sNewPwd)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId='" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",sPassword='" + tMultiMysqlDAL.RealEscapeString(sNewPwd) + "' "+
            ",lUpTime='" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += "where sPrimaryKey = '"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//修改帐号
int UserDataDAL::UpdateAccount( const string & psAccount, const string & psNewAccount)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",sAccount = '" + tMultiMysqlDAL.RealEscapeString(psNewAccount) + "' ";
    sSql += "where sAccount = '"+tMultiMysqlDAL.RealEscapeString( psAccount)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//获取最大用户ID
long UserDataDAL::GetMaxUserId()
{
    long lRet = -1;
    // SQL
    string sSql = "select max(lUserId) from "+this->msTableName+" ;";

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
                    lRet = 0;
                } else {
                    lRet = atol(row[0]);
                }
                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return lRet;
}
//记录是否存在
int UserDataDAL::IsExist( const string & psPrimaryKey, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sPrimaryKey='"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}
//记录是否存在
int UserDataDAL::IsExistByAccont( const string& psAccount, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sAccount='"+tMultiMysqlDAL.RealEscapeString(psAccount)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}
//记录是否存在-帐号密码
int UserDataDAL::IsExistByAccontPassword( const string& psAccount,const string& psPassword, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sAccount='"+tMultiMysqlDAL.RealEscapeString(psAccount)+"' and sPassword='"+tMultiMysqlDAL.RealEscapeString( psPassword)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}
//记录是否存在-主键帐号密码
int UserDataDAL::IsExistByPrimaryKeyAccontPassword( const string& psPrimaryKey, const string& psAccount,const string& psPassword, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sPrimaryKey='"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"' and sAccount='"+tMultiMysqlDAL.RealEscapeString(psAccount)+"' and sPassword='"+tMultiMysqlDAL.RealEscapeString(psPassword)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}


//记录是否存在-用户ID
int UserDataDAL::IsExistByUserId( const string& psUserId, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lUserId='"+tMultiMysqlDAL.RealEscapeString(psUserId)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}
//登录
int UserDataDAL::IsLogin( const string& psAccont, const string& psPassword, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sAccount='"+tMultiMysqlDAL.RealEscapeString(psAccont)+"' and sPassword='"+tMultiMysqlDAL.RealEscapeString(psPassword)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}
//查询一行数据
//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntity(const string& psPrimaryKey,const string& psAccount,const string& psGciID, UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";
    if ( psPrimaryKey != "-1nochange" ) {
        sSql += " and sPrimaryKey = '" + tMultiMysqlDAL.RealEscapeString(psPrimaryKey) + "' ";
    }
    if ( psAccount != "-1nochange" ) {
        sSql += " and sAccount = '" + tMultiMysqlDAL.RealEscapeString(psAccount) + "' ";
    }
    if ( psGciID != "-1nochange" ) {
        sSql += " and sGciID = '" + tMultiMysqlDAL.RealEscapeString(psGciID )+ "' ";
    }

    return ExcutEntity(sSql,pEntity);

}

//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityByAccount(const string& sAccount,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";


    sSql += " and sAccount = '" + tMultiMysqlDAL.RealEscapeString(sAccount )+ "' ";

    return ExcutEntity(sSql,pEntity);
}

//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityById(long lUserId,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;TConvert tConvert;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";


    sSql += " and lUserId = '" + tConvert.LongToStr(lUserId )+ "' ";

    return ExcutEntity(sSql,pEntity);

}

//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityByQqNum(const string& sQqNum,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";


    sSql += " and sQqNum = '" + tMultiMysqlDAL.RealEscapeString(sQqNum )+ "' ";

    return ExcutEntity(sSql,pEntity);
}

//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityByWeixinNum(const string& sWeixinNum,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";

    sSql += " and sWeixinNum = '" + tMultiMysqlDAL.RealEscapeString(sWeixinNum )+ "' ";

    return ExcutEntity(sSql,pEntity);
}


//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntityBysAliNum(const string& sAliNum,UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";

    sSql += " and sAliNum = '" + tMultiMysqlDAL.RealEscapeString(sAliNum )+ "' ";

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
            pEntity.sAliNum = row[iIndex++];
            pEntity.sBirthDay = row[iIndex++];
            if( pEntity.sBirthDay.substr(0,4) < "1900"){
                pEntity.sBirthDay = "1900-01-01 00:00:00";
            }



        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}




//根据用户获取我的推送人数
int UserDataDAL::GetMyPushCount(const string& psPushManId, long& plCount)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select count(*)"
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";
    sSql += " and lPushManId = '" +tMultiMysqlDAL.RealEscapeString( psPushManId) + "' ";

    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        MYSQL_ROW row = mysql_fetch_row( result );
        plCount = atol( row[0] );
    }

    // 释放内存
    con->FreeResult( result );

    return 0;

}

//获取粉丝人数（根据平台号或推广人）
int UserDataDAL::GetFansNum(long Id, int iType,int & iCount)
{

    string sSql = "select count(*)"
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";

    if( iType == enum_kvalue_type_push){
        sSql += " and lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and lCurrentPlatformId='" + mtConvert.LongToStr( Id) + "' ";
    }

    return GetQueryTotalCount(sSql,iCount);

}

//获取可交易（激活）粉丝人数（根据平台号或推广人）
int UserDataDAL::GetWeixinBindNum(long Id, int iType,int & iCount)
{

    string sSql = "select count(*)"
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' and iHot=1 ";

    if( iType == enum_kvalue_type_push){
        sSql += " and lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and lCurrentPlatformId='" + mtConvert.LongToStr( Id) + "' ";
    }

    return GetQueryTotalCount(sSql,iCount);

}

//获取粉丝人数GroupBy
int UserDataDAL::GetFansNumGroupBy(int iType,int iHot,LongLongMap & longlongMap)
{

    string column = "";
    if( iType == enum_kvalue_type_push){
        column = "lPushManId";
    }else{
        column = "lCurrentPlatformId";
    }

    string sSql = "select "  + column  + ",count(*) as count "
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' and iHot=" + mtConvert.IntToStr(iHot)  + " group by ";

    sSql += column;

    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

            longlongMap[atol(row[0] )]  = atol(row[1]);
        }
    }

    FreeResult( result );
    return 0;

}

//获取粉丝审核列表(某一天的)
int UserDataDAL::GetUnhotGroupById(int iType,int iPage, int iCount,string sDay,int iHot,CommonStructVector & lst, int & total)
{

    string column = "";
    if( iType == enum_kvalue_type_push){
        column = "lPushManId";
    }else{
        column = "lCurrentPlatformId";
    }

    string sSql = "select "  + column  + ",count(*) as countNum,iHot "
                  " from " + this->msTableName + " ";
    string where;
    where = " where iState<>'333'";

    where += " and iHot=" + mtConvert.IntToStr(iHot);

    where += " and substring(from_unixtime(lAddTime/1000000),1,10)='" +
            mMulti.RealEscapeString( sDay ) + "' group by ";

    where += column;
    sSql += where;
    sSql += " order by countNum desc ";
    sSql += " limit " + mtConvert.IntToStr((iPage-1)*iCount) + "," + mtConvert.IntToStr(iCount);

    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {
            CommonStruct en;int index = 0;
            en.v1 = row[index++];
            en.v2 = row[index++];
            en.v3 = row[index++];

            lst.push_back(en);

        }
    }

    GetCount(where,total);

    FreeResult( result );
    return 0;

}
//获取粉丝审核列表(所有日期>=2018-05-01)
int UserDataDAL::GetVerifyList(int iType,int iPage, int iCount,int iHot,CommonStructVector & lst, int & total)
{

    string column = "";
    if( iType == enum_kvalue_type_push){
        column = "lPushManId";
    }else{
        column = "lCurrentPlatformId";
    }

    string sSql = "select "  + column  + ",count(*) as countNum,iHot,substring(from_unixtime(lAddTime/1000000),1,10) as date "
                  " from " + this->msTableName + " ";
    string where;
    where = " where iState<>'333'";

    where += " and iHot=" + mtConvert.IntToStr(iHot);
   // where += " and substring(from_unixtime(lAddTime/1000000),1,10)>='2018-05-01'";

    where += " group by substring(from_unixtime(lAddTime/1000000),1,10),";
    where += column;
    sSql += where;
    sSql += " order by date desc,countNum desc ";
    sSql += " limit " + mtConvert.IntToStr((iPage-1)*iCount) + "," + mtConvert.IntToStr(iCount);

    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {
            CommonStruct en;int index = 0;
            en.v1 = row[index++];
            en.v2 = row[index++];
            en.v3 = row[index++];
            en.v4 = row[index++];

            lst.push_back(en);

        }
    }

    GetCountAterGroup(where,total);

    FreeResult( result );
    return 0;

}


//有消费过的粉丝人数
int UserDataDAL::GetConsumFansNum(long Id, int iType,int & iCount)
{
    string sSql = "select count(DISTINCT b.lFansId) from " + this->msTableName + " as a inner join public_fans as b";
    sSql += " where a.lUserId=b.lFansId and a.iState<>'333' and b.iCount>= 1";

    if( iType == enum_kvalue_type_push){
        sSql += " and a.lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and a.lCurrentPlatformId='" + mtConvert.LongToStr( Id) + "' ";
    }

    return GetQueryTotalCount(sSql,iCount);

}

//有消费过的粉丝
int UserDataDAL::GetConsumFans(string sSelPhone,long Id, int iType,int iIndex,int iNum,longs & lUserIds)
{
    string sSql = "select  b.lFansId from " + this->msTableName + " as a inner join public_fans as b";
    sSql += " where a.lUserId=b.lFansId and a.iState<>'333' and b.iCount>= 1";

    if( iType == enum_kvalue_type_push){
        sSql += " and a.lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and a.lCurrentPlatformId='" + mtConvert.LongToStr( Id) + "' and a.sAccount!=" + sSelPhone;
    }
    sSql +=  " group by b.lFansId order by b.iCount desc";


    sSql += " limit " + mtConvert.IntToStr(iIndex) + "," + mtConvert.IntToStr(iNum);

    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql, &result);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

            iRet = 0;
            int iIndex = 0;
            long lUserId = atol( row[iIndex++] );
            lUserIds.push_back(lUserId);

        }
    }
    // 释放内存
    FreeResult( result );

    return iRet;

}


//随机获取一批粉丝
int UserDataDAL::GetRandFans(string sSelPhone,long Id, int iType,int iNum,longs & lUserIds)
{
    string sSql = "select  lUserId from " + this->msTableName ;
    sSql += " where iState<>'333' and iHot=1 and sAccount!=" + sSelPhone;

    if( iType == enum_kvalue_type_push){
        sSql += " and lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and lCurrentPlatformId='" + mtConvert.LongToStr( Id)+ "' ";
    }
    sSql +=  " order by rand()";


    sSql += " limit "  + mtConvert.IntToStr(iNum);

    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql, &result);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

            iRet = 0;
            int iIndex = 0;
            long lUserId = atol( row[iIndex++] );
            lUserIds.push_back(lUserId);

        }
    }
    // 释放内存
    FreeResult( result );

    return iRet;

}


//获取一批粉丝
int UserDataDAL::GetFansv2(string sSelPhone,long Id, int iType,int iNum,longs & lUserIds)
{
    string sSql = "select  lUserId from " + this->msTableName ;
    sSql += " where iState<>'333' and iHot=1 and sAccount!=" + sSelPhone;

    if( iType == enum_kvalue_type_push){
        sSql += " and lPushManId='" + mtConvert.LongToStr( Id) + "' ";
    }
    else{
        sSql += " and lCurrentPlatformId='" + mtConvert.LongToStr( Id)+ "' ";
    }

    sSql += " limit "  + mtConvert.IntToStr(iNum);
    sSql += " for update";


    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql, &result);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

            iRet = 0;
            int iIndex = 0;
            long lUserId = atol( row[iIndex++] );
            lUserIds.push_back(lUserId);

        }
    }
    // 释放内存
    FreeResult( result );

    return iRet;

}


//根据用户获取我的推送且登陆过的人数
int UserDataDAL::GetMyPushAndLoginedCount( const string& psPushManId, long& plCount )
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select count(*) from user_datadal,user_statedal  "
                  "where  user_datadal.lUserId=user_statedal.lUserId and user_statedal.lLoginTime>0 and user_datadal.iState!=333";

    sSql += " and lPushManId = '" + tMultiMysqlDAL.RealEscapeString(psPushManId )+ "' ";

    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        MYSQL_ROW row = mysql_fetch_row( result );
        plCount = atol( row[0] );
    }

    // 释放内存
    con->FreeResult( result );

    return 0;

}


//根据用户获取all的推送人数
int UserDataDAL::GetBatchUserPushCount(const vector<long>  & plPushManId, long& plCount )
{
    // SQL
    if( plPushManId.size() == 0){
        plCount = 0;
        return 0;
    }
    TConvert tConvert;
    string sSql = "select count(*)"
                  " from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";
    sSql += " and lPushManId in (";
    for( vector<long>::const_iterator it = plPushManId.begin(); it != plPushManId.end() ; it++){
        long lUserId = *it;
        sSql += tConvert.LongToStr(lUserId) + ",";
    }
    sSql = sSql.substr(0, sSql.length() -1);
    sSql += ") ";


    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        MYSQL_ROW row = mysql_fetch_row( result );
        plCount = atol( row[0] );
    }

    // 释放内存
    con->FreeResult( result );

    return 0;

}


//获取平台粉丝
int UserDataDAL::PlatformFans( const long &  plPlatformId, vector<UserDataEntity>& pList )
{
    TConvert tConvert;
    // SQL
    string sSql =  msAllColumn + msTableName + " where lCurrentPlatformId=" + tConvert.LongToStr(plPlatformId);

    int iRet = -1;
    MYSQL_RES* result;
    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        iRet = 0;
        ResultToLst(result,pList);
    }

    FreeResult( result );

    return iRet;

}


//分页获取粉丝列表(可带查询条件)
int UserDataDAL::GetFans(  long Id,int iType,int iPage, int iCount,string sHot,long lUserId,string sNickName
                           , string sAccount, string sTimeStart,string sTimeEnd,
                           string sWeixinNum, string sAliNum,UserDataList & pList , int & iTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL tMulti;

    string sSql = msAllColumn + msTableName + " where  ";

    string sCondition;

    if( iType ==enum_kvalue_type_platform ){
        sCondition += " lCurrentPlatformId=" + tConvert.LongToStr(Id);
    }else {
        sCondition += " lPushManId=" + tConvert.LongToStr(Id);
    }

    if( !sHot.empty()){


       sCondition += " and iHot in(" + mMulti.RealEscapeString(sHot) + ")" ;


    }
    if( lUserId >= 0){
        sCondition += " and lUserId like '%" + tConvert.LongToStr(lUserId) + "%'";
    }
    if( sNickName != "null"){
        sCondition += " and sNickName like '%" + tMulti.RealEscapeString(sNickName) + "%'";
    }
    if( sAccount != "null"){
        sCondition += " and sAccount like '%" + tMulti.RealEscapeString(sAccount) + "%'";
    }
    if( sWeixinNum == "bind"){
        sCondition += " and sWeixinNum!=''";
    }else if(sWeixinNum == "nobind" ){
        sCondition += " and sWeixinNum=''";
    }
    if( sAliNum == "bind"){
        sCondition += " and sAliNum!=''";
    }else if(sAliNum == "nobind") {
        sCondition += " and sAliNum=''";
    }
    if( sTimeStart != "null" && sTimeEnd != "null"){
        long lTimeStart = tConvert.StrToTime(sTimeStart.c_str())*1000000;
        long lTimeEnd = tConvert.StrToTime(sTimeEnd.c_str())*1000000;
        sCondition += " and lAddTime >=" + tConvert.LongToStr(lTimeStart)  + " and lAddTime <="  + tConvert.LongToStr(lTimeEnd);
    }

    sSql = sSql + sCondition;
    sSql += " order by lAddTime desc ";
    sSql += " limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    MYSQL_RES* result;

    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        iRet = 0;
        ResultToLst(result,pList);
    }

    FreeResult( result );

    sSql = "select count(*) from " + msTableName + " where " + sCondition;
    GetQueryTotalCount(sSql,iTotal);

    return iRet;

}


//分页获取生产的粉丝列表(可带查询条件)
int UserDataDAL::GetMakedFans(  int iPage, int iCount,long lUserId,long lPlatformId,long lPushManId,string sNickName
                           , string sAccount, string sTimeStart,string sTimeEnd,
                           string sWeixinNum, string sAliNum,UserDataList & pList , int & iTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL tMulti;

    string sSql = msAllColumn + msTableName + " where 1=1 ";

    string sCondition;

    if( lUserId >= 0){
        sCondition += " and lUserId like '%" + tConvert.LongToStr(lUserId) + "%'";
    }
    if( lPlatformId >= 0){
        sCondition += " and lCurrentPlatformId like '%" + tConvert.LongToStr(lUserId) + "%'";
    }
    if( lPushManId >= 0){
        sCondition += " and lPushManId like '%" + tConvert.LongToStr(lUserId) + "%'";
    }
    if( sNickName != "null"){
        sCondition += " and sNickName like '%" + tMulti.RealEscapeString(sNickName) + "%'";
    }
    if( sAccount == "null"){
        sAccount ="";
    }
    sCondition += " and sAccount like '2%" + tMulti.RealEscapeString(sAccount) + "%'";
    if( sWeixinNum == "bind"){
        sCondition += " and sWeixinNum!=''";
    }else if(sWeixinNum == "nobind" ){
        sCondition += " and sWeixinNum=''";
    }
    if( sAliNum == "bind"){
        sCondition += " and sAliNum!=''";
    }else if(sAliNum == "nobind") {
        sCondition += " and sAliNum=''";
    }
    if( sTimeStart != "null" && sTimeEnd != "null"){
        long lTimeStart = tConvert.StrToTime(sTimeStart.c_str())*1000000;
        long lTimeEnd = tConvert.StrToTime(sTimeEnd.c_str())*1000000;
        sCondition += " and lAddTime >=" + tConvert.LongToStr(lTimeStart)  + " and lAddTime <="  + tConvert.LongToStr(lTimeEnd);
    }

    sSql = sSql + sCondition;
    sSql += " order by lAddTime desc ";
    sSql += " limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    MYSQL_RES* result;

    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        iRet = 0;
        ResultToLst(result,pList);
    }

    FreeResult( result );

    sSql = "select count(*) from " + msTableName + " where 1=1 " + sCondition;
    GetQueryTotalCount(sSql,iTotal);

    return iRet;

}


//获取一批用户数据
int UserDataDAL::GetUserInfoList( const vector<long> &  plUserIdLst, vector<UserDataEntity>& pList  )
{
    if( plUserIdLst.size() == 0){
        return 0;
    }
    TConvert tConvert;
    // SQL
    string sUsers;
    for( vector<long>::const_iterator it = plUserIdLst.begin(); it != plUserIdLst.end(); it++){
        sUsers += tConvert.LongToStr(*it);
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


//获取推广粉丝
int UserDataDAL::GetMyPush(const long & plUserId, vector<UserDataEntity>& pList )
{
    string sSql =   msAllColumn +  msTableName + " where lPushManId='" + mtConvert.LongToStr(plUserId)+ "'";

    int iRet = -1;
    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        iRet = 0;
        ResultToLst(result,pList);
    }

    FreeResult( result );

    return iRet;

}

void UserDataDAL::ResultToLst(MYSQL_RES* result ,vector<UserDataEntity>& lst)
{

    MYSQL_ROW row = NULL;

    while( NULL !=  ( row = mysql_fetch_row( result ) )) {
        UserDataEntity entity;

        RowToEntity(row,entity);

        lst.push_back(entity);
    }

}

void UserDataDAL::RowToEntity(MYSQL_ROW row ,UserDataEntity & entity)
{
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
    entity.sQqNum = row[iIndex++] ;
    entity.sWeixinNum =  row[iIndex++];
    entity.sAliNum =  row[iIndex++];

}

int UserDataDAL::GetUserIdByPhone(const string & sPhone, long & lUserId)
{

    TMultiMysqlDAL multi;

    string sSql =   "select lUserId from " + this->msTableName + " where sAccount='" + multi.RealEscapeString( sPhone ) + "'";

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
            while( NULL != row ) {
                lUserId = atol(row[0]);
                row = mysql_fetch_row( result );
            }
        }
    }
    if( result != NULL)
        con->FreeResult( result );

    return iRet;

}

//获取推广粉丝数
int UserDataDAL::GetFansByPeragentId(long lPeragnetId, int & iCount)
{
    TConvert tConvert;
    string sWhere = " where lPushManId='" + tConvert.LongToStr(lPeragnetId)+ "'" ;
    return GetCount(sWhere,iCount);

}


//获取推广粉丝数 and hot=1
int UserDataDAL::GetHotFansCountByPeragent(long lPeragnetId, int & iCount)
{
    TConvert tConvert;
    string sWhere = " where iHot=1 and lPushManId='" + tConvert.LongToStr(lPeragnetId)+ "'" ;
    return GetCount(sWhere,iCount);

}

//获取平台的关注粉丝数
int UserDataDAL::GetFansByPlatformId(long lPlatformId, int & iCount)
{
    TConvert tConvert;
    string sWhere = " where lCurrentPlatformId='" + tConvert.LongToStr(lPlatformId)+ "'" ;
    return GetCount(sWhere,iCount);

}

//获取平台的关注男性粉丝数
int UserDataDAL::GetMaleFansByPlatformId(long lPlatformId, int & iCount)
{
    TConvert tConvert;
    string sWhere = " where lCurrentPlatformId='" + tConvert.LongToStr(lPlatformId)+ "' and iSex=1" ;
    return GetCount(sWhere,iCount);

}


//获取平台的关注女性粉丝数
int UserDataDAL::GetFemaleFansByPlatformId(long lPlatformId, int & iCount)
{
    TConvert tConvert;

    string sWhere = " where lCurrentPlatformId='" + tConvert.LongToStr(lPlatformId)+ "' and iSex=0" ;
    return GetCount(sWhere,iCount);
}

//获取关注的成员列表
int UserDataDAL::GetAttentionByPlatformId(long lPlatformId,int iIndex ,int iCount,UserDataList & userDataLst)
{
    TConvert tConvert;


    // SQL
    string sSql = "select lUserId,sNickName, sHeadImage from " +msTableName;
    sSql += "  where lCurrentPlatformId=" + tConvert.LongToStr(lPlatformId) ;
    sSql += " order by lAddTime desc limit " + tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    MYSQL_RES* result = NULL;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            UserDataEntity entity;

            entity.lUserId = atol( row[iIndex++]);
            entity.sNickName = row[iIndex++];
            entity.sHeadImage =  row[iIndex++];

            userDataLst.push_back(entity);


            iRet = 0;


        }
    }

    if( result != NULL)
        con->FreeResult(result);

    return iRet;

}

int UserDataDAL::UpQqNum(const string sAccount  , const string & sQqNum)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sQqNum = '" + tMultiMysqlDAL.RealEscapeString(sQqNum) + "' ";

    sSql += "where sAccount = '"+tMultiMysqlDAL.RealEscapeString(sAccount)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int UserDataDAL::UpWeixinNum(const string sAccount  , const string & sQqWeixinNum)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sWeixinNum = '" + tMultiMysqlDAL.RealEscapeString(sQqWeixinNum) + "' ";

    sSql += "where sAccount = '"+tMultiMysqlDAL.RealEscapeString(sAccount)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);


}

int UserDataDAL::UpAliNum(const string sAccount  , const string & num)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sAliNum = '" + tMultiMysqlDAL.RealEscapeString(num) + "' ";

    sSql += "where sAccount = '"+tMultiMysqlDAL.RealEscapeString(sAccount)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);


}

int UserDataDAL::GetRegistersByMonth(string sStart, string sEnd,CommonStructVector & vct)
{
    TMultiMysqlDAL tmult;
    //select substring(from_unixtime(lAddTime/1000000),1,7) as date, count(*) as count from  user_datadal  where substring(from_unixtime(lAddTime/1000000),1,7) >= '2017-01' group by  substring(from_unixtime(lAddTime/1000000),1,7)
    string sSql= "select substring(from_unixtime(lAddTime/1000000),1,7) as date, count(*) as count from  user_datadal"
                 "  where substring(from_unixtime(lAddTime/1000000),1,7) >= '";
    sSql += tmult.RealEscapeString( sStart );
    sSql += "' and substring(from_unixtime(lAddTime/1000000),1,7) <= '";
    sSql += tmult.RealEscapeString( sEnd );
    sSql += "' group by  substring(from_unixtime(lAddTime/1000000),1,7)";


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
                CommonStruct entity;

                int iIndex = 0;

                entity.v1 =row[iIndex++];
                entity.v2 =  row[iIndex++] ;


                vct.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int UserDataDAL::GetRegistersByDay(string sStart, string sEnd,CommonStructVector & vct)
{
    TMultiMysqlDAL tmult;
    //select substring(from_unixtime(lAddTime/1000000),1,7) as date, count(*) as count from  user_datadal  where substring(from_unixtime(lAddTime/1000000),1,7) >= '2017-01' group by  substring(from_unixtime(lAddTime/1000000),1,7)
    string sSql= "select substring(from_unixtime(lAddTime/1000000),1,10) as date, count(*) as count from  user_datadal"
                 "  where substring(from_unixtime(lAddTime/1000000),1,10) >= '";
    sSql += tmult.RealEscapeString( sStart );
    sSql += "' and substring(from_unixtime(lAddTime/1000000),1,10) <= '";
    sSql += tmult.RealEscapeString( sEnd );
    sSql += "' group by  substring(from_unixtime(lAddTime/1000000),1,10)";


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
                CommonStruct entity;

                int iIndex = 0;

                entity.v1 =row[iIndex++];
                entity.v2 =  row[iIndex++] ;


                vct.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int UserDataDAL::GetUnHotCount(CommonStructVector & vct)
{
    TMultiMysqlDAL tmult;
    //select substring(from_unixtime(lAddTime/1000000),1,7) as date, count(*) as count from  user_datadal  where substring(from_unixtime(lAddTime/1000000),1,7) >= '2017-01' group by  substring(from_unixtime(lAddTime/1000000),1,7)
    string sSql= "select substring(from_unixtime(lAddTime/1000000),1,10) as date, count(*) as count from  user_datadal"
                 "  where iHot!=1 ";

    sSql += " group by  substring(from_unixtime(lAddTime/1000000),1,10) order by date desc ";


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
                CommonStruct entity;

                int iIndex = 0;

                entity.v1 =row[iIndex++];
                entity.v2 =  row[iIndex++] ;


                vct.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int UserDataDAL::GetRegistersByYear(string sStart, string sEnd,CommonStructVector & vct)
{
    TMultiMysqlDAL tmult;
    //select substring(from_unixtime(lAddTime/1000000),1,7) as date, count(*) as count from  user_datadal  where substring(from_unixtime(lAddTime/1000000),1,7) >= '2017-01' group by  substring(from_unixtime(lAddTime/1000000),1,7)
    string sSql= "select substring(from_unixtime(lAddTime/1000000),1,4) as date, count(*) as count from  user_datadal"
                 "  where substring(from_unixtime(lAddTime/1000000),1,4) >= '";
    sSql += tmult.RealEscapeString( sStart );
    sSql += "' and substring(from_unixtime(lAddTime/1000000),1,4) <= '";
    sSql += tmult.RealEscapeString( sEnd );
    sSql += "' group by  substring(from_unixtime(lAddTime/1000000),1,4)";


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
                CommonStruct entity;

                int iIndex = 0;

                entity.v1 =row[iIndex++];
                entity.v2 =  row[iIndex++] ;


                vct.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}



//转移推广关系
int UserDataDAL::MovePushMan(long lOrgId, long lNewId,  string sSelledUser)
{

    string sSql = GetSqlMovePushMan(lOrgId,lNewId,sSelledUser);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}
//转移推广关系 sql
string UserDataDAL::GetSqlMovePushMan(long lOrgId, long lNewId,  string sSelledUser)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",lPushManId=" + tConvert.LongToStr(lNewId) +
            " where lPushManId=" + tConvert.LongToStr(lOrgId) + " and lUserId in(" + sSelledUser + ")";

    return sSql;

}


//转移关注关系
int UserDataDAL::MoveAttention(long lOrgId, long lNewId,  string sSelledUser)
{
    string sSql = GetSqlMoveAttention(lOrgId,lNewId,sSelledUser);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//转移关注关系 sql
string UserDataDAL::GetSqlMoveAttention(long lOrgId, long lNewId, string sSelledUser)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",lCurrentPlatformId=" + tConvert.LongToStr(lNewId) +
            " where lCurrentPlatformId=" + tConvert.LongToStr(lOrgId)   +  " and lUserId in(" + sSelledUser + ")";

    return sSql;

}

//解冻
int UserDataDAL::Jeidong(string sAccount)
{
    long lCurrentTime = mtConvert.GetCurrentTime();
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",iState=0";

    sSql +=  " where sAccount='" + mMulti.RealEscapeString(sAccount) + "'" ;

    return BaseQuery(sSql);
}

int UserDataDAL::HotVerify(string sDay, int iType , string id, int iHot)
{
    long lCurrentTime = mtConvert.GetCurrentTime();
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",iHot=" + mtConvert.IntToStr(iHot);

    sSql +=  " where substring(from_unixtime(lAddTime/1000000),1,10)='" + mMulti.RealEscapeString(sDay) + "'" ;
    if( iType == enum_kvalue_type_platform){
        sSql += " and lCurrentPlatformId=" + mMulti.RealEscapeString(id);
    }else{
        sSql += " and lPushManId=" + mMulti.RealEscapeString(id);
    }

    return BaseQuery(sSql);
}

int UserDataDAL::FansRecover(string sDay, int iType , string id , string selfid)
{
    long lCurrentTime = mtConvert.GetCurrentTime();
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + mtConvert.LongToStr(lCurrentTime) + "' "+
            ",lCurrentPlatformId=100012,lPushManId=39";

    sSql +=  " where substring(from_unixtime(lAddTime/1000000),1,10)='" + mMulti.RealEscapeString(sDay) + "'" ;
    sSql += " and lUserId!=" + selfid;
    if( iType == enum_kvalue_type_platform){
        sSql += " and lCurrentPlatformId=" + mMulti.RealEscapeString(id);
    }else{
        sSql += " and lPushManId=" + mMulti.RealEscapeString(id);
    }

    return BaseQuery(sSql);
}
