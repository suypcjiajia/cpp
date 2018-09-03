#ifndef KVALUEDAL_H
#define KVALUEDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "KVlaueEntity.h"
#include "TypeDef.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/FansBankOrderDAL.h"
#include "dal/FansBankUserValueDAL.h"
#include "dal/UserDataDAL.h"
#include "param/Param.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:日K线表
 *Others:
*/
class FansBankKValueDAL
{

public:

    int Add(const KValueEntity & entity   );

    int IsExist(long Id,int iTpye, const string & sDay);

    //获取某天的一条K柱
    int GetEntity(long Id,int iTpye, const string & sDay, KValueEntity & entity);

    //获取最新的一条K柱
    int GetLast(long Id,int iTpye, KValueEntity & entity);

    //获取前20条K线图
    int GetKs(long Id,int iTpye, KValueLst & Ks);

    //获取行情（按分页）
    int GetPage(int iTpye, int iPage, int iCount, string sCondition ,KValueLst & Ks );



    //更新日K线当前价格
    int UpdatePrice(long Id,int iTpye, const string & sDay, double dPrice);
    //更新日K线最低价
    int UpdateLow(long Id,int iTpye, const string & sDay, double dPrice);
    //更新日K线最高价
    int UpdateHigh(long Id,int iTpye, const string & sDay, double dPrice);
    //更新日K线收盘价
    int UpdateClose(long Id,int iTpye, const string & sDay, double dPrice);
    //更新日K线开盘价
    int UpdateOpen(long Id,int iTpye, const string & sDay, double dPrice);

    //获取移动平均值（ma值）
    int GetMaAvg(long Id,int iType, string sDay , int iMa, double &  dAvg);

    //Increase均价
    int IncreaseValue(double dValue, long id,int iType);

private:

    const string msAllColumn = "select price,open,close,hight,low,turnOver,turnRate,name,yclose from ";
    void TransToStruct(KValueEntity & entity,MYSQL_ROW row);



private:


    const string msTableName = "fansbank_k_value";

    TConvert tConvert;
    TMultiMysqlDAL multi;


};

#endif // USERSIGNDAL_H
