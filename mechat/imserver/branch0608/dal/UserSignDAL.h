#ifndef USERSIGNDAL_H
#define USERSIGNDAL_H

#include <string>
#include <vector>
#include "UserStateEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:每日签到表
 *Others:
*/
class UserSignDAL
{
private:
    const string msTableName = "user_sign";
public:
    //创建每日签到表
    int CreateTable();

    int Insert( long lUserId, long lAddTime );

    int IsExist(long  lUserId, const string & sDay);


private:

};

#endif // USERSIGNDAL_H
