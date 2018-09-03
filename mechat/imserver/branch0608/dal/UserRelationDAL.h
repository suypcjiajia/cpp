#ifndef USERRELATIONDAL_H
#define USERRELATIONDAL_H

#include <string>
#include <vector>
#include "UserRelationEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class UserRelationDAL
{

public:
    //创建表
    int CreateTable();
    int UpdateType(const long & plUserId, const long & plToUserId , int piType);
    int UpdateMark(const long & plUserId, const long & plToUserId , int iWho, const string & sMark);
    int AddData( const UserRelationEntity & entity);
    int IsExist(const long & plUserId, const long & plToUserId);
    //查询数据
    int GetData( const UserRelationQueryEntity & pQueryEntity , UserRelationEntityList & plstOut, int & piTotalCount);
    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const long & plUserId, const long & plToUserId,UserRelationEntity & entity);

private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);

private:
    const string msTableName = "user_relationdal";

};

#endif // USERRELATIONDAL_H
