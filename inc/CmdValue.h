#ifndef CMDVALUE_H
#define CMDVALUE_H

//推送消息的类型
enum EnumCmdValue
{
    enum_cmd_get_registerinfo = 1,
    enum_cmd_register = 100, //注册
    enum_cmd_offline = 101, //下线
    enum_cmd_clearmsgcount = 102,//IOS推送角标清0
    enum_cmd_resqfri =110 ,//申请好友
    enum_cmd_agreefri = 111, //同意成为好友
    enum_cmd_disagreefri = 112 ,//不同意成为好友
    enum_cmd_blackfri = 120, //拉黑好友
    enum_cmd_delfri = 121 , //删除好友
    enum_cmd_chat = 130, //私聊
    enum_cmd_cancelchat = 131, //撤销私聊
    enum_cmd_unpacklkmoney = 132, //拆红包
    enum_cmd_outtimelkmoney = 133, //红包过期
    enum_cmd_payrecord = 134, //付款记录
    enum_cmd_group_join = 140 ,  //入群
    enum_cmd_group_out = 141, //退群
    enum_cmd_group_kickout = 142,  //群主踢人
    enum_cmd_group_chat = 143, //群聊
    enum_cmd_group_set = 144,  //变更群信息
    enum_cmd_group_membername = 145, //群成员变更昵称

    enum_cmd_notice  = 160, //系统通知


    enum_cmd_public_chat  = 170, //公众号一对一聊天

    enum_cmd_get_money = 211//有钱到帐

};

#endif
