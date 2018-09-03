#ifndef FANSBANKFANSMOVEDAL_H
#define FANSBANKFANSMOVEDAL_H

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
 *Description:粉丝转移表
 *Others:
*/
class FansBankFansmoveDAL:public BaseDal
{    
public:
    FansBankFansmoveDAL();

    int Add(string sOrder, string sFans );
    string GetSqlAdd( string sOrder, string sFans  );

};

#endif // FANSBANKFANSMOVEDAL_H
