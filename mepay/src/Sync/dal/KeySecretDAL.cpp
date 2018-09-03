#include "KeySecretDAL.h"




KeySecretDAL::KeySecretDAL()
{

}



int KeySecretDAL::Add( const string & sAppKey, const string & sAppSecret ,long lPlatformId )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "insert into " +  msTableName+"( sAppKey,sAppSecret,lPlatformId)" +
            "VALUES( '"+mlti.RealEscapeString( sAppKey)+"','" +
            mlti.RealEscapeString(sAppSecret)+ "','" +
            tConvert.LongToStr(lPlatformId) +
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int KeySecretDAL::Replace(const string & sAppKey, const string & sAppSecret,long lPlatformId)
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "replace into " +  msTableName+"( sAppKey,sAppSecret,lPlatformId)" +
            "VALUES( '"+mlti.RealEscapeString( sAppKey)+"','" +
            mlti.RealEscapeString(sAppSecret)+ "','" +
            tConvert.LongToStr(lPlatformId) +
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;

}



int KeySecretDAL::IsExist( const string & sAppKey)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where sAppKey='"+ mlti.RealEscapeString(sAppKey) + "'";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        iRet = -4;
        return iRet;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        iRet = 0; //exist
    }else if( NULL != result ){
        iRet = 1;//no exist
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


int KeySecretDAL::UpSecret(string sAccount )
{
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql = "update user_datadal set iState=1 where sAccount=" + sAccount ;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int KeySecretDAL::Get(const string & sAppKey,string & sAppSecret, long & lPlatformId)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL mlti;
    TMultiMysqlDAL * con = NULL;
    // SQL
    string sSql = "select sAppSecret,lPlatformId "
                  " from "+msTableName;
    sSql+=" where sAppKey= '"+ mlti.RealEscapeString(sAppKey )+"'";
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
            sAppSecret = row[iIndex++];
            lPlatformId = atol(row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}

