#ifndef USERSETENTITY_H
#define USERSETENTITY_H

#include <string>
#include <vector>
using namespace std;

struct UserSetEntity
{
    long lUserId;           //用户ID
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    string sTopChatFriends;//聊天置顶的好友,多个用,号分隔
};

typedef vector<UserSetEntity> UserSetList;

struct UserSetQueryEntity
{
};

#endif // USERSETENTITY_H
