#ifndef FriendChatEntity_H
#define FriendChatEntity_H

#include <string>
#include <vector>
using namespace std;


//红包状态 0 未拆  1 已拆 2 过期
enum EnumLkMoneyStat
{
    enum_lkstat_unpack = 0,
    enum_lkstat_packed = 1,
    enum_lkstat_expire = 2
};

enum EnumChatType
{
    enum_chattype_char = 1,//文字
    enum_chattype_pic = 2,//图片
    enum_chattype_audio = 3,//语音
    enum_chattype_video = 4,//视频
    enum_chattype_lkmoney = 5, //红包
    enum_chattype_transfer = 6, //转帐
    enum_chattype_payrecord = 7, //付款记录
    enum_chattype_share = 8,//分享
    enum_chattype_public = 9 //商家管理后台的发布信息
};


struct PrivateChatEntity
{
    string sKey;
    long lUserId;           //用户ID
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iState;             //状态  0未读 1已读 2红包已拆 3红包已过期 333删除
    long lToUserId;         //对方用户ID
    int iType;              //1为文字 2为图片 3为语音 4为视频5为发红包
    string sContent;        //内容
    EnumLkMoneyStat iLkstat;            //红包状态
};
typedef vector<PrivateChatEntity> PrivateChatList;


struct PrivateChatQueryEntity
{

    long lUserId;
    long lToUserId;
    int iType;
    int iState;
};





#endif // FriendChatEntity_H
