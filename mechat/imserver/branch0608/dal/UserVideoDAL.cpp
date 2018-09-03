#include "UserVideoDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//
int UserVideoDAL::CreateTable()
{

    string sSql = "create table if not exists "+this->msTableName+" ("+
            "id BIGINT NOT NULL AUTO_INCREMENT,"+
            "addTime BIGINT NOT NULL,"+
            "type INT NOT NULL default 0,"+
            "url VARCHAR(1024) NOT NULL,"+
            "title VARCHAR(128) NOT NULL default '',"+
            "icon VARCHAR(128) NOT NULL default '',"+
            "content VARCHAR(1024) NOT NULL default '',"+
            "PRIMARY KEY(id)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    return BaseQuery(sSql);
}
//
int UserVideoDAL::Insert( const UserVideoEntity & entity )
{

    string sSql;
    sSql = "insert into "+ msTableName+"( addTime,type,url,title,icon,content)" +
            "VALUES( '"+mtConvert.LongToStr(entity.addTime)+"','" +
            mtConvert.IntToStr(entity.type)+"','" +
            mMulti.RealEscapeString(entity.url)+"','" +
            mMulti.RealEscapeString(entity.title)+"','" +
            mMulti.RealEscapeString(entity.icon)+"','" +
            mMulti.RealEscapeString(entity.content)+
            "')";
    return BaseQuery(sSql);
}


int UserVideoDAL::GetData(const string & sType ,UserVideoLst & lst)
{

    if(sType.empty())
        return 0;

    int iRet = -1;
    MYSQL_RES* result;
    string sSql = "select " + msAllColumn +  " from "+msTableName;;
    string sCond = " where type in(" + sType + ")" ;

    sSql += sCond;


    BaseQueryResult( sSql, &result);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;
        while ( NULL != (row = mysql_fetch_row(result) )) {

            UserVideoEntity entity;

            int iIndex = 0;

            entity.addTime = atol( row[iIndex++]);
            entity.type = atoi( row[iIndex++] );
            entity.url = row[iIndex++];
            entity.title = row[iIndex++];
            entity.icon = row[iIndex++];
            entity.content = row[iIndex++];

            lst.push_back(entity);
        }
    }

    // 释放内存
    FreeResult( result );
    return iRet;

}


