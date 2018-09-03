#include "KValueDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"



int KValueDAL::IsExist(long Id,int iTpye, const string & sDay)
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

int KValueDAL::Add( const KValueEntity & entity   )
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( price,open,close,hight,low,yclose,turnOver,turnRate,id,type,name,day)" +
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
            tMultiMysqlDAL.RealEscapeString( entity.sName) +"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sDay) +
            "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


int KValueDAL::GetEntity(long Id,int iTpye, const string & sDay, KValueEntity & entity)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select price,open,close,hight,low,turnOver,turnRate,name from " + msTableName + " where id=" + tConvert.LongToStr(Id) +  " and day='" +  multi.RealEscapeString(sDay) + "' and type=" + tConvert.IntToStr(iTpye);

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

                int iIndex = 0;
                entity.price = atof( row[iIndex++] );
                entity.open = atof( row[iIndex++] );
                entity.close = atof( row[iIndex++] );
                entity.hight = atof( row[iIndex++] );
                entity.low = atof( row[iIndex++] );
                entity.turnOver = atof( row[iIndex++] );
                entity.turnRate = atof( row[iIndex++] );
                entity.sName = row[iIndex++] ;
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

//更新日K线当前价格
int KValueDAL::UpdatePrice(long Id,int iTpye, const string & sDay, double dPrice)
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
int KValueDAL::UpdateOpen(long Id,int iTpye, const string & sDay, double dPrice)
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
int KValueDAL::UpdateClose(long Id,int iTpye, const string & sDay, double dPrice)
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
int KValueDAL::UpdateHigh(long Id,int iTpye, const string & sDay, double dPrice)
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
int KValueDAL::UpdateLow(long Id,int iTpye, const string & sDay, double dPrice)
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

