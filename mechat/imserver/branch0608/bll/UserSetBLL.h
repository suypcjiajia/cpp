#ifndef USERSETBLL_H
#define USERSETBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/UserSetDAL.h"
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户配置业务接口
 *Others:
*/
class UserSetBLL
{
public:
    /*
    Description:初始化
    Input: null
    Output: null
    Return: 0 -success
    */
    int Init();
    /*
    Description:分配业务接口
    Input: @psMethod 方法名 @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:设置好友置顶
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetTopChatFriend( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:取消好友置顶
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int DelTopChatFriend( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取我的置顶的好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyTopChatFriends( const Json::Value& pJson, Json::Value& pResponse );



    int DelTopChatFriend(const long & lUserId, const long & lToUserId);


private:    

    UserSetDAL mUserSetDAL;

};

#endif // USERSETBLL_H
