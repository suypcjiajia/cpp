#include "SubjectDAL.h"



SubjectDAL::SubjectDAL()
{

}


//初始化表-没有自动创建
int SubjectDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+msTableName+" ("+
            "Id INT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "sName VARCHAR(64) NOT NULL,"+
            "sBigTopPic VARCHAR(1024) NOT NULL default '' ,"+
            "sBigBottonPic VARCHAR(1024) NOT NULL default '',"+
            "sSmallTopPic VARCHAR(1024) NOT NULL default '',"+
            "sSmallBottonPic VARCHAR(1024) NOT NULL default '',"+
            "sRemark VARCHAR(128) NOT NULL default '',"+
            "iState INT NOT NULL default 0,"+
            "PRIMARY KEY(Id)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}


int SubjectDAL::replace( const Subject& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mutlti;
    // SQL
    string sSql   = "replace into " +  msTableName+"( Id,lAddTime,sName,sBigTopPic,sBigBottonPic,sSmallTopPic,sSmallBottonPic,sRemark,iState)" +
            "VALUES( '"+tConvert.LongToStr( entity.Id)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            mutlti.RealEscapeString(entity.sBigTopPic)+"','" +
            mutlti.RealEscapeString(entity.sBigBottonPic)+"','" +
            mutlti.RealEscapeString(entity.sSmallTopPic)+"','" +
            mutlti.RealEscapeString(entity.sSmallBottonPic)+"','" +
            mutlti.RealEscapeString(entity.sRemark)+"','" +
            tConvert.IntToStr(entity.iState)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int SubjectDAL::GetAll(SubjectLst  & subjects)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TConvert tConvert;
    // SQL
    string sSql = "select Id,lAddTime,sName,sBigTopPic,sBigBottonPic,sSmallTopPic,sSmallBottonPic,sRemark,iState"
                  " from "+msTableName;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        while((row =mysql_fetch_row( result )) != NULL){


                Subject entity;

                int iIndex = 0;
                entity.Id = atol(row[iIndex++]);
                entity.lAddTime = atol( row[iIndex++] );
                entity.sName =  row[iIndex++];
                entity.sBigTopPic =  row[iIndex++];
                entity.sBigBottonPic =  row[iIndex++];
                entity.sSmallTopPic =  row[iIndex++];
                entity.sSmallBottonPic =  row[iIndex++];
                entity.sRemark =  row[iIndex++];
                entity.iState = atoi(row[iIndex++]);
                subjects.push_back(entity);


        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}
