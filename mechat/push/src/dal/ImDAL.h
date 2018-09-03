#ifndef IMDAL_H
#define IMDAL_H

#include <string>
#include <vector>
#include "UserDataEntity.h"
using namespace std;


/*
Description:推送机访问IM的mysql
*/
class ImDAL
{
public:

    //更新chat状态(已读)
    int UpdateChatState(const string &  psKey, int piState);
    //更新groupchat已读用户
    int UpdateGroupChatReaded(const string &  psKey, long  lUserId);
    //更新publicchat已读用户
    int UpdatePublicChatReaded(const string &  psKey, long  lUserId);
    int GetUserInfo(const long & lUserId, UserDataEntity& pEntity);
    int GetMsgMode(long lUserId, long lGroupId, int & iMsgMode);

private:

};

#endif // IMDAL_H
