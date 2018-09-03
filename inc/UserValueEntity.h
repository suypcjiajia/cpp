#ifndef USERVALUEENTITY_H
#define USERVALUEENTITY_H

#include <string>
#include <vector>
using namespace std;

struct UserValueEntity
{
    long lUserId;           //用户ID
    double dValue;          //添加时间

};

typedef vector<UserValueEntity> UserValueLst;

#endif // USERVALUEENTITY_H
