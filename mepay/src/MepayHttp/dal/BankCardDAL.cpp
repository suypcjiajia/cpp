#include "BankCardDAL.h"
#include "dal/TMysqlDAL.h"
#include "dal/TMultiMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"

BankCardDAL* BankCardDAL::mInstance = NULL;
BankCardDAL* BankCardDAL::GetInstance()
{

    if (mInstance == NULL){
        mInstance = new BankCardDAL;
    }
    return mInstance;
}

//创建表
int BankCardDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+gBankCardTb+" ("+
            "sPrimaryKey VARCHAR(50) NOT NULL,"+
            "lUserId BIGINT  NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT default 0,"+
            "lUpdateId BIGINT default 0,"+
            "sBankCard VARCHAR(50) NOT NULL," +
            "sName VARCHAR(50) default '',"+
            "sBank VARCHAR(50) default '' ,"+
            "sBranch VARCHAR(50) NOT NULL,"+
            "iCerType INT default 1,"+
            "sCerId VARCHAR(50) default '',"+
            "sCustomerNm VARCHAR(50) default '',"+
            "sCredit1  VARCHAR(50) default '',"+
            "sCredit2  VARCHAR(50) default '',"+
            "sBindId VARCHAR(50) default '',"+
            "sOrderNum VARCHAR(50) default '',"+
            "sMerId VARCHAR(50)  default '',"+
            "lBandTime BIGINT default 0,"+
            "PRIMARY KEY(sPrimaryKey)"+
            ") ENGINE=INNODB  DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}
//添加
int BankCardDAL::Insert( const BankCardEntity& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql   = "insert into " +  gBankCardTb+"( sPrimaryKey,sBankCard,lUserId,lAddTime,lUpTime,iState,lUpdateId," +
            "sName,sBank,sBranch,iCerType,sCerId,sCustomerNm,sCredit1,sCredit2,sBindId,sOrderNum,sMerId,lBandTime)" +
            "VALUES( '"+entity.sPrimaryKey+"','" +
            entity.sBankCard+"','" +
            tConvert.LongToStr(entity.lUserId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lUpdateId)+"','" +
            entity.sName+"','" +
            entity.sBank+"','" +
            entity.sBranch+"','" +
            tConvert.IntToStr(entity.iCerType) +"','"+
            entity.sCerId+"','"+
            entity.sCustomerNm+"','"+
            entity.sCredit1+"','"+
            entity.sCredit2+"','"+
            entity.sBindId+"','"+
            entity.sOrderNum+"','"+
            entity.sMerId+"','"+
            tConvert.LongToStr(entity.lBandTime)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}
//删除
int BankCardDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+gBankCardTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lUptTime) + "' "+
            ",iState = '333' ";
    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);

    return 0;
}
//修改
int BankCardDAL::Update(  const string& sPrimaryKey,const BankCardEntity& entity )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+gBankCardTb+" set "+
            "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' " +
            ",iState = '0' ";
    if ( entity.iCerType != -1 ) {
        sSql += ",iCerType = '" + tConvert.IntToStr(entity.iCerType) + "' ";
    }
    if ( entity.lBandTime != -1 ) {
        sSql += ",lBandTime = '" + tConvert.LongToStr(entity.lBandTime) + "' ";
    }
    if ( entity.lUserId  != -1 ) {
        sSql += ",lUserId = '" + tConvert.LongToStr(entity.lUserId) + "' ";
    }
    if ( entity.sBank  != "-1nochange" ) {
        sSql += ",sBank = '" + entity.sBank + "' ";
    }
    if ( entity.sBindId != "-1nochange") {
        sSql += ",sBindId = '" + entity.sBindId + "' ";
    }
    if ( entity.sBranch != "-1nochange" ) {
        sSql += ",sBranch = '" + entity.sBranch + "' ";
    }
    if ( entity.sCerId != "-1nochange" ) {
        sSql += ",sCerId = '" + entity.sCerId + "' ";
    }
    if ( entity.sCredit1 != "-1nochange" ) {
        sSql += ",sCredit1 = '" + entity.sCredit1 + "' ";
    }
    if ( entity.sCredit2 != "-1nochange" ) {
        sSql += ",sCredit2 = '" + entity.sCredit2 + "' ";
    }
    if ( entity.sCustomerNm != "-1nochange" ) {
        sSql += ",sCustomerNm = '" + entity.sCustomerNm + "' ";
    }
    if ( entity.sMerId != "-1nochange" ) {
        sSql += ",sMerId = '" + entity.sMerId + "' ";
    }
    if ( entity.sName != "-1nochange" ) {
        sSql += ",sName = '" + entity.sName + "' ";
    }
    if ( entity.sOrderNum != "-1nochange" ) {
        sSql += ",sOrderNum = '" + entity.sOrderNum + "' ";
    }
    if( entity.sBankCard != "-1nochange"){
        sSql += ",sBankCard = '" + entity.sBankCard + "' ";
    }

    sSql += "where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}




