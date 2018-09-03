#ifndef USERSIGNENTITY_H
#define USERSIGNENTITY_H

#include <string>
#include <vector>
using namespace std;

struct UserSignEntity
{
    long lUserId;           //用户ID
    long lAddTime;          //添加时间
    string sDay;

};

typedef vector<UserSignEntity> UserSignLst;

#endif // USERSIGNENTITY_H
