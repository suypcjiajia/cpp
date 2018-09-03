#ifndef GROUPCHATDAL_H
#define GROUPCHATDAL_H


/*
 *群聊mysql访问层
 * */

#include <string>
#include <vector>
#include "GroupMemberEntity.h"
#include "GroupChatEntity.h"
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class GroupChatDAL
{

public:

    int CreateTable();

    int AddData( const GroupChatEntity & entity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const string & sKey,GroupChatEntity & entity);

    //获取未读群聊
    int GetMyChat( const long & lUserId, const vector<long> & GroupIdLst,const long & lAddTime, const int & iCount,GroupChatList & ChatLst);

    //获取未读群聊(去掉加入群之前的聊天)
    int GetMyChat(const long & lUserId,const GroupMemberList & memberTimeLst,const long & lAddTime, const int & iCount,GroupChatList & ChatLst);
    int UpdateReaded(long lUserId,const GroupChatList & lst);
private:

    int Execute(const string & sSql,GroupChatList & chatLst);
    const string msTableName = "group_chat";

};

#endif // GROUPCHATDAL_H
