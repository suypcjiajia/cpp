#ifndef FANSBANCOUNTDAL_H
#define FANSBANCOUNTDAL_H

#include <string>
#include <vector>
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:配置信息表
 *Others:
*/
class FansBankCountDAL:public BaseDal
{
private:
    const string msTableName = "fansbank_count";
public:

    int GetValueForLock(string id,string type, string & value);

    int SetValue(string id,string type, string value);


private:

};

#endif // FANSBANCOUNTDAL_H
