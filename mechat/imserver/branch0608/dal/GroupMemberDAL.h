#ifndef GROUPMEMBERDAL_H
#define GROUPMEMBERDAL_H


/*
 *群成员mysql访问层
 * */

#include <string>
#include <vector>
#include "GroupMemberEntity.h"
#include "GroupDataDAL.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class GroupMemberDAL
{

public:

    int CreateTable();

    int IsExist(const long & plGroupId,const long & plMemberId);

    int AddData( const GroupMemberEntity & entity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const long & plGroupId,const long & plMemberId,GroupMemberEntity & entity);

    //我加入的群(包括没关系和群成员)
    int GetMyGroup( const long & lUserId, const long & lUpTime,GroupMemberList & plstOut);
    //我加入的群
    int GetMyGroup( const long & lUserId,GroupMemberList & plstOut);


    //一个群的所有成员(包括没关系和群成员)
    int GetMemberById(const long & lGroupId,GroupMemberList & plstOut);

    //一个群的所有成员(只包括群成员)
    int GetRealMemberById(const long & lGroupId,GroupMemberList & plstOut);

    //一批群的成员(按时间戳)
    int GetMemberBylotsId(GroupUpTimeList gruopUpLst,int iType,GroupMemberList & plstOut);

    //更新和群的关系
    int UpDataType( long  lGroupId, long  lMemberId, int iType );


    int SetNickName(long lMemberId,long lGroupId, string sNickName);
    int SetMsgMode(long lMemberId,long lGroupId, int iMsgMode);

private:
    int Execute(const string & sSql,GroupMemberList & plstOut);
private:
    const string msTableName = "group_member";

};

#endif // GROUPMEMBERDAL_H
