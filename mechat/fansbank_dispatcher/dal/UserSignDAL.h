#ifndef USERVALUEDAL_H
#define USERVALUEDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "TypeDef.h"
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


    int IsExist(long  lUserId, const string & sDay);

    //获取未计算的签到
    int GetSigns(UserSignLst & userSignLst);

    //设置签到为已计算状态
    int SetState( const longs & lUserIds);


private:

};

#endif // USERVALUEDAL_H
