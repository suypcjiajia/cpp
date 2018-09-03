#ifndef USERSETDAL_H
#define USERSETDAL_H

#include <string>
#include <vector>
#include "UserSetEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class UserSetDAL
{

public:
    //创建表
    int CreateTable();

    int IsExist(const long & plUserId);

    int AddData( const UserSetEntity & entity);

    int UpdateTopChatFriends(const long & plUserId , const string & psTopChatFriends);

    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const long & plUserId,UserSetEntity & entity);
private:
    const string msTableName = "user_set";

};

#endif // USERSETDAL_H
