#ifndef PrivateChatBLL_H
#define PrivateChatBLL_H

#include <string>
#include <vector>
#include"json/json.h"
#include "dal/PrivateChatDAL.h"
#include "dal/UserRelationDAL.h"
#include "dal/PublicInfoDAL.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装私聊业务接口
 *Others:
*/
class PrivateChatBLL
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
    Description:发送一条私聊
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Send( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:撤销私聊
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int CancelChat(const Json::Value& pJson, Json::Value& pResponse );

    /*
    Description:获取我的聊天
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyChat( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:拆红包
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


    /*
    Description:上传付款记录
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UpLoadPayRecord(const Json::Value& pJson, Json::Value& pResponse);


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
    PrivateChatDAL mFriendChatDAL;
    UserRelationDAL mUserRelationDal;
    PublicInfoDAL mPublicInfoDAL;

    //设置默认值
    void SetDefaultValue( PrivateChatEntity& pEntity );

    /*
    Description:
    Input: @lUserId @lToUserId
    Output: @userRelEntity
    Return: true -跳过好友关系的判断
    */
    bool PowerCheck(long lUserId,long lToUserId,const UserRelationEntity & userRelEntity);


};

#endif // PrivateChatBLL_H
