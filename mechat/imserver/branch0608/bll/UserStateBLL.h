#ifndef UserStateBLL_H
#define UserStateBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/UserStateDAL.h"
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户状态业务接口
 *Others:
*/
class UserStateBLL
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
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse);

    /*
    Description:更新状态
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Updata( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:附近的人
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetNeerBy( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:获取最近的登录时间
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetLoginTime( const  Json::Value  & pJson  ,Json::Value & pResponse);
private:    

    UserStateDAL mUserStateDAL;
};

#endif // UserStateBLL_H
