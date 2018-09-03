#ifndef UserStateDAL_H
#define UserStateDAL_H

#include <string>
#include <vector>
#include "UserStateEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class UserStateDAL
{
private:
    const string msTableName = "user_statedal";
public:
    //创建表
    int CreateTable();

    int Insert( UserStateEntity& pEntity );
    int SetLoginTime(const long & plUserId, const long & plLoginTime);
     //状态-更新
    int Update(UserStateEntity& pEntity);

    /*功能：附近的人
     * 参数：　dDistance --距离之内,公里　　iCount --距离之内最近的最多人数
     * pdGpsLat 纬度,pdGpsLon 纬度
    */
    int GetNeerBy(const long plUserId,double pdGpsLat, double pdGpsLon,double pdDistance,int piCount,int piSexType,vector<UserStateEntity>& pUserStateVector);
    //获取最近的登录时间
    int GetLoginTime(const long plUserId, long & plLoginTime);
    int IsExistByUserId( const string& psUserId, bool& pbIsBool );

private:

};

#endif // UserStateDAL_H
