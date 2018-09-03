#ifndef GROUPMEMBERENTITY_H
#define GROUPMEMBERENTITY_H

#include <string>
#include <vector>
using namespace std;

struct GroupMemberEntity
{
    long lGroupId;           //群Id
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    string sNickName;//成员在群里的昵称
    long lMemberId;//群成员
    int iJoin; //加入方式 0未知 1二维码 2好友
    int iType;

};

typedef vector<GroupMemberEntity> GroupMemberList;

struct GroupMemberQueryEntity
{
};

//和群的关系
enum EnumMemberType
{
    enum_membertype_none = 0,//没关系
    enum_membertype_member = 1 //群成员
};

//加入方式
enum EnumMemberJoin
{
    enum_memberjoin_unknow = 0,//未知
    enum_memberjoin_qrcode = 1,//二维码
    enum_memberjoin_friend  =2 //好友
};

struct GroupUpTime{
    long lGroupId;
    long lUpTime;
};
typedef vector<GroupUpTime> GroupUpTimeList;
#endif // GROUPMEMBERENTITY_H
