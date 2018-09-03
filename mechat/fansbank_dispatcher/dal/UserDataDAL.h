//用户数据访问层
//封装sql


#ifndef UserDataDAL_H
#define UserDataDAL_H

#include <string>
#include <vector>
#include "UserDataEntity.h"
#include "TypeDef.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/



class UserDataDAL
{
private:
    const string msTableName = "user_datadal";
public:

    //获取一批用户数据
    int GetUserInfoList( const LongLongMap &  Ids, UserDataList & pList  );
    int GetUserIdByPhone(const string & sPhone, long & lUserId,long & lCurrentPlatformId);
    //ret : -5 -数据不存在  0 -成功
    int GetEntityById(long lUserId,UserDataEntity& pEntity);

    int Get100FansPlatfrom(  longs & pList  );

    int Get100FansPush(  longs & pList  );

    int Get100User(  long lLastId, longs & pList  );

    int LetBindWeixinHot();

    int LetPerfectInfo2Hot();

    int LetConsume2Hot(string sWhere);

private:
    int ExcutEntity(const string  & sQql, UserDataEntity& pEntity);




};

#endif // UserDataDAL_H
