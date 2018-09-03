#ifndef PushDataBLL_H
#define PushDataBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "json/json.h"
#include "dal/RedisConnect.h"
#include "dal/ImDAL.h"
#include "dal/UserDAL.h"
#include "com/WsSendCOM.h"
#include <tr1/unordered_map>
#include "TypeDef.h"
//按IP地址存放多个用户<IP地址,多个用户>
typedef std::tr1::unordered_map<string,vector<PushUserEntity> > IpManyUserMap;
typedef IpManyUserMap::iterator IpManyUserMapIter;


using namespace std;

class PushDataBLL
{
public:
    //初始化
    int Init();
    //http接口调用入口
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse);
    //推送数据(http对外接口)
    int PushData( const Json::Value& pJson ,Json::Value & pResponse);
    //推送数据(http推送机与推送机通信)
    int Inside( Json::Value  pJson ,Json::Value & pResponse);
    //推送数据(http推送机与推送机通信)目标是多用户
    int PushInsides(  Json::Value   pJson, Json::Value & pResponse );

    int LogOut(Json::Value  pJson ,Json::Value & pResponse);


    /////////////////////////////////////
    int PushOffline(long plUserId);

    //向推送机注册
    int Register(server* pServer, connection_hdl pHdl,const long lUserId,const int iPhoneType,const string& sToken,const string & sVer);
    //下线
    int OffLine(server* pServer, connection_hdl pHdl,const long lUserId);

    //处理客户端还回
    int OnRetForChat(const int iRet ,const string & sKey);
    //处理客户端还回
    int OnRetForGroupChat(const int iRet ,const string & sKey,long lUserId);
    //处理客户端还回
    int OnRetForPublicChat(const int iRet ,const string & sKey,long lUserId);

    ////////////////////////////////////
    //发送数据
    int ToSend(const long plUserId,const int piPhoneType,const string& psToken,const string& psMsg);
    //发送数据by websocket
    int SendByWs( const long plUserId,server* pServer, connection_hdl pHdl, const string& psStrData );
    //向苹果的APNS发送消息
    int ApplePushNetServer( long lUserId, const string& psToken, const string& psMsg );
private:
    bool FiltCmd(long lUserId,int iCmd);
    //消息是否打扰
    //jMsg : 推送的数据
    //return : true -打扰 -false -消息免打扰
    bool IsTrouble(long lUserId,const Json::Value & jMsg);

    //分批。每一批用户所在的推送机要相同
    void PutUserToBucket( IpManyUserMap & ipManyUserMap,const PushUserEntity & con );

    void GetAllPushIPs(StringLst & all) ;

    UserDAL mUserDAL;

    ImDAL mImDAL;

};

#endif // PushIpBLL_H
