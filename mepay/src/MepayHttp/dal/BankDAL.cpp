#include "BankDAL.h"


//公司名称，string,默认''
//对公账户，string,默认''
//银行类型，string,默认''
//开户支行，string,默认''
BankDAL::BankDAL()
{

}


//初始化表-没有自动创建
int BankDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+msTableName+" ("+
            "lPlatformId VARCHAR(128) NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "sName VARCHAR(1024) NOT NULL,"+
            "sAccount VARCHAR(128) NOT NULL ,"+
            "sBranch VARCHAR(1024) NOT NULL ,"+
            "sBank VARCHAR(1024) NOT NULL ,"+
            "PRIMARY KEY(lPlatformId),"+
            "constraint FK_bank_lPlatformId foreign key(lPlatformId) references shopdal(sPrimaryKey)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}


int BankDAL::replace( const BankEntity& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    // SQL
    string sSql   = "replace into " +  msTableName+"( lPlatformId,lAddTime,sName,sAccount,sBranch,sBank)" +
            "VALUES( '"+tConvert.LongToStr( entity.lPlatformId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            entity.sName+"','" +
            entity.sAccount+"','" +
            entity.sBranch+"','" +
            entity.sBank+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int BankDAL::GetEntity(long lPlatformId,BankEntity  & entity)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TConvert tConvert;
    // SQL
    string sSql = "select lPlatformId,lAddTime,sName,sAccount,sBranch,sBank"
                  " from "+msTableName;
    sSql+=" where  lPlatformId= '"+tConvert.LongToStr(lPlatformId)+"'";
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
            entity.lPlatformId = atol(row[iIndex++]);
            entity.lAddTime = atol( row[iIndex++] );
            entity.sName =  row[iIndex++];
            entity.sAccount =  row[iIndex++];
            entity.sBranch =  row[iIndex++];
            entity.sBank =  row[iIndex++];

        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}
