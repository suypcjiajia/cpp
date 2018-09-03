#ifndef FANSBANKSALLINGDAL_H
#define FANSBANKSALLINGDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "KVlaueEntity.h"
#include "TypeDef.h"
#include "SallingEntity.h"
#include "FansBankOrderEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:日K线表
 *Others:
*/
class FansBankSallingDAL
{
private:
    const string msTableName = "fansbank_salling";
public:





    //获取交易中的可买入列表
    int GetSelling(int iTpye ,int iPage, string sCondition ,int iCount,SallingLst & lst);




private:

};

#endif // FANSBANKSALLINGDAL_H
