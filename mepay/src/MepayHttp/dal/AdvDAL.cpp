#include "AdvDAL.h"




AdvDAL::AdvDAL()
{

}



//初始化表-没有自动创建
int AdvDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+msTableName+" ("+
            "id BIGINT not null AUTO_INCREMENT," +
            "lPlatformId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "sAdvLink varchar(500) NOT NULL,"+
            "iVerify INT default 0," +
            "iState int NOT NULL default 1,"
            "PRIMARY KEY(id)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

int AdvDAL::Add( const ShopAdv & entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "insert into " +  msTableName+"( lPlatformId,lAddTime,sAdvLink,id,iState)" +
            "VALUES( '"+tConvert.LongToStr( entity.lPlatformId)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            mlti.RealEscapeString(entity.sAdvLink)+"','" +
            tConvert.LongToStr(entity.id)+"','" +
            tConvert.LongToStr( entity.iState)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int AdvDAL::Del(long id)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;
    string sSql = "delete from " + msTableName + " where id=" + tConvert.LongToStr(id);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

int AdvDAL::IsExist( long lPlatformId,const string & sAdvLink)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lPlatformId='"+ tConvert.LongToStr(lPlatformId);
    sSql += "' and ";
    sSql += "sAdvLink='";
    sSql += mlti.RealEscapeString(sAdvLink) + "'";

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


int AdvDAL::IsExist( long id)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL mlti;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where id="+ tConvert.LongToStr(id);

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



//查询数据
int AdvDAL::GetData(  long  lPlatformId ,int iPage,int iCount,int iVerify,int iState, ShopAdvLst & lst, int & iTotalCount)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    int iIndex = (iPage -1 )*iCount;
    string sCondition;

    // SQL
    string sSql = "select id,lPlatformId,lAddTime,sAdvLink,iState,iVerify"
                  " from "+msTableName ;
    if( -1 != iVerify){
      sCondition += " iVerify=" + tConvert.IntToStr(iVerify) + " and";
    }
    if( -1 != iState){
      sCondition += " iState=" + tConvert.IntToStr(iState) + " and";
    }
    if( -1 != lPlatformId){
        sCondition += " lPlatformId=" + tConvert.LongToStr(lPlatformId) + " and";
    }

    if( !sCondition.empty()){
        sCondition = sCondition.substr(0,sCondition.length() - 3);
    }
    if( !sCondition.empty()){
        sCondition = " where " + sCondition;
        sSql += sCondition;
    }

    sSql += " order by lAddTime desc  limit " +  tConvert.IntToStr(iIndex) + "," + tConvert.IntToStr(iCount);



    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            ShopAdv entity;
            int iIndex = 0;

            entity.id =  atol(row[iIndex++]);
            entity.lPlatformId =  atol(row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.sAdvLink = row[iIndex++];
            entity.iState = atoi( row[iIndex++]);
            entity.iVerify = atoi( row[iIndex++]);


            lst.push_back(entity);

        }
    }

    sSql = "select count(*)  from " + msTableName ;
    sSql  +=  sCondition;

    GetQueryTotalCount(sSql,iTotalCount);

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

int AdvDAL::UpVerify(long id, int iValue )
{
    TConvert tConvert;
    // SQL
    string sSql = "update "+ msTableName + " set ";
    sSql += " iVerify =" + tConvert.IntToStr(iValue) ;
    sSql += " where id='"+ tConvert.LongToStr(id) + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int AdvDAL::GetQueryTotalCount(const string & psQuery, int & piCount)
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
