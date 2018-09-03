#include "UserStateDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"

//创建表
int UserStateDAL::CreateTable()
{
    // SQL
    string sSql = "create table if not exists "+this->msTableName+" ("+
            "sPrimaryKey VARCHAR(50) NOT NULL,"+
            "lUserId BIGINT NOT NULL,"+
            "lAddTime BIGINT NOT NULL,"+
            "lUpTime BIGINT NOT NULL,"+
            "iState INT NOT NULL,"+
            "lUpdateId BIGINT NOT NULL,"+
            "iSync INT NOT NULL,"+

            "sVersion VARCHAR(8) NOT NULL,"+
            "dGpsLon DOUBLE(13,10) NOT NULL,"+
            "dGpsLat DOUBLE(13,10) NOT NULL,"+
            "sNickName VARCHAR(50) NOT NULL,"+
            "iSex INT NOT NULL,"+
            "sSignature VARCHAR(50) NOT NULL,"+
            "sHeadImage VARCHAR(200) NOT NULL,"+
            "lLoginTime BIGINT default 0,"+
            "PRIMARY KEY(sPrimaryKey),UNIQUE KEY (lUserId)"+
            ") ENGINE=INNODB DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//状态-更新
int UserStateDAL::Insert( UserStateEntity& pEntity  )
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    pEntity.lAddTime = lCurrentTime;
    pEntity.lUpTime = lCurrentTime;
    pEntity.lUpdateId = lCurrentTime;
    pEntity.iState = 0;
    pEntity.iSync = 0;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( sPrimaryKey,lUserId,lAddTime,lUpTime,iState,lUpdateId,iSync," +
            "sVersion,dGpsLon,dGpsLat,sNickName,iSex,sSignature,sHeadImage)" +
            "VALUES( '"+pEntity.sPrimaryKey+"','" +
            tConvert.LongToStr(pEntity.lUserId)+"','" +
            tConvert.LongToStr(pEntity.lAddTime)+"','" +
            tConvert.LongToStr(pEntity.lUpTime)+"','" +
            tConvert.IntToStr(pEntity.iState)+"','" +
            tConvert.LongToStr(pEntity.lUpdateId)+"','" +
            tConvert.IntToStr(pEntity.iSync)+"','" +

            pEntity.sVersion+"','" +
            tConvert.DoubleToStr(pEntity.dGpsLon)+"','" +
            tConvert.DoubleToStr(pEntity.dGpsLat)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sNickName)+"','" +
            tConvert.IntToStr(pEntity.iSex)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sSignature)+"','" +
            tMultiMysqlDAL.RealEscapeString(pEntity.sHeadImage)+
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
/*功能：附近的人，公里
 * 参数：　dDistance --距离之内　iCount --距离之内最近的最多人数
 * pdGpsLat 纬度,pdGpsLon 纬度
*/
int UserStateDAL::GetNeerBy(const long plUserId,double pdGpsLat, double pdGpsLon,double pdDistance,int piCount,int piSexType, vector<UserStateEntity>& pUserStateVector)
{
    int iRet = -1;
    TConvert tConvert;
    //appendlog(TTDLogger::LOG_ERROR,"UserStateBLL::GetNeerBy %f",pdGpsLat);
    //char buf[1024] = {0};

    //sprintf(buf,"select lUserId,sNickName,iSex,sSignature,dGpsLon,dGpsLat,lUpTime,sHeadImage,( 6378140 * acos( cos( radians(%f) ) * cos( radians( dGpsLat ) ) "
    //            "* cos( radians( dGpsLon ) - radians(%f) ) + sin( radians(%f) ) * sin( radians( dGpsLat ) ) ) ) "
    //            "as lDistance from %s where lUserId<>%ld HAVING lDistance < %f order by lDistance limit 0, %d",pdGpsLat,pdGpsLon,pdGpsLat,this->msTableName.c_str(),plUserId,pdDistance,piCount);

    string sSql = " select lUserId,sNickName,iSex,sSignature,dGpsLon,dGpsLat,lUpTime,sHeadImage,( 6378140 * acos( cos( radians("+tConvert.DoubleToStr(pdGpsLat)+") ) * cos( radians( dGpsLat ) ) ";
    sSql += " * cos( radians( dGpsLon ) - radians("+tConvert.DoubleToStr(pdGpsLon)+") ) + sin( radians("+tConvert.DoubleToStr(pdGpsLat)+") ) * sin( radians( dGpsLat ) ) ) ) ";
    sSql += " as lDistance from "+this->msTableName+" ";
    sSql += " where lUserId<>"+tConvert.LongToStr(plUserId);
    if ( 0 == piSexType ) {
        sSql += " and iSex=0";
    } else if ( 1 == piSexType ) {
        sSql += " and iSex=1";
    }
    sSql += " HAVING lDistance < "+tConvert.DoubleToStr(pdDistance)+" order by lDistance limit 0, "+tConvert.IntToStr(piCount);

    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);
    //appendlog(TTDLogger::LOG_ERROR,"UserStateBLL::GetNeerBy 11");

    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {

                int iIndex = 0;

                UserStateEntity entity;

                entity.lUserId = atol( row[iIndex++] );
                entity.sNickName = row[iIndex++];
                entity.iSex = atoi( row[iIndex++] );
                entity.sSignature = row[iIndex++];
                entity.dGpsLon = tConvert.StrToDouble( row[iIndex++] );
                entity.dGpsLat = tConvert.StrToDouble( row[iIndex++] );
                entity.lUpTime = atol( row[iIndex++] );
                entity.sHeadImage = atol( row[iIndex++] );
                entity.lDistance = atol( row[iIndex++] );

                pUserStateVector.push_back(entity);

                row = mysql_fetch_row( result );
            }
        }
    }

    // 释放内存
    con->FreeResult( result );

    return iRet;
}

