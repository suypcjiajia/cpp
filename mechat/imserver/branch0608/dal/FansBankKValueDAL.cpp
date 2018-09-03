#include "FansBankKValueDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"



int FansBankKValueDAL::IsExist(long Id,int iTpye, const string & sDay)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select 1 from "+this->msTableName+ " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

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

int FansBankKValueDAL::Add( const KValueEntity & entity   )
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( price,open,close,hight,low,yclose,turnOver,turnRate,id,type,day,name)" +
            "VALUES( '"+
            tConvert.DoubleToStr( entity.price )+"','" +
            tConvert.DoubleToStr( entity.open ) +"','" +
            tConvert.DoubleToStr( entity.close) +"','" +
            tConvert.DoubleToStr( entity.hight) +"','" +
            tConvert.DoubleToStr( entity.low) +"','" +
            tConvert.DoubleToStr( entity.yclose) +"','" +
            tConvert.DoubleToStr( entity.turnOver) +"','" +
            tConvert.DoubleToStr( entity.turnRate) +"','" +
            tConvert.LongToStr( entity.Id) +"','" +
            tConvert.IntToStr( entity.iType) +"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sDay) +"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sName) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//获取某天的一条K柱
int FansBankKValueDAL::GetEntity(long Id,int iTpye, const string & sDay, KValueEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn +  msTableName + " where id=" + tConvert.LongToStr(Id) +
            " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

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

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                TransToStruct(entity,row);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//获取最新的一条K柱
int FansBankKValueDAL::GetLast(long Id,int iTpye, KValueEntity & entity)
{

    // SQL

    string sSql =  msAllColumn + msTableName +
            " where id=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye) + " order by day desc limit 1";

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

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                TransToStruct(entity,row);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

void FansBankKValueDAL::TransToStruct(KValueEntity & entity,MYSQL_ROW row)
{
    int iIndex = 0;
    entity.price = atof( row[iIndex++] );
    entity.open = atof( row[iIndex++] );
    entity.close = atof( row[iIndex++] );
    entity.hight = atof( row[iIndex++] );
    entity.low = atof( row[iIndex++] );
    entity.turnOver = atof( row[iIndex++] );
    entity.turnRate = atof( row[iIndex++] );
    entity.sName = row[iIndex++];
    entity.yclose = atof( row[iIndex++] );

}

//获取前20条K线图
int FansBankKValueDAL::GetKs(long Id,int iTpye, KValueLst & Ks)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =  msAllColumn +
            msTableName + " where id=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iTpye)
            + " limit 20";

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

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                KValueEntity entity;

                TransToStruct(entity,row);

                Ks.push_back(entity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

//获取全部行情（按分页）
/*
int FansBankKValueDAL::GetPage( int iTpye,int iPage, int iCount,string sCondition,KValueLst & Ks )
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select id,price,open,close,hight,low,turnOver,turnRate,name from " +
            msTableName + " where day='" + tConvert.GetCurrentDay() + "' and type=" + tConvert.IntToStr(iTpye);

    if( !sCondition.empty()){
        sSql += " and (id like '%" + multi.RealEscapeString(sCondition) + "%' or name like '%" + multi.RealEscapeString(sCondition) + "%')";
    }
    sSql += " order by price desc limit " + tConvert.IntToStr( (iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

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

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                int iIndex = 0;
                KValueEntity entity;
                entity.Id = atol( row[iIndex++] );
                entity.price = atof( row[iIndex++] );
                entity.open = atof( row[iIndex++] );
                entity.close = atof( row[iIndex++] );
                entity.hight = atof( row[iIndex++] );
                entity.low = atof( row[iIndex++] );
                entity.turnOver = atof( row[iIndex++] );
                entity.turnRate = atof( row[iIndex++] );
                entity.sName = row[iIndex++];

                Ks.push_back(entity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}
*/

//获取全部行情（按分页）
int FansBankKValueDAL::GetPage( int iTpye,int iPage, int iCount,string sCondition,KValueLst & Ks )
{


    //select a.day ,a.id,a.price,a.open,a.close,a.hight,a.low,a.turnOver,a.turnRate,a.name from
    //fansbank_k_value as a inner join user_datadal as b where a.day='2017-08-08'
    //and a.type=1 and a.id=b.lCurrentPlatformId  group by a.id having count(b.lCurrentPlatformId) >= 100 order by a.price desc limit 5,10;
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select a.id,a.price,a.open,a.close,a.hight,a.low,a.turnOver,a.turnRate,a.name,a.yclose,a.price-a.yclose as uprice from " +
            msTableName + " as a inner join user_datadal as b where a.day='" + tConvert.GetCurrentDay()
            + "' and a.id>=50000 and a.type=" + tConvert.IntToStr(iTpye) + " and a.id=";
    if( iTpye == enum_kvalue_type_platform){
        sSql += "b.lCurrentPlatformId";
    }else{
        sSql += "b.lPushManId";
    }

    if( !sCondition.empty()){
        sSql += " and (a.id like '%" + multi.RealEscapeString(sCondition) + "%' or a.name like '%" + multi.RealEscapeString(sCondition) + "%')";
    }
    sSql += " group by a.id having count(*) >="  +  tConvert.IntToStr( Param::GetFansPerLots() ) + " order by uprice desc limit " + tConvert.IntToStr( (iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    //appendlog(TTDLogger::LOG_INFO, "xxx:%s",sSql.c_str());
    result =  con->QueryResult(sSql);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                int iIndex = 0;
                KValueEntity entity;
                entity.Id = atol( row[iIndex++] );
                entity.price = atof( row[iIndex++] );
                entity.open = atof( row[iIndex++] );
                entity.close = atof( row[iIndex++] );
                entity.hight = atof( row[iIndex++] );
                entity.low = atof( row[iIndex++] );
                entity.turnOver = atof( row[iIndex++] );
                entity.turnRate = atof( row[iIndex++] );
                entity.sName = row[iIndex++];
                entity.yclose = atof( row[iIndex++] );

                Ks.push_back(entity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//更新日K线当前价格
int FansBankKValueDAL::UpdatePrice(long Id,int iTpye, const string & sDay, double dPrice)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+" set price=" + tConvert.DoubleToStr(dPrice) +
            " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}




//更新日K线开盘价
int FansBankKValueDAL::UpdateOpen(long Id,int iTpye, const string & sDay, double dPrice)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+" set open=" + tConvert.DoubleToStr(dPrice) +
            " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//更新日K线收盘价
int FansBankKValueDAL::UpdateClose(long Id,int iTpye, const string & sDay, double dPrice)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+" set close=" + tConvert.DoubleToStr(dPrice) +
            " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//更新日K线最高价
int FansBankKValueDAL::UpdateHigh(long Id,int iTpye, const string & sDay, double dPrice)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+" set hight=" + tConvert.DoubleToStr(dPrice) +
            " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//更新日K线最低价
int FansBankKValueDAL::UpdateLow(long Id,int iTpye, const string & sDay, double dPrice)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+" set low=" + tConvert.DoubleToStr(dPrice) +
            " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//获取移动平均值（ma值）
int FansBankKValueDAL::GetMaAvg(long Id,int iType, string sDay , int iMa, double &  dAvg)
{
    TConvert tConvert;
    long lTime = tConvert.StrToTime( (sDay + " 00:00:00").c_str());
    string sMaDay = tConvert.TimeToStr(lTime - 3600*24*iMa).substr(0,10);


    string sSql =   "select avg(close)  from " + msTableName + " where id=" + tConvert.LongToStr(Id)
            + " and type="  + tConvert.IntToStr(iType) + " and day<='" + multi.RealEscapeString(sDay)+
            "' and day>='" + sMaDay + "'";

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

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;
                if( row[0] == NULL){
                    dAvg = 0.0;
                }else{
                  int iIndex = 0;
                  dAvg = atof( row[iIndex++] );
                }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//Increase均价
int FansBankKValueDAL::IncreaseValue(double dValue, long id,int iType)
{
    //update  fansbank_k_value set price=4.00 where day='2018-04-18' and id= and type=
    TConvert tConvert;
    string day = tConvert.GetCurrentDay();

    // SQL
    string sSql = "update "+ msTableName+" set price=price+"
            + tConvert.DoubleToStr(dValue)  + " and type=" + tConvert.IntToStr(iType)
            + " and id=" + tConvert.LongToStr(id)
            + " and day='" + day + "'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


