#include "PublicDataDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"


/**
 *Copyright(C),TTD
 * 公众号资料数据访问
 * @author suyp
 * @modify suyp
 */



/**
 * 创建表格
 * @param
 * @return
 */
int PublicDataDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "Id BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "sName VARCHAR(128) NOT NULL default '',"+
            "lMasterId BIGINT NOT NULL," +
            "sHead VARCHAR(1024) NOT NULL default '',"
            "primary key(Id),"+
            "constraint FK_publicdata_lmasterid foreign key(lMasterId) references user_datadal(lUserId)" +
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";

    return BaseQuery(sSql);

}

/**
 * 是否存在
 * @param Id 公众号
 * @return 0 存在
 */
int PublicDataDAL::IsExist(long  Id)
{
    TConvert tConvert;
    int iRet = -1;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where Id='"+ tConvert.LongToStr(Id)   +"' limit 1;";

    MYSQL_RES* result = NULL;

    BaseQueryResult(sSql,&result);


    if ( NULL != result && result->row_count > 0) {
        iRet = 0; //exist
    }else if( NULL != result ){
        iRet = 1;//no exist
    }

    FreeResult(result);

    return iRet;

}


/**
 * 添加一条记录
 * @param entity
 * @return 0 sucess
 */
int PublicDataDAL::AddData( const PublicDataEntity & entity)
{
    TConvert tConvert;
    //TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( Id,lAddTime,lUpTime," +
            "sName,sHead,lMasterId)" +
            "VALUES( '"+
            tConvert.LongToStr(entity.Id)+"','" +
            tConvert.LongToStr(entity.lAddTime)+"','" +
            tConvert.LongToStr(entity.lUpTime)+"','" +
            entity.sName + "','" +
            entity.sHead + "','" +
            tConvert.LongToStr(entity.lMasterId) +
            "')";

    return BaseQuery(sSql);

}


/**
 * 添加一条记录
 * @param id 公众号
 * @param entity
 * @return 0 sucess -5 数据不存在
 */
int PublicDataDAL::GetEntity(long  id,PublicDataEntity & entity)
{
    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result = NULL;

    // SQL
    string sSql = "select Id,lAddTime,lUpTime,sName,sHead,lMasterId from " + msTableName;

    string sCond = " where  ";

    sCond += " Id= '" + tConvert.LongToStr( id) + "'";


    sSql += sCond;

    iRet = BaseQueryResult(sSql,&result);


    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = -5;

        if ( NULL != (row = mysql_fetch_row(result) )) {


            int iIndex = 0;

            entity.Id = atol( row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol(row[iIndex++]);
            entity.sName = row[iIndex++];
            entity.sHead = row[iIndex++];
            entity.lMasterId = atol(row[iIndex++]);

            iRet = 0;


        }
    }

    FreeResult(result);

    return iRet;

}

/**
 * 更新名称
 * @param id 公众号
 * @param sValue 头像
 * @return 0 sucess
 */
int PublicDataDAL::UpdateName(long Id, string sValue)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sName ='" + mutlidal.RealEscapeString(sValue) + "'";

    sSql += " where Id='"+ tConvert.LongToStr(Id)  + "'";

    return BaseQuery(sSql);

}

/**
 * 更新头像
 * @param id 公众号
 * @param sValue 头像
 * @return 0 sucess
 */
int PublicDataDAL::UpdateHead(long Id, string sValue)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL mutlidal;
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            " lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";

    sSql += ",sHead ='" + mutlidal.RealEscapeString(sValue) + "'";

    sSql += " where Id='"+ tConvert.LongToStr(Id)  + "'";

    return BaseQuery(sSql);

}
