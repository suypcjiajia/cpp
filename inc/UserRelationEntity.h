#ifndef USERRELATIONENTITY_H
#define USERRELATIONENTITY_H
#include <vector>
#include <string>
using namespace std;
//好友关系实体
struct UserRelationEntity
{
    long lUserId;             //用户ID
    long lAddTime;            //添加时间
    long lUpTime;             //更新时间
    int iState;               //状态
    long lToUserId;           //对方用户ID
    int iType;                //类型
    string sUserMark;
    string sToUserMark;
};
//好友关系列表
typedef vector<UserRelationEntity> UserRelationEntityList;
//查询条件
struct UserRelationQueryEntity
{
    long lUserId;
    long lToUserId;
    int iType;
    long lUpTime;
};
//枚举，好友关系
enum EnumRelationType
{
    enum_rel_no = 0,//0 为没任何关系
    enum_rel_friend = 1,//1 好友
    enum_rel_stranger = 2,//2 陌生人
    enum_rel_ablackb = 3, //3 用户A拉黑B
    enum_rel_bbcacka = 4, //4 用户B拉黑A
    enum_rel_adelb = 5,//5 用户A删除B
    enum_rel_bdela = 6, //6 用户B删除A
    enum_rel_blackab = 7, //7 双向拉黑
    enum_rel_delab = 8,//8 双向删除
    enum_rel_ablackb_bdela = 9, //9用户A拉黑B，用户B删除A
    enum_rel_adelb_bblacka = 10,//10 用户B拉黑用户A删除B

};

#endif // USERRELATIONENTITY_H