int BankCardDAL::GetEntity(const string & psCond,BankCardEntity  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,sBankCard,lUserId,lAddTime,lUpTime,iState,lUpdateId,sName,sBank,sBranch,iCerType,sCerId,sCustomerNm,sCredit1,sCredit2,sBindId,sOrderNum,sMerId,lBandTime"
                  " from "+gBankCardTb;
    sSql+=" where iState!=333 and sPrimaryKey= '"+psCond+"'";
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = ERRNOTDATA;
        MYSQL_ROW row = NULL;
        row =mysql_fetch_row( result );
        if( NULL != row  ) {
            iRet = 0;
            int iIndex = 0;
            entity.sPrimaryKey = row[iIndex++];
            entity.sBankCard = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            entity.sName = row[iIndex++];
            entity.sBank = row[iIndex++];
            entity.sBranch = row[iIndex++];
            entity.iCerType =atoi( row[iIndex++]);
            entity.sCerId =  row[iIndex++] ;
            entity.sCustomerNm =  row[iIndex++] ;
            entity.sCredit1 = row[iIndex++] ;
            entity.sBindId =  row[iIndex++] ;
            entity.sOrderNum =  row[iIndex++] ;
            entity.sMerId =  row[iIndex++] ;
            entity.lBandTime =  atol(row[iIndex++]) ;
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

int BankCardDAL::IsExist(const string & psPrimaryKey)
{

    // SQL
    string sSql = "select 1 from "+ gBankCardTb +" where sPrimaryKey='"+psPrimaryKey+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        return 0;
    }
    // 释放内存
    con->FreeResult( result );

    return -1;

}

int BankCardDAL::GetList(const BankCardQueryEntity & pQueryEntity,int piIndex ,int piCount,BankCardList & pOutLst, int & piTotalCount)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,sBankCard,lUserId,lAddTime,lUpTime,iState,lUpdateId,sName,sBank,sBranch,iCerType,sCerId,sCustomerNm,sCredit1,sCredit2,sBindId,sOrderNum,sMerId,lBandTime"
                  " from "+gBankCardTb ;

    string sCond = " where iState!=333 and";
    if( pQueryEntity.lUserId != -1){
        sCond += " lUserId='" + tConvert.LongToStr( pQueryEntity.lUserId ) + "' and";
    }
    sCond = sCond.substr(0,sCond.length() -3);
    sSql += sCond;
    sSql += " order by lAddTime desc ";
    sSql += " limit " + tConvert.IntToStr(piIndex) + "," + tConvert.IntToStr(piCount);
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        iRet = 0;

        while( NULL != (row =mysql_fetch_row( result )) ) {

            //加载主键表
            BankCardEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey = row[iIndex++];
            entity.sBankCard = row[iIndex++];
            entity.lUserId = atol( row[iIndex++] );
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            entity.sName = row[iIndex++];
            entity.sBank = row[iIndex++];
            entity.sBranch = row[iIndex++];
            entity.iCerType =atoi( row[iIndex++]);
            entity.sCerId =  row[iIndex++] ;
            entity.sCustomerNm =  row[iIndex++] ;
            entity.sCredit1 = row[iIndex++] ;
            entity.sBindId =  row[iIndex++] ;
            entity.sOrderNum =  row[iIndex++] ;
            entity.sMerId =  row[iIndex++] ;
            entity.lBandTime =  atol(row[iIndex++]) ;
            pOutLst.push_back( entity );
        }
        sSql = "select count(*)  from " + gBankCardTb ;
        sSql  +=  sCond;
        GetQueryTotalCount(sSql,piTotalCount);
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}




int BankCardDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(psQuery);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        piCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            piCount = atoi(row[0]);
        }
    }
    return iRet;
}


