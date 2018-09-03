#ifndef GROUPCHATENTITY_H
#define GROUPCHATENTITY_H

#include <string>
#include <vector>
using namespace std;

struct GroupChatEntity
{
    string sKey;
    long lSendId;
    long lGroupId;           //群Id
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iType;
    string sContent;
    int iLkstat;//红包状态 0 未拆  1 已拆 2 过期
    string sReaded;
};

typedef vector<GroupChatEntity> GroupChatList;

struct GroupChatQueryEntity
{
};

#endif // GROUPCHATENTITY_H
