#include "FansBankOrderDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "param/Param.h"

int FansBankOrderDAL::GetEntity(const string & orderNo, FansBankOrderEntity & entity)
{

    string sSql =   msAllColumn +  msTableName + " where orderNo='" + mMulti.RealEscapeString(orderNo) + "'";

    int iRet = -1;
    MYSQL_RES* result;

    BaseQueryResult(sSql,&result);
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

        }
    }
    FreeResult( result );

    return iRet;

}

//是否有正在卖出
int FansBankOrderDAL::IsExistSelling(long Id,int iTpye)
{
    TConvert tConvert;
    int iRet = -1;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select 1 from "+this->msTableName+
            " where idSell=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye)  + " and state=0";

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


//获取交易中的可买入列表
int FansBankOrderDAL::GetSelling( int iTpye ,int iPage, string sCondition ,int iCount,FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select orderNo,addTime,endTime,state,type,idSell,nameSell,price,lots from " + msTableName +
            " where type=" + tConvert.IntToStr(iTpye) + " and state=0 ";


    if( !sCondition.empty()){
        sSql += " and (idSell like '%" + multi.RealEscapeString(sCondition) + "%' or nameSell like '%" + multi.RealEscapeString(sCondition) + "')";
    }
    sSql += " limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

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


                FansBankOrderEntity entity;
                int iIndex = 0;
                entity.sOrderNo =  row[iIndex++] ;
                entity.lAddTime = atol( row[iIndex++] );
                entity.lEndTime = atol( row[iIndex++] );
                entity.iState = atoi( row[iIndex++] );
                entity.iType = atoi( row[iIndex++] );
                entity.lIdSell =  atol(row[iIndex++]) ;
                entity.sNameSell =  row[iIndex++] ;
                entity.dPrice =  atof(row[iIndex++] );
                entity.iLots = atoi( row[iIndex++] );

                lst.push_back(entity);

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

//获取根据报单数据返回挂卖列表
int FansBankOrderDAL::GetSellingByBaodanState( int iTpye ,int iPage, string sCondition ,int isBaodan,string baodanMemeber,int iCount,FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select orderNo,addTime,endTime,state,type,idSell,nameSell,price,lots from " + msTableName +
            " where type=" + tConvert.IntToStr(iTpye) + " and state=0 ";



    if( !baodanMemeber.empty()){
        if( isBaodan == 0){
            sSql += " and idSell in (" + multi.RealEscapeString(baodanMemeber) + ")";
        }else{
            sSql += " and idSell not in (" + multi.RealEscapeString(baodanMemeber) + ")";
        }
    }

    if( !sCondition.empty()){
        sSql += " and (idSell like '%" + multi.RealEscapeString(sCondition) + "%' or nameSell like '%" + multi.RealEscapeString(sCondition) + "')";
    }
    sSql += " limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

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


                FansBankOrderEntity entity;
                int iIndex = 0;
                entity.sOrderNo =  row[iIndex++] ;
                entity.lAddTime = atol( row[iIndex++] );
                entity.lEndTime = atol( row[iIndex++] );
                entity.iState = atoi( row[iIndex++] );
                entity.iType = atoi( row[iIndex++] );
                entity.lIdSell =  atol(row[iIndex++]) ;
                entity.sNameSell =  row[iIndex++] ;
                entity.dPrice =  atof(row[iIndex++] );
                entity.iLots = atoi( row[iIndex++] );

                lst.push_back(entity);

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//得到正在卖出的手数
int FansBankOrderDAL::GetLotsSell(long Id,int iTpye , int & iLots)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select lots from " +
            msTableName + " where idSell=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye) + " and state=0 FOR UPDATE";

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
                iLots = atof( row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}



//得到正在卖出的添加时间
int FansBankOrderDAL::GetLotsSellAddTime(long Id,int iTpye , long & lAddTime)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select addTime from " +
            msTableName + " where idSell=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye) + " and state=0";

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
                lAddTime = atol( row[iIndex++] );

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

int FansBankOrderDAL::Add( const FansBankOrderEntity & entity   )
{
    string  sSql = GetSqlAdd(entity);
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

string FansBankOrderDAL::GetSqlAdd( const FansBankOrderEntity & entity   )
{
    TConvert tConvert;
    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "insert into "+this->msTableName+"( orderNo,addTime,endTime,state,type,idBuy,nameBuy,idSell,nameSell,price,lots,comeFrom,turnOver)" +
            "VALUES( '"+
            tMultiMysqlDAL.RealEscapeString( entity.sOrderNo )+"','" +
            tConvert.LongToStr( entity.lAddTime ) +"','" +
            tConvert.LongToStr( entity.lEndTime) +"','" +
            tConvert.IntToStr( entity.iState) +"','" +
            tConvert.IntToStr( entity.iType) +"','" +
            tConvert.LongToStr( entity.lIdBuy) +"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sNameBuy) +"','" +
            tConvert.LongToStr( entity.lIdSell) +"','" +
            tMultiMysqlDAL.RealEscapeString( entity.sNameSell) +"','" +
            tConvert.DoubleToStr( entity.dPrice) +"','" +
            tConvert.IntToStr( entity.iLots) +"','" +
            tConvert.IntToStr( entity.comeFrom) +"','" +
            tConvert.DoubleToStr( entity.dTurnOver) +
            "')";

    return sSql;

}


//得到卖出总额
int FansBankOrderDAL::GetTotalSell(long Id,int iTpye , double & dTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    int iFansPer = Param::GetFansPerLots();

    string sSql =   "select sum(price*lots*" +  tConvert.IntToStr(iFansPer)  +") from " + msTableName + " where idSell=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye);

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    //appendlog(TTDLogger::LOG_DEBUG, "ssss:%s",sSql.c_str());
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = 0;

        if ( 0 != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                if( row[0] == NULL){
                    dTotal = 0.0;
                }else{
                    int iIndex = 0;
                    dTotal = atof( row[iIndex++] );

                }

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//得到买入总额
int FansBankOrderDAL::GetTotalBuy(long Id,int iTpye , double & dTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    int iFansPer = Param::GetFansPerLots();

    string sSql =   "select sum(price*lots*" +  tConvert.IntToStr(iFansPer)  +") from " + msTableName + " where idBuy=" + tConvert.LongToStr(Id)  + " and type=" + tConvert.IntToStr(iTpye);

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
                    dTotal = 0.0;
                }else{
                    int iIndex = 0;
                    dTotal = atof( row[iIndex++] );

                }


        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


//增加卖出的手数
int FansBankOrderDAL::InreemLots(long Id,int iTpye, int iLots)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+  " set lots=lots+" + tConvert.IntToStr(iLots) +
            " where idSell=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iTpye) + " and state=0";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//增加订单中的已经卖出手数
int FansBankOrderDAL::InreemLotsOver(long Id,int iTpye, int iLots, double dTurnOver)
{
    string sSql = GetSqlInreemLotsOver(Id,iTpye,iLots,dTurnOver);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}
//增加订单中的已经卖出手数 sql
string FansBankOrderDAL::GetSqlInreemLotsOver(long Id,int iTpye, int iLots, double dTurnOver)
{
    // SQL
    string sSql = "update "+ msTableName+  " set turnOver=turnOver+" + tConvert.DoubleToStr(dTurnOver) + ",lotsOver=lotsOver+" + tConvert.IntToStr(iLots) +
            " where idSell=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iTpye) + " and state=0";


    return sSql;

}


//减少正在卖出的手数
int FansBankOrderDAL::DeInreemLots(long Id,int iTpye, int iLots)
{

    // SQL
    string sSql = GetSqlDeInreemLots(Id,iTpye,iLots);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}

//减少正在卖出的手数
string FansBankOrderDAL::GetSqlDeInreemLots(long Id,int iTpye, int iLots)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "update "+ msTableName+  " set lots=lots-" + tConvert.IntToStr(iLots) +
            " where idSell=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iTpye) + " and state=0";

    return sSql;

}

//取消订单
int FansBankOrderDAL::CancelOrder(long Id, int iType)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    long lCurTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+ msTableName+  " set state=1,endTime=" + tConvert.LongToStr(lCurTime) +
            " where idSell=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iType) + " and state=0";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);

}


//删除正在卖出但手数为0的记录
int FansBankOrderDAL::Del(long Id, int iType)
{
    string sSql = GetSqlDel(Id,iType);

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//删除正在卖出但手数为0的记录 sql
string FansBankOrderDAL::GetSqlDel(long Id, int iType)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;

    // SQL
    string sSql = "delete from "+ msTableName+ " where idSell=" + tConvert.LongToStr(Id) + " and type=" + tConvert.IntToStr(iType) + " and state=0 and lots=0";

    return sSql;

}


//获取Id对像的成交记录
int FansBankOrderDAL::GetPageById(int iType,long Id, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn + msTableName
            + " where state=2" + " and type=" + tConvert.IntToStr(iType) +  " and (idSell=" + tConvert.LongToStr(Id)
            + " or idBuy=" + tConvert.IntToStr(Id) + ")";
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}


//获取订单(后台管理)
int FansBankOrderDAL::GetOrderByManage(int iType, int iState,long idSell, long idBuy,
                                       long startTime,long endTime ,int iPage,int iCount , FansBankOrderLst & lst, int & iTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn + msTableName;
    string sWhere =  " where 1=1 " ;
    if( iState >=0 ){
       sWhere += " and state=" + tConvert.IntToStr(iState) ;
    }
    if( iType >=0 ){
       sWhere += " and type="  + tConvert.IntToStr(iType);
    }
    if( idSell >=0 ){
       sWhere += " and idSell like '%"  + tConvert.LongToStr(idSell) + "%'";
    }
    if( idBuy >=0 ){
       sWhere += " and idBuy like '%"  + tConvert.LongToStr(idBuy) + "%'";
    }

    if( startTime >=0 && endTime >=0){
       if( iState == 1 || iState == 2){
         sWhere += " and endTime>="  + tConvert.LongToStr(startTime) + " and endTime<="  + tConvert.LongToStr(endTime);
       }else if( iState == 0){
         sWhere += " and addTime>="  + tConvert.LongToStr(startTime) + " and addTime<="  + tConvert.LongToStr(endTime);

       }
    }

    sSql += sWhere;
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    GetCount(sWhere,iTotal);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}


//获取所有卖出记录
int FansBankOrderDAL::GetAllTrade(int iType, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn + msTableName
            + " where state=2" + " and type=" + tConvert.IntToStr(iType);
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);


    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}




//获取我的正在卖出
int FansBankOrderDAL::GetPageMySalling(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL
    if( sIds.empty()){
        return 0;
    }

    string sSql =   msAllColumn + msTableName
            + " where state=0" + " and type=" + tConvert.IntToStr(iType) +  " and idSell in(" + sIds + ")";
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

//获取我的取消
int FansBankOrderDAL::GetPageMyCancel(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn + msTableName
            + " where state=1" + " and type=" + tConvert.IntToStr(iType) +" and idSell in(" + sIds + ")";
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}
//获取我的已卖出
int FansBankOrderDAL::GetPageMyOk(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn + msTableName
            + " where state=2" + " and type=" + tConvert.IntToStr(iType) +" and idSell in(" + sIds + ")";
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

//获取DasaiBaodan已卖出
int FansBankOrderDAL::GetPageDasaiBaodanOk( int iPage,int iCount ,string sOrderNo ,FansBankOrderLst & lst,int &iTotal)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sWhere;
    sWhere  =" where state=2 and comeFrom=1 and orderNo like '%" + multi.RealEscapeString(sOrderNo) + "%'";
    string sSql =   msAllColumn + msTableName
            + sWhere;
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    GetCount(sWhere,iTotal);
    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}

//获取today id已卖total
int FansBankOrderDAL::GeTodaySelledLots(int iType,string sIds,int  & iCount)
{
    string sSql =    " where state=2 and type=" + tConvert.IntToStr(iType) +" and idSell in(" + sIds + ")";
    sSql += " and left( from_unixtime(endTime/1000000),10)='" +  mtConvert.GetCurrentDay() + "'";
    return GetSum("lots",sSql,iCount);

}

//获取我买入的
int FansBankOrderDAL::GetPageMyBuy(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   msAllColumn +     msTableName
            + " where state=2" + " and type=" + tConvert.IntToStr(iType) +" and idBuy in(" + sIds + ")";
    sSql += " order by endTime desc limit " + tConvert.IntToStr((iPage-1)*iCount) + "," + tConvert.IntToStr(iCount);

    int iRet = -1;
    iRet = Fetch(sSql,lst);
    return iRet;

}


int FansBankOrderDAL::Fetch( string sSql ,FansBankOrderLst & lst){


    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    int iRet = -1;
    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;

        while ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;

                int iIndex = 0;
                FansBankOrderEntity entity;
                entity.sOrderNo = row[iIndex++] ;
                entity.lAddTime = atol( row[iIndex++] );
                entity.lEndTime = atol(row[iIndex++] );
                entity.iState = atoi(row[iIndex++]) ;
                entity.iType =atoi( row[iIndex++] );
                entity.lIdBuy =atol( row[iIndex++] );
                entity.sNameBuy = row[iIndex++] ;
                entity.lIdSell = atol(row[iIndex++] );
                entity.sNameSell = row[iIndex++] ;
                entity.dPrice = atof( row[iIndex++] );
                entity.iLots = atoi(row[iIndex++]) ;
                entity.dTurnOver = atof( row[iIndex++] );
                entity.iLotsOver = atoi(row[iIndex++]) ;

                lst.push_back(entity);


        }
    }

    con->FreeResult( result );

    return iRet;

}


//获取某天交易额
int FansBankOrderDAL::GetOneDayTrade(long Id,int iType, string sDay , double &  dOut)
{
    // SQL

    long lStartTime = tConvert.StrToTime((sDay + " 00:00:00").c_str());
    long lEndTime = tConvert.StrToTime((sDay + " 23:59:59").c_str());
    lStartTime = lStartTime*1000000;
    lEndTime = lEndTime*1000000;


    string sSql =   "select sum(price*lots*"  +  tConvert.IntToStr(Param::GetFansPerLots())   + ")  from " + msTableName + " where state=2 and idSell=" + tConvert.LongToStr(Id)
            + " and type="  + tConvert.IntToStr(iType) + " and endTime<=" + tConvert.LongToStr(lEndTime)+ " and endTime>=" + tConvert.LongToStr(lStartTime);

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
                    dOut = 0.0;
                }else{
                  int iIndex = 0;
                  dOut = atof( row[iIndex++] );
                }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}



//获取某个时间点之后的挂卖的条数
int FansBankOrderDAL::GetNewCount(long lUserId,long lLastTime,int iType, int &  iCount)
{



    string sSql =   "select count(*) from " + msTableName +
            " where state=0 and type=" + tConvert.IntToStr(iType) +
            " and addTime>" + tConvert.LongToStr(lLastTime) + " and idSell!=" + tConvert.LongToStr(lUserId);

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
                    iCount = 0;
                }else{
                  int iIndex = 0;
                  iCount = atoi( row[iIndex++] );
                }
        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}

