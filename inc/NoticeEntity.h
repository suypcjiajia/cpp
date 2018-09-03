#ifndef NOTICEENTITY_H
#define NOTICEENTITY_H

#include <string>
#include <vector>
using namespace std;

enum EnumTarget
{
    enum_broadcast = 1,//广播
    enum_multicast = 2  //组播
};

enum EnumNoticeType
{
    enum_notice_gonggao = 1,//1 公告
    enum_notice_xiaoxi = 2  //2 消息
};


//红点类型
enum EnumRedPointType
{
    enum_red_sysxiaoxi = 1,//系统公告 and 消息
};

//数据实体(系统通知)
struct NoticeEntity
{
    string sKey;//标记一条记录
    long lSendId;//通知发送者
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    string sContent;//通知的内容,格式见下面
    EnumTarget iTarget;//目的地
    string sRevIds;//接收者Id（数组；当是组播时才用）
    string sReaded;//已读过的用户
    int iType;//消息类型, 1 公告  2 消息

};
/*
//sContent格式,必须有sTitle,sText .  sPic,sUrl可有可无
{
   sTitle : ""  //标题
   sText  : ""  //正文
   sPic : ""    //图片oss地址
   sUrl : ""    //跳转到Url
}
*/
typedef vector<NoticeEntity> NoticeList;

struct NoticeQueryEntity
{
};

#endif // NOTICEENTITY_H
