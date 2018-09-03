#ifndef USERGRADEDAL_H
#define USERGRADEDAL_H

#include <string>
#include <vector>
#include "UserDataEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class UserGradeDAL
{

public:
    //创建表
    int CreateTable();

    int Add(const long & lUserId, int & iLevel);

    int IsExist(const long & lUserId);

    int GetLevel(const long & lUserId, int & iLevel);

    int UpdateLevel(const long & lUserId, int & iLevel);

    int GetVip(int iPage, int iCount,UserGradeList & lst);

private:
    const string msTableName = "user_grade";

};

#endif // USERGRADEDAL_H
