#include "PersonalAgentDAL.h"
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

const string PersonalAgentDAL::msTableName = "peragent";
PersonalAgentDAL* PersonalAgentDAL::mInstance = NULL;
PersonalAgentDAL* PersonalAgentDAL::GetInstance()
{

    if (mInstance == NULL){
        mInstance = new PersonalAgentDAL;
    }
    return mInstance;
}


//添加
int PersonalAgentDAL::Insert( const PersonalAgentEntity& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql   = "insert into " +  msTableName+"( sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId," +
            "sPhone,iActivationState,lActivationTime,sOrderNo,lMoney,sCity,lPushId,lDealTime,lExpireTime)" +
            "VALUES( '"+entity.sPrimaryKey+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            tConvert.IntToStr(entity.iState)+"','" +
            tConvert.LongToStr(entity.lUpdateId)+"','" +
            entity.sPhone+"','" +
            tConvert.IntToStr(entity.iActivationState)+"','" +
            tConvert.LongToStr(entity.lActivationTime)+"','" +
            entity.sOrderNo+"','"+
            tConvert.LongToStr(entity.lMoney)+"','"+
            entity.sCity+"','"+
            tConvert.LongToStr(entity.lPushId)+"','"+
            tConvert.LongToStr(entity.lDealTime)+"','"+
            tConvert.LongToStr(entity.lExpireTime)+
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
int PersonalAgentDAL::Delete( const string& sPrimaryKey, const long lUptTime,const long lUpdateId )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql = "update "+msTableName+" set "+
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
int PersonalAgentDAL::Update(  const string& sPrimaryKey,const PersonalAgentEntity& entity )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL multidal;
    // SQL
    string sSql = "update "+msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(entity.lUpdateId) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(entity.lUpTime) + "' "+
            ",iState = '" + tConvert.LongToStr(entity.iState) + "' ";
    if ( entity.sPhone !=  "-1nochange" ) {
        sSql += ",sPhone = '" + multidal.RealEscapeString(entity.sPhone) + "' ";
    }
    if ( entity.iActivationState >= 0 ) {
        sSql += ",iActivationState = '" + tConvert.IntToStr(entity.iActivationState) + "' ";
    }
    if ( entity.lActivationTime != -1 ) {
        sSql += ",lActivationTime = '" + tConvert.LongToStr(entity.lActivationTime) + "' ";
    }
    if ( entity.sOrderNo !=  "-1nochange" ) {
        sSql += ",sOrderNo = '" + entity.sOrderNo + "' ";
    }
    if ( entity.lMoney != -1 ) {
        sSql += ",lMoney = '" + tConvert.LongToStr(entity.lMoney) + "' ";
    }
    if ( entity.lExpireTime != -1 ) {
        sSql += ",lExpireTime = '" + tConvert.LongToStr(entity.lExpireTime) + "' ";
    }
    if ( entity.lPushId != -1 ) {
        sSql += ",lPushId = '" + tConvert.LongToStr(entity.lPushId) + "' ";
    }
    if ( entity.sCity != "-1nochange"  ) {
        sSql += ",sCity = '" + entity.sCity + "' ";
    }
    if( entity.lDealTime != -1){
        sSql += ",lDealTime = '" +tConvert.LongToStr( entity.lDealTime) + "' ";
    }

    sSql += " where sPrimaryKey = '"+sPrimaryKey+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

int PersonalAgentDAL::GetEntityByKey(const string & psKey, int piType,PersonalAgentEntity  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,sPhone,iActivationState,lActivationTime,sOrderNo,lMoney,lPushId,sCity,lDealTime,lExpireTime"
                  " from "+msTableName;
    if( piType == 1){
      sSql+=" where iState!=333 and sPrimaryKey= '"+psKey+"'";
    }else if( piType == 2){
        sSql+=" where iState!=333 and sOrderNo= '"+psKey+"'";
    }
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
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            entity.sPhone = row[iIndex++];
            entity.iActivationState =  atoi( row[iIndex++] );
            entity.lActivationTime = atol( row[iIndex++] );
            entity.sOrderNo =  row[iIndex++] ;
            entity.lMoney = atol( row[iIndex++]) ;
            entity.lPushId = atol( row[iIndex++]) ;
            entity.sCity =  row[iIndex++] ;
            entity.lDealTime = atol(row[iIndex++] );
            entity.lExpireTime = atol(row[iIndex++] );
        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

int PersonalAgentDAL::GetData(const PersonalAgentQueryEntity & pQueryEntity,int piIndex ,int piCount,PersonalAgentList & pOutLst,  int & piTotalCount)
{
    TConvert tConvert;
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,sPhone,iActivationState,lActivationTime,sOrderNo,lMoney,lPushId,sCity,lDealTime,lExpireTime"
                  " from "+msTableName ;
    string sCond = " where iState!=333 and";

    if( pQueryEntity.iActivationState != -1){
        sCond += " iActivationState=" + tConvert.IntToStr( pQueryEntity.iActivationState ) + " and";
    }
    if( pQueryEntity.lActivationTimeStart > 0 && pQueryEntity.lActivationTimeEnd > 0){
        sCond += " lActivationTime>=" + tConvert.LongToStr( pQueryEntity.lActivationTimeStart ) + " and";
        sCond += " lActivationTime<=" + tConvert.LongToStr( pQueryEntity.lActivationTimeEnd ) + " and";
    }
    if(! pQueryEntity.sPhone.empty()){
        sCond += " sPhone='" +  pQueryEntity.sPhone + "' and";
    }
    if(! pQueryEntity.sCity.empty()){
        sCond += " sCity like '%" +  pQueryEntity.sCity + "%' and";
    }

    sCond = sCond.substr(0,sCond.length() - 3);
    sSql +=  sCond;
    sSql += " order by lDealTime desc ";

    sSql += " limit " + tConvert.IntToStr(piIndex) + "," + tConvert.IntToStr(piCount);
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;

        while( NULL != (row =mysql_fetch_row( result )) ) {
            iRet = 0;
            //加载主键表
            PersonalAgentEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey = row[iIndex++];
            entity.lAddTime = atol( row[iIndex++] );
            entity.lUpTime = atol( row[iIndex++] );
            entity.iState = atoi( row[iIndex++] );
            entity.lUpdateId = atol( row[iIndex++] );
            entity.sPhone = row[iIndex++];
            entity.iActivationState =  atoi( row[iIndex++] );
            entity.lActivationTime = atol( row[iIndex++] );
            entity.sOrderNo =  row[iIndex++] ;
            entity.lMoney = atol( row[iIndex++]) ;
            entity.lPushId = atol( row[iIndex++]) ;
            entity.sCity =  row[iIndex++] ;
            entity.lDealTime = atol(row[iIndex++] );
            entity.lExpireTime = atol(row[iIndex++] );
            pOutLst.push_back( entity );
        }

        sSql = "select count(*)  from " + msTableName ;
        sSql  += sCond;
        GetQueryTotalCount(sSql,piTotalCount);
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int PersonalAgentDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
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

