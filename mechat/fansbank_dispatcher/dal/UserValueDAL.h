#ifndef USERSIGNDAL_H
#define USERSIGNDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:用户价值表
 *Others:
*/
class UserValueDAL:public BaseDal
{
private:
    const string msTableName = "fansbank_u_value";
public:

    int Add(long lUserId,double dValue  ,int iType );

    int IsExist(long  lUserId, int iType);

    //获取用户价值
    int GetUserValue(long lUserId,int iType, double & dValue);

    //增加用户价值
    int IncreemValue(long lUserId, double dValue,int iType);

    //对于昨天没签到并且没有消费的用户,每用户贬值0.5元
    int DeIncreemVeryUser();

    //针对价值小于5.0的用户，加0.5元价值
    int IncreemVeryUser();

    //小于保低值的用户，设置成保低值
    int SetLowest();

    //获取平台粉丝的用户均价
    int GetPlatformAvg(string sShopPhone,long Id,int iType,double &  dAvg);

    //获取推荐人粉丝均价
    int GetPushAvg(long Id,int iType,double &  dAvg);


private:

};

#endif // USERSIGNDAL_H
