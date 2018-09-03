#ifndef COMMUNUITYBLL_H
#define COMMUNUITYBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/CommunityPublishDAL.h"
#include "dal/CommunityRemarkDAL.h"
#include "dal/UserDataDAL.h"
#include "dal/UserRelationDAL.h"
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:圈子 bll
 *Others:
*/
class CommunityBLL
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
    Description:分页获取首页圈子信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetShouYe( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:获取单条圈子信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetPublish( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:发布一条圈子信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SendPublish( const  Json::Value  & pJson ,Json::Value & pResponse);


    /*
    Description:圈子互动 （点赞，分享，收藏）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Hudong( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:发表评论
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SendComment( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:回复评论
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ReplyComment( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:分页获取圈子互动消息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetComment( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:获取给我的互动消息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ToMeHudong( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:清空给我的互动消息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ClearHudong( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:分页获取我发出的圈子信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMyPuslish( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:分页获取收藏的圈子信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetCollection( const  Json::Value  & pJson ,Json::Value & pResponse);


    /*
    Description:获取我的圈子个人详情
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyInfo( const  Json::Value  & pJson ,Json::Value & pResponse);


private:
    string GetFriends(long userId);

private:    

    CommunityPublishDAL mCommunityPublishDAL;
    CommunityRemarkDAL mCommunityRemarkDAL;
    UserDataDAL mUserDataDAL;
    UserRelationDAL mUserRelationDAL;

};

#endif // COMMUNUITYBLL_H
