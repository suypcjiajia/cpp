#ifndef PRIVATECHATDAL_H
#define PRIVATECHATDAL_H

#include <string>
#include <vector>
#include "PrivateChatEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class PrivateChatDAL
{

public:
    //创建表
    int CreateTable();
    //更新状态
    int UpdateState(const string &  psKey,  int piState);
    int UpdateState(const PrivateChatList & lst, int iState);

    //更新红包状态
    int UpdateLkStat(const string & psKey, EnumLkMoneyStat iState);
    //添加一条私聊
    int AddData( const PrivateChatEntity & entity);
    //获取我的聊天
    int GetMyChat( const long & lUserId, const long & lToUserId,const long & lAddTime, const int & iCount,PrivateChatList & plstOut);
    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const string & psKey,PrivateChatEntity & entity);


    //获取未过期最旧的100条红包
    int GetLockyMoney(PrivateChatList & lst);

private:
    int GetQueryTotalCount(const string & psQuery, int & piCount);
private:
    const string msTableName = "private_chatdal";
};

#endif // PRIVATECHATDAL_H
