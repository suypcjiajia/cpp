#include "ShopInfoDAL.h"
#include "dal/TMysqlDAL.h"
#include "log/Logger.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"
#include "dal/MysqlConnect.h"
#include "ErrDef.h"
#include "stdio.h"
#include "UserDataEntity.h"
#include "TypeDef.h"
ShopInfoDAL* ShopInfoDAL::mInstance = NULL;
string mysqlhost_mechat = "rm-wz92t4rr5j7cbg964.mysql.rds.aliyuncs.com";
string mysqlhost_wallet = "rm-wz9c618cpa59o812l.mysql.rds.aliyuncs.com";

//string mysqlhost_mechat = "127.0.0.1";
//string mysqlhost_wallet = "127.0.0.1";

ShopInfoDAL* ShopInfoDAL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new ShopInfoDAL;
    }
    return mInstance;
}

int ShopInfoDAL::GetList(   UserDataList & plstOut)
{
    TMultiMysqlDAL multi(mysqlhost_mechat,"mechat","Mechat1234");
    multi.ConnectDB("mechatdb");
    string sSql = "select lUserId,lCurrentPlatformId from user_datadal where lPushManId=72";

    MYSQL_RES* result;
    result =  multi.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){


        while ( NULL != (row = mysql_fetch_row(result) )) {
            UserDataEntity en;
            int iIndex = 0;

            en.lUserId = atol(row[iIndex++]);
            en.lCurrentPlatformId =  atol(row[iIndex++]);
            plstOut.push_back(en);
        }

    }

    if( result != NULL){
       multi.FreeResult( result );
    }
    multi.CloseDB();
    return 0;
}

long ShopInfoDAL::GetMasterId( long lPlatformId)
{
    TMultiMysqlDAL multi(mysqlhost_wallet,"wallet","Wallet1234");
    multi.ConnectDB("walletdb");
    TConvert tConvert;
    string sSql = "select lUserId from shopdal where sPrimaryKey=" + tConvert.LongToStr(lPlatformId);

    MYSQL_RES* result;
    result =  multi.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    long l;

    if( result != NULL){



        if ( NULL != (row = mysql_fetch_row(result) )) {
            UserDataEntity en;
            int iIndex =0;

            l = atol(row[iIndex++]);

        }

    }

    if( result != NULL){
       multi.FreeResult( result );
    }
    multi.CloseDB();
    return l;
}

//获取所有区域
long ShopInfoDAL::GetZones( StringLst & zones)
{
    TMultiMysqlDAL multi(mysqlhost_wallet,"wallet","Wallet1234");
    multi.ConnectDB("walletdb");

    string sSql = "select sCity from shopdal where sCity!='美支付供用商' and iVerify=1 group by sCity";

    MYSQL_RES* result;
    result =  multi.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    long l;

    if( result != NULL){



        while ( NULL != (row = mysql_fetch_row(result) )) {
            UserDataEntity en;
            int iIndex =0;

            string sCity = row[iIndex++];
            zones.push_back(sCity);

        }

    }

    if( result != NULL){
       multi.FreeResult( result );
    }
    multi.CloseDB();
    return l;
}


//获取区域下的所有平台Id
long ShopInfoDAL::GetPlatformIdsByZone( string  zone, LongLst & platformIds)
{
    TMultiMysqlDAL multi(mysqlhost_wallet,"wallet","Wallet1234");
    multi.ConnectDB("walletdb");

    string sSql = "select sPrimaryKey from shopdal where sCity!='美支付供用商' and iVerify=1 and sCity='" + zone + "'";

    MYSQL_RES* result;
    result =  multi.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    long l;

    if( result != NULL){



        while ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex =0;

            long  platformId = atol(row[iIndex++]);
            platformIds.push_back(platformId);

        }

    }

    if( result != NULL){
       multi.FreeResult( result );
    }
    multi.CloseDB();
    return l;
}


//获取区域下的粉丝粉量
long ShopInfoDAL::GetCountByZone( string day, LongLst & platformIds)
{
    TMultiMysqlDAL multi(mysqlhost_mechat,"mechat","Mechat1234");
    multi.ConnectDB("mechatdb");

    string ids;TConvert t;
    for(LongLst::iterator it =  platformIds.begin(); it != platformIds.end(); it++){

        ids += t.LongToStr(*it) + ",";
    }
    if( ids.empty())
        return 0;
    else
        ids = ids.substr(0,ids.length() -1);

    string sSql = "select count(*) from user_datadal where lCurrentPlatformId in(" + ids  + ") and left(from_unixtime(lAddTime/1000000),10)>='" + day + "'" ;

    MYSQL_RES* result;
    result =  multi.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    long l;

    if( result != NULL){



        if ( NULL != (row = mysql_fetch_row(result) )) {

            int iIndex =0;

            return atol(row[iIndex++]);


        }

    }

    if( result != NULL){
       multi.FreeResult( result );
    }
    multi.CloseDB();
    return l;
}

