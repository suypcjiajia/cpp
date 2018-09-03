#include "ClassDAL.h"



ClassDAL::ClassDAL()
{

}


//自动创建表
int ClassDAL::CreateTable()
{
    ScopeLock lock( this->mMutex );
    // SQL
    string sSql = "create table if not exists "+msTableName+" ("+
            "Id INT NOT NULL,"+
            "sName VARCHAR(64) NOT NULL,"+
            "sBigPic VARCHAR(1024) NOT NULL default '' ,"+
            "sSmallPic VARCHAR(1024) NOT NULL default '',"+
            "sZone VARCHAR(1024) NOT NULL default '',"+
            "iLevel INT NOT NULL default 1,"+
            "sAction VARCHAR(1024) NOT NULL default '' "+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}


int ClassDAL::replace( const NearCalss& entity  )
{
    //ScopeLock lock( this->mMutex );
    TConvert tConvert;
    TMultiMysqlDAL mutlti;
    // SQL
    string sSql   = "replace into " +  msTableName+"( Id,sName,sBigPic,sSmallPic,sAction)" +
            "VALUES( '"+tConvert.LongToStr( entity.Id)+"','" +
            mutlti.RealEscapeString(entity.sName)+"','" +
            mutlti.RealEscapeString(entity.sBigPic)+"','" +
            mutlti.RealEscapeString(entity.sSmallPic)+"','" +
            mutlti.RealEscapeString(entity.sAction)+
            "')";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    int iRet = -1;
    iRet =  con->Query(sSql);
    return iRet;
}

int ClassDAL::GetAll(string sZone, NearCalssLst  & subjects)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "select Id,sName,sBigPic,sSmallPic,sAction"
                  " from "+msTableName + " where  sZone='' or sZone like '%" + multi.RealEscapeString(sZone) + "%'";
    sSql += " order by iLevel desc limit 8";
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        while((row =mysql_fetch_row( result )) != NULL){


                NearCalss entity;

                int iIndex = 0;
                entity.Id = atol(row[iIndex++]);
                entity.sName =  row[iIndex++];
                entity.sBigPic =  row[iIndex++];
                entity.sSmallPic =  row[iIndex++];
                entity.sAction =  row[iIndex++];

                subjects.push_back(entity);


        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}


int ClassDAL::GetAllCalss(NearCalssLst  & subjects)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    string sSql = "select Id,sName,sBigPic,sSmallPic,sAction"
                  " from "+msTableName ;
    sSql += " order by iLevel desc";
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        while((row =mysql_fetch_row( result )) != NULL){


                NearCalss entity;

                int iIndex = 0;
                entity.Id = atol(row[iIndex++]);
                entity.sName =  row[iIndex++];
                entity.sBigPic =  row[iIndex++];
                entity.sSmallPic =  row[iIndex++];
                entity.sAction =  row[iIndex++];

                subjects.push_back(entity);


        }
    }
    // 释放内存
    con->FreeResult( result );
    return iRet;
}
