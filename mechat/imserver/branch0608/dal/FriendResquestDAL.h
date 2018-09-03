#ifndef FriendResquestDAL_H
#define FriendResquestDAL_H

#include <string>
#include <vector>
#include "FriendResquestEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class FriendResquestDAL
{
private:
    const string msTableName = "friend_requestdal";
public:
    //创建表
    int CreateTable();

    int IsExist(const long & plUserId, const long & plToUserId);

    int AddData( const FriendResquestEntity & entity);

    int UpdateAction(const long & plUserId, const long & plToUserId , int piAction);


    //查询数据
    int GetData( const FriendResquestQueryEntity & pQueryEntity , int piIndex, int piCount, FriendResquestList & plstOut, int & piTotalCount);


private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);

};

#endif // FriendResquestDAL_H