int ShopInfoDAL::Update(   long lUserId,long lPushId)
{
    TMultiMysqlDAL multi(mysqlhost_mechat,"mechat","Mechat1234");
    multi.ConnectDB("mechatdb");

    TConvert tConvert;
    string sSql = "update user_datadal set lPushManId="   +  tConvert.LongToStr(lPushId) + " where lUserId="  + tConvert.LongToStr(lUserId);

    multi.Query(sSql);
    multi.CloseDB();
    return 0;
}

int ShopInfoDAL::UpdateWeixinNum(   const string & s)
{
    TMultiMysqlDAL multi(mysqlhost_mechat,"mechat","Mechat1234");
    multi.ConnectDB("mechatdb");

    TConvert tConvert;
    string sSql = "update user_datadal set sWeixinNum='fans11111111' where sWeixinNum='' and lUserId in("  + s +")";

    multi.Query(sSql);
    multi.CloseDB();
    return 0;
}



int ShopInfoDAL::GetList(  int piIndex, int piCount, ShopInfoList & plstOut)
{

    TMultiMysqlDAL multdal(mysqlhost_wallet,"wallet","Wallet1234");
    multdal.ConnectDB("walletdb");

    TConvert tConvert;

    int iRet = -1;
    MYSQL_RES* result;

    TMultiMysqlDAL multi;

    // SQL
    string sSql = "select sPrimaryKey,lAddTime,lUpTime,iState,lUpdateId,"
                  "lUserId,sAccount,sPassWord,sShopName,sCompanyName,sShopPhone,sBusiness,sShopType,"
                  "iOffLineDisCount,sLicensePhoto,sIdPhoto,sShopFrontPhoto,sShopDetailPhoto,sAddress,"
                  "sCity,dGpsLat,dGpsLon,sHours,sPosNum,lPerAgentId,iAwardingPoints,lServiceUserId,"
                  "sSevvicePhone,iAvgConsume,iShowNearby,iVerify,iAuth,iProrate,sRemark,sOtherCerPhoto,sTradeMark from shopdal ";


    sSql += " order by  lAddTime asc";

    sSql+=" limit "+tConvert.IntToStr(piIndex)+","+tConvert.IntToStr(piCount);
    result =  multdal.QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){

        iRet = 0;

        while ( NULL != (row = mysql_fetch_row(result) )) {


            ShopInfoEntity entity;
            int iIndex = 0;

            entity.sPrimaryKey =  (row[iIndex++]);
            entity.lAddTime = atol(row[iIndex++]);
            entity.lUpTime = atol( row[iIndex++]);
            entity.iState = atoi( row[iIndex++]);
            entity.lUpdateId = atol( row[iIndex++] );
            entity.contentEntity.lUserId = atol(row[iIndex++]);
            entity.contentEntity.sAccount =  (row[iIndex++]);
            entity.contentEntity.sPassWord =   (row[iIndex++]);
            entity.contentEntity.sShopName =  (row[iIndex++]);
            entity.contentEntity.sCompanyName =  (row[iIndex++]);
            entity.contentEntity.sShopPhone =  (row[iIndex++]);
            entity.contentEntity.sBusiness =   (row[iIndex++]);
            entity.contentEntity.sShopType =   (row[iIndex++]);
            entity.contentEntity.iOffLineDisCount = atoi(row[iIndex++]);
            entity.contentEntity.sLicensePhoto =   (row[iIndex++]);
            entity.contentEntity.sIdPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopFrontPhoto =   (row[iIndex++]);
            entity.contentEntity.sShopDetailPhoto =   (row[iIndex++]);
            entity.contentEntity.sAddress =   (row[iIndex++]);
            entity.contentEntity.sCtiy =  (row[iIndex++]);
            entity.contentEntity.dGpsLat = atof(row[iIndex++]);
            entity.contentEntity.dGpsLon = atof(row[iIndex++]);
            entity.contentEntity.sHours =  (row[iIndex++]);
            entity.contentEntity.sPosNum =   (row[iIndex++]);
            entity.contentEntity.lPerAgentId =  tConvert.StrToLong( row[iIndex++] );
            entity.contentEntity.iAwardingPoints =  atoi(row[iIndex++]);
            entity.contentEntity.lServiceUserId = atol(row[iIndex++]);
            entity.contentEntity.sSevvicePhone =   (row[iIndex++]);
            entity.contentEntity.iAvgConsume =  atoi(row[iIndex++]);
            entity.contentEntity.iShowNearby =  atoi(row[iIndex++]);
            entity.contentEntity.iVerify =  atoi(row[iIndex++]);
            entity.contentEntity.iAuth =  atoi(row[iIndex++]);
            entity.contentEntity.iProrate =  atoi(row[iIndex++]);
            string sRemark = row[iIndex++];
            entity.contentEntity.sOtherCerPhoto = row[iIndex++];
            entity.contentEntity.sTradeMark = row[iIndex++];


            plstOut.push_back(entity);

        }

    }

    // 释放内存
    if( result != NULL){
       multdal.FreeResult( result );
    }
    return iRet;

}

