#ifndef FansBankUserValueDAL_H
#define FansBankUserValueDAL_H

#include <string>
#include <vector>
#include "UserSignEntity.h"
#include "UserValueEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:用户价值表
 *Others:
*/
class FansBankUserValueDAL : public BaseDal
{

public:

    FansBankUserValueDAL();

    int IsExist(long  lUserId, int iType);

    int Add(long lUserId,double dValue  ,int iType );

    //update用户价值
    int UpValue(long lUserId, double dValue,int iType);

    //Increase对像下面粉丝的用户价值
    int IncreaseValue(double dValue, long id,int iType);

    //update用户价值
    //sUserids a,b,c format
    int UpValuelots(string sUserids, double dValue,int iType);

    //获取平台粉丝的用户均价
    int GetPlatformAvg(string sShopPhone,long Id,int iType,double &  dAvg);

    //获取推荐人粉丝均价
    int GetPushAvg(long Id,int iType,double &  dAvg);

    int GetCount(long Id,int iType, int & iCount);


    //获取一批价值的用户
    int GetValueUser(string sSellPhone,long Id, int iType,bool bLow,int iIndex,int iNum,UserValueLst & lst);

    int DeIncreem(long lUserId,double dDeValue);



private:

};

#endif // FansBankUserValueDAL_H
