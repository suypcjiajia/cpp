#ifndef GROUPCHATBLL_H
#define GROUPCHATBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/GroupChatDAL.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群聊业务
 *Others:
*/
class GroupChatBLL
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
    Description:发送一条群聊
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Send( const Json::Value& pJson, Json::Value& pResponse );

    /*
    Description:获取未读群聊
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyChat( const Json::Value& pJson, Json::Value& pResponse );

    /*
    Description:抢红包
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UnPackLkMoney(const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:红包信息查询 by订单号
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int QueryLkByOrder(const Json::Value& pJson, Json::Value& pResponse);


    //////////////////////////


public:

    /*
    Description:推送一条群聊天
    Input:
    Output:
    Return: 0 -success
    */
    int PushNewChatCmd( long lUserId, long lSendId, const string sNickName, const string sHeadImage,long lGroupId,const string & sKey, long lAddTime,
                                      const string & sContent, int iType);


    /*
    Description:推送成功回调
    Input:  @value 回调数据
    Output: null
    Return: null
    */
    static void Sucess(const Json::Value & value);

    /*
    Description:推送失败回调
    Input:  @value 回调数据
    Output: null
    Return: null
    */
    static void Fail(const Json::Value & value);


private:    

    GroupChatDAL mGroupDataDAL;

};

#endif // GROUPCHATBLL_H
