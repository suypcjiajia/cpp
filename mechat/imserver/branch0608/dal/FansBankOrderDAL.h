#ifndef FANSBANKORDERDAL_H
#define FANSBANKORDERDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "FansBankOrderEntity.h"
#include "TypeDef.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:订单表
 *Others:
*/
class FansBankOrderDAL:public BaseDal
{

public:

    FansBankOrderDAL(){
        msAllColumn = "select orderNo,addTime,endTime,state,type,idBuy,nameBuy,idSell,nameSell,price,lots,turnOver,lotsOver from ";
        msTableName = "fansbank_order";

    }

    int GetEntity(const string & orderNo, FansBankOrderEntity & entity);

    int Add(const FansBankOrderEntity & entity   );
    string GetSqlAdd( const FansBankOrderEntity & entity   );

    //是否有正在卖出
    int IsExistSelling(long Id,int iTpye);

    //得到正在卖出的手数
    int GetLotsSell(long Id,int iTpye , int & iLots);

    //得到正在卖出的添加时间
    int GetLotsSellAddTime(long Id,int iTpye , long & lAddTime);

    //增加正在卖出的手数
    int InreemLots(long Id,int iTpye, int iLots);

    //增加订单中的已经卖出手数
    int InreemLotsOver(long Id,int iTpye, int iLots, double dTurnOver);
    //增加订单中的已经卖出手数 sql
    string GetSqlInreemLotsOver(long Id,int iTpye, int iLots, double dTurnOver);

    //减少正在卖出的手数
    int DeInreemLots(long Id,int iTpye, int iLots);
    //减少正在卖出的手数
    string GetSqlDeInreemLots(long Id,int iTpye, int iLots);

    //获取交易中的可买入列表
    int GetSelling( int iTpye ,int iPage, string sCondition ,int iCount,FansBankOrderLst & lst);

    //获取根据报单数据返回挂卖列表
    int GetSellingByBaodanState( int iTpye ,int iPage, string sCondition ,int isBaodan,string baodanMemeber,int iCount,FansBankOrderLst & lst);


    //得到卖出总额
    int GetTotalSell(long Id,int iTpye , double & dTotal);

    //得到买入总额
    int GetTotalBuy(long Id,int iTpye , double & dTotal);


    //获取Id对像的成交记录
    int GetPageById(int iType,long Id, int iPage,int iCount , FansBankOrderLst & lst);

    //获取订单(后台管理)
    int GetOrderByManage(int iType,int iState, long idSell, long idBuy,
                         long startTime,long endTime,int iPage,int iCount , FansBankOrderLst & lst,int & iTotal);

    //获取所有卖出记录
    int GetAllTrade(int iType, int iPage,int iCount , FansBankOrderLst & lst);


    //获取我的正在卖出
    int GetPageMySalling(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst);

    //获取我的取消
    int GetPageMyCancel(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst);

    //获取我的已卖出
    int GetPageMyOk(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst);

    //获取DasaiBaodan已卖出
    int GetPageDasaiBaodanOk( int iPage,int iCount ,string sOrderNo, FansBankOrderLst & lst,int & iTotal);

    //获取today id已卖total lots
    int GeTodaySelledLots(int iType,string sIds,int  & iCount);

    //获取我买入的
    int GetPageMyBuy(int iType,string sIds, int iPage,int iCount , FansBankOrderLst & lst);

    //取消订单
    int CancelOrder(long Id, int iType);

    //删除正在卖出但手数为0的记录
    int Del(long Id, int iType);
    //删除正在卖出但手数为0的记录 sql
    string GetSqlDel(long Id, int iType);

    //获取某天交易额
    int GetOneDayTrade(long Id,int iType, string sDay , double &  dOut);


    //获取某个时间点之后的挂卖的条数
    int GetNewCount(long lUserId,long lLastTime,int iType, int &  iCount);




private:

    int Fetch(string sSql,FansBankOrderLst & lst);



private:


    TConvert tConvert;
    TMultiMysqlDAL multi;

};

#endif // FANSBANKORDERDAL_H
