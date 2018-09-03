#include "FansBankSallingDAL.h"

#include "log/Logger.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"









//获取交易中的可买入列表
int FansBankSallingDAL::GetSelling( int iTpye ,int iPage, string sCondition ,int iCount,SallingLst & lst)
{
    TConvert tConvert;
    TMultiMysqlDAL multi;
    // SQL

    string sSql =   "select orderNo,addTime,endTime,state,type,idSell,nameSell,price,lots from " + msTableName +
            " where type=" + tConvert.IntToStr(iTpye);


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

        if ( NULL != (row = mysql_fetch_row( result )) ) {

                iRet = 0;


                SallingEntity entity;
                int iIndex = 0;
                entity.sOrderNo =  row[iIndex++] ;
                entity.lAddTime = atol( row[iIndex++] );
                entity.lEndTime = atol( row[iIndex++] );
                entity.iState = atoi( row[iIndex++] );
                entity.iType = atoi( row[iIndex++] );
                entity.lIdSell =  atol(row[iIndex++]) ;
                entity.sName =  row[iIndex++] ;
                entity.dPrice =  atof(row[iIndex++] );
                entity.iLots = atoi( row[iIndex++] );

                lst.push_back(entity);

        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}





