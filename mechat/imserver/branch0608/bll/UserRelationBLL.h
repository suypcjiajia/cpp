#ifndef FriendRelationBLL_H
#define FriendRelationBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/UserRelationDAL.h"
#include "UserDataEntity.h"
using namespace std;


/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户关系业务接口
 *Others:
*/
class UserRelationBLL
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
    Description:拉黑好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Black( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取我的好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyFriends( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:删除好友
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int DelFri( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:设置好友备注
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetMark( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:通信录用户状态
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Contacts( const Json::Value& pJson, Json::Value& pResponse );

private:    

    UserRelationDAL mUserRelationDAL;

    //设置默认值
    void SetDefaultValue( UserRelationEntity& pEntity );



};

#endif // UserDataBLL_H
