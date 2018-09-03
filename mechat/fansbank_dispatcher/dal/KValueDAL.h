#ifndef KVALUEDAL_H
#define KVALUEDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "KVlaueEntity.h"
#include "TypeDef.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:日K线表
 *Others:
*/
class KValueDAL
{
private:
    const string msTableName = "fansbank_k_value";
public:

    int Add(const KValueEntity & entity   );

    int IsExist(long Id,int iTpye, const string & sDay);


    int GetEntity(long Id,int iTpye, const string & sDay, KValueEntity & entity);

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




private:

};

#endif // USERSIGNDAL_H
