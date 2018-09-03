#ifndef TBLL_H
#define TBLL_H

#include <string>
#include "TMutex.hpp"
#include <json/json.h>
using namespace std;
class  TBLL
{
public:  static TBLL* GetInstance();
private: static TBLL* mInstance;
private: TMutex mMutex;                  //线程锁
public:
    //获取调试日志
    const string GetDebugLog(){return this->msDebugLog;}
    //是否使用https,true or false
    const string IsUseSSL(){return this->msOpenSSL;}
    //获取启动的线程数量
    const string & GetThreads(){return this->msThreads;}
    //mysql地址
    const string & GetMysqlAddr(){return this->msMySqlAddr;}
    const string & GetMysqlUser(){return this->msMysqlUser;}
    const string & GetMysqlPwd(){return this->msMysqlPwd;}
    //是否打开安全检验,true or false
    const string & IsCheckSafe(){return this->msCheckSafe;}
    //是否打开AES加密,true or false
    const  string  & IsOpenAes(){return this->msAes;}
    //获取本机内网IP
    const string & GetLocalNetworkIP(){return this->msLocalNetworkIP;}
    //获取本机内网IP
    const string & GetIosSandbox(){return this->msIosSandbox;}
    //获取私聊服务IP
    const string & GetFriendIP(){return this->msFriendIP;}
    //获取广播服务IP
    const string & GetBroadcastIP(){return this->msBroadcastIP;}
    //获取用户服务IP
    const string & GetUserIP(){return this->msUserIP;}
    const string & GetOtherPushAddr(){return this->msOtherPushAddr;}
    //初始化
    int Init(const string& sServiceName);

private:    
    //调试日志
    string msDebugLog;
    //是否使用https
    string msOpenSSL;
    //启动的线程数量
    string msThreads;
    //mysql地址
    string msMySqlAddr;
    string msMysqlUser;
    string msMysqlPwd;
    //是否打开安全检验
    string msCheckSafe;
    //是否打开AES加密
    string msAes;
    //本机内网IP
    string msLocalNetworkIP;
    //IOS测试选项
    string msIosSandbox;

    //主动获取数据-私聊服务IP
    string msFriendIP;
    //主动获取数据-广播服务IP
    string msBroadcastIP;
    //户服务IP
    string msUserIP;
    string msRedisAddr;
    string msOtherPushAddr;
};


#endif // TDAL_H
