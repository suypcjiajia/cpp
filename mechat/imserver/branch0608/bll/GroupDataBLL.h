#ifndef GROUPDATABLL_H
#define GROUPDATABLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/GroupDataDAL.h"

using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装群资料业务
 *Others:
*/
class GroupDataBLL
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
    Description:创建一个群
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Creat( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:查询群资料
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetInfoById( const Json::Value& pJson, Json::Value& pResponse );

    /*
    Description:遍历群（后台维护使用）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetGroupIds( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:更改群信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Set( const Json::Value& pJson, Json::Value& pResponse );

    /*
    Description:群头像（合成头像）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetHead( const Json::Value& pJson, Json::Value& pResponse );



private:    

    GroupDataDAL mGroupDataDAL;

};

#endif // GROUPDATABLL_H
