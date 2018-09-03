#ifndef NOTICEBLL_H
#define NOTICEBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/NoticeDAL.h"
#include "dal/RedPoinDAL.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装调系统通知业务接口
 *Others:
*/
class NoticeBLL
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
    Description:发送一条通知
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Send( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取未读通知
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMyNotice( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:记录已读的时间点
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Touch( const Json::Value& pJson, Json::Value& pResponse );


    /*
    Description:获取红点情况
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int RedPoin( const Json::Value& pJson, Json::Value& pResponse );


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

public:

    /*
    Description:推送系统通知
    Input:  @jRevIds 推送的目标用户，多人
    Output: @nEntity
    Return: 0 -success
    */
    int PushNotice( const Json::Value & jRevIds ,const NoticeEntity&  nEntity , string sColor = "");

private:    

    NoticeDAL mNoticeDAL;
    RedPointDAL mRedPointDAL;

    string mMsgColor[20] = {"#1C1C1C","#4EA4F7","#F025AA","#F85DA8","#FF4848","#7D64B3","#FD7373","#49A95D","#E51644",
                           "#FF9329","#5063F3","#1EA19A"};

};

#endif // NOTICEBLL_H
