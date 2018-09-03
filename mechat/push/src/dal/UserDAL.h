#ifndef USERDAL_H
#define USERDAL_H

#include <string>
#include <vector>
#include "entity/PushUserEntity.h"

using namespace std;


/*
Description:推送机的用户连接用户类型等信息
*/
class UserDAL
{

public:
    //创建表
    int CreateTable();
    //更新连接用户信息
    int Replace( PushUserEntity& pEntity );

    //查询连接用户信息
    int GetEntity(const long plUserId, PushUserEntity& pEntity );
    int GetPushIp(long lUserId,string & sPushIp);
    int GetToken(long lUserId,string & sToken);
    int GetType(long lUserId,int & iPhoneType);

private:
    const string msTableName = "push_ipdal";

public:
    const static  string sPushIp ;
    const static  string sPhoneType ;
    const static  string sToken ;
    const static  string  sTime;
};

#endif // USERDAL_H
