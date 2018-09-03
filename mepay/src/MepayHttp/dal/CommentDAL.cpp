#include "CommentDAL.h"







int CommentDAL::Add( const CommentEntity & entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    // SQL
    string sSql   = "insert into " +  msTableName+"( lPlatformId,lProductId,lAddTime,lUserId,sNickName,sHeadImage,sContent,sPics,iScore)" +
            "VALUES( '"+tConvert.LongToStr( entity.lPlatformId)+"','" +
            tConvert.LongToStr(entity.idProduct)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUserId)+"','" +
            mlti.RealEscapeString(entity.sNickName)+"','" +
            mlti.RealEscapeString(entity.sHeadImage)+"','" +
            mlti.RealEscapeString(entity.sContent)+"','" +
            mlti.RealEscapeString(entity.sPics)+"','" +
            tConvert.LongToStr( entity.iScore)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}





//查询数据
int CommentDAL::GetData(  long  lPlatformId ,long lProductId,long lAddTime,int iCount, CommentEntityLst & lst,int & iTotal)
{

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;

    // SQL
    string sSql = "select " + msAllColumn +  " from "+msTableName ;

    string sCondition = "  where  lPlatformId=" + tConvert.LongToStr(lPlatformId) + " and lProductId=" + tConvert.LongToStr(lProductId);
    sCondition += " and lAddTime<" + tConvert.LongToStr(lAddTime) ;
    sSql += sCondition;
    sSql += " order by lAddTime desc";
    sSql += " limit " + tConvert.IntToStr(iCount);

    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    //appendlog(TTDLogger::LOG_DEBUG,"xxxxx:%s",sSql.c_str());


    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {

            CommentEntity entity;
            RowToEntity(row,entity);
            lst.push_back(entity);

        }
    }

    GetCount(sCondition,iTotal);

    // 释放内存
    con->FreeResult( result );
    return iRet;

}

//获取综合评分
int CommentDAL::GetAvgSorce( long lPlatformId,long lProductId)
{
    //ScopeLock lock( this->mMutex );
    int iRet = 30;//默认30分
    TConvert tConvert;
    TMultiMysqlDAL mlti;
    MYSQL_RES* result;
    // SQL
    string sSql   = "select avg(iScore) from " +  msTableName;
    sSql += " where lPlatformId=" + tConvert.LongToStr(lPlatformId) + " and lProductId=" + tConvert.LongToStr(lProductId);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return iRet;
    }

    result =  con->QueryResult(sSql);

    if ( NULL != result ) {

        MYSQL_ROW row = NULL;
        row =mysql_fetch_row( result );
        if( NULL != row  ) {

            int iIndex = 0;
            if( row[iIndex] != NULL){
               iRet  = atoi(row[iIndex++]);
            }
            if( iRet < 30)
                iRet = 30;//默认30分

        }
    }

    // 释放内存
    con->FreeResult( result );
    return iRet;

}


int CommentDAL::GetOneById( long lPlatformId,long lProductId,long lUserId,CommentEntity & entity)
{
    int iRet = -1;
    MYSQL_RES* result;

    string sSql = "select " + msAllColumn +  " from "+msTableName ;

    string sCondition = "  where  lPlatformId=" + mtConvert.LongToStr(lPlatformId) +
            " and lProductId=" + mtConvert.LongToStr(lProductId) +
            " and lUserId=" + mtConvert.LongToStr(lUserId);

    sSql += sCondition;

    sSql += " order by lAddTime desc limit 1 ";

    BaseQueryResult(sSql,&result);

    if( result != NULL){

        iRet = 0;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row(result) )) {

            RowToEntity(row,entity);
        }
    }

    FreeResult( result );
    return iRet;

}


int CommentDAL::RowToEntity(MYSQL_ROW row,CommentEntity & entity)
{
    int iIndex = 0;

    entity.lPlatformId =  atol(row[iIndex++]);
    entity.idProduct =  atol(row[iIndex++]);
    entity.lAddTime = atol(row[iIndex++]);
    entity.lUserId = atol (row[iIndex++]);
    entity.sNickName =  row[iIndex++];
    entity.sHeadImage =  row[iIndex++];
    entity.sContent =  row[iIndex++];
    entity.sPics =  row[iIndex++];
    entity.iScore = atoi(row[iIndex++]);

}