//获取最近的登录时间
int UserStateDAL::GetLoginTime(const long plUserId, long & plLoginTime)
{
    // SQL
    TConvert tConvert;
    string sSql = "select lLoginTime  from " + this->msTableName + " ";
    sSql += " where iState<>'333' ";
    sSql += " and lUserId = '" + tConvert.LongToStr(plUserId) + "' ";

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            iRet = 0;
            plLoginTime = atol( row[0]);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

int UserStateDAL::SetLoginTime(const long & plUserId, const long & plLoginTime)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    bool bExist = false;
    IsExistByUserId(tConvert.LongToStr( plUserId ),bExist);
    if(!bExist){
        UserStateEntity entity;
        entity.lUserId = plUserId;
        entity.lLoginTime = plLoginTime;
        entity.sPrimaryKey = tConvert.LongToStr(entity.lUserId);
        Insert(entity);
    }
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",lLoginTime = '" + tConvert.LongToStr(plLoginTime) + "' ";
    sSql += "where lUserId = '"+tConvert.LongToStr(plUserId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//状态-更新
int UserStateDAL::Update(UserStateEntity& pEntity)
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set "+
            "lUpdateId = '" + tConvert.LongToStr(lCurrentTime) + "' "+
            ",lUpTime = '" + tConvert.LongToStr(lCurrentTime) + "' ";
    sSql += ",dGpsLon = '" + tConvert.DoubleToStr(pEntity.dGpsLon) + "' ";
    sSql += ",dGpsLat = '" + tConvert.DoubleToStr(pEntity.dGpsLat) + "' ";
    sSql += ",iSex = '" + tConvert.IntToStr(pEntity.iSex) + "' ";
    sSql += ",sHeadImage = '" + pEntity.sHeadImage + "' ";
    sSql += ",sNickName = '" + pEntity.sNickName + "' ";
    sSql += ",sSignature = '" + pEntity.sSignature + "' ";
    sSql += ",sVersion = '" + pEntity.sVersion + "' ";
    sSql += "where lUserId = '"+tConvert.LongToStr(pEntity.lUserId)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//记录是否存在
int UserStateDAL::IsExistByUserId( const string& psUserId, bool& pbIsBool )
{
    pbIsBool = false;
    // SQL
    string sSql = "select 1 from "+this->msTableName+" where lUserId='"+psUserId+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}

