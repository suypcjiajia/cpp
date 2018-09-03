#ifndef PUBLICFANSBLL_H
#define PUBLICFANSBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "dal/PublicFansDAL.h"
#include "dal/UserDataDAL.h"
#include "dal/PublicDataDAL.h"
#include "dal/PublicInfoDAL.h"
#include "PushBLL.h"
using namespace std;



/**
 *Copyright(C),TTD
 * 公众号粉丝接口
 * @author suyp
 * @modify suyp
 */


typedef std::tr1::unordered_map<string,PublicDataList> PingYingMap;

class PublicFansBLL
{
public:
    /**
     * 初始化
     * @param
     * @return 0 sucess
     */
    int Init();

    /**
     * 外部入口
     * @param psMethod
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 天天兑用户在附近商家消费(钱包调用)
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Tick( const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 获取我加入的公众号
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int MyPublic( const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 取消关注公众号
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int UnAttention( const Json::Value& pJson, Json::Value& pResponse );

    /**
     * 获取我的公众号上的消费等详细
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Detail( const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 获取公众号的消费粉丝（包括头像，昵称）（按分页批量）
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int ConsumerPage( const Json::Value& pJson, Json::Value& pResponse );

    /**
     * 在公众号发布一个红包
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Luky( const  Json::Value  & pJson, Json::Value & pResponse );

    /**
     * 在公众号抢一个红包
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int UnPackLk(const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 在公众号聊天
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Chat(const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 获取公众号信息（包括发布的，聊天的）
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int GetInfo(const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 公众号中发布一条信息（只能公众号所有者发布）
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int Release(const Json::Value& pJson, Json::Value& pResponse );


    /**
     * 获取公众号资料
     * @param pJson
     * @param pResponse
     * @return 0 sucess
     */
    int PublicData(const Json::Value& pJson, Json::Value& pResponse );


private:
    //推送一条聊天
    int PushNewChatCmd( long lUserId, long lSendId,const string sNickName, const string sHeadImage,long Id,const string & sKey, long lAddTime,
                                      const string & sContent, int iType,const string & sPublicName, long lMasterId);

    /*
    Description:推送成功回调
    Input:  @value 回调数据
    Output: null
    Return: null
    */
    void Sucess(const Json::Value & value);


    /*
    Description:推送失败回调
    Input:  @value 回调数据
    Output: null
    Return: null
    */
    void Fail(const Json::Value & value);

    //计算百份比
    int GetPersent(int iCount, int iTotal);




private:    

    PublicFansDAL mPublicFansDAL;
    UserDataDAL mUserDataDAL;
    PublicDataDAL mPublicDataDAL;
    PublicInfoDAL mPublicInfoDAL;

};

#endif // PUBLICFANSBLL_H
