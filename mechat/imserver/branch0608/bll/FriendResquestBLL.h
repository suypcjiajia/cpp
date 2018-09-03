#ifndef FRIENDRESQUESTBLL_H
#define FRIENDRESQUESTBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/FriendResquestDAL.h"
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装好友请求业务
 *Others:
*/
class FriendResquestBLL
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
    Description:申请好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Resquest( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:同意申请好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Agree( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:不同意为好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int DisAgree( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取对方向我的申请
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ReqToMe( const Json::Value& pJson, Json::Value& pResponse );


private:    
    //数据库对像
    FriendResquestDAL mFriendResquestDAL;

    //设置默认值
    void SetDefaultValue( FriendResquestEntity& pEntity );
};

#endif // FRIENDRESQUESTBLL_H
