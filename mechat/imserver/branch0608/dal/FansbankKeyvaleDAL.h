#ifndef FANSBANKKEYVALUEDAL_H
#define FANSBANKKEYVALUEDAL_H

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
class FansBankKeyvalueDAL:public BaseDal
{
private:
    const string msTableName = "fansbank_keyvalue";
public:

    int GetValue(string key, string & value);

    int SetValue( string key, string value);

    int GetAll( CommonStructVector & lst);


private:

};

#endif // FANSBANKKEYVALUEDAL_H
