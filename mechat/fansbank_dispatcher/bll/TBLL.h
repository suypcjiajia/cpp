#ifndef TBLL_H
#define TBLL_H

#include <string>
#include "TMutex.hpp"
#include <json/json.h>
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
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
    //获取Evhtp启动的线程数量
    const string & GetEvhtpThreads(){return this->msEvthtpThreads;}
    //mysql地址
    const string & GetMysqlAddr(){return this->msMySqlAddr;}
    const string & GetMysqlUser(){return this->msMysqlUser;}
    const string & GetMysqlPwd(){return this->msMysqlPwd;}
    //获取推送服务IP
    const  string& GetPushIP(){return this->msPushIP;}

    //初始化
    int Init(const string& sServiceName);

    //短信服务
    const string & GetPhonesmsIP(){return this->msPhonesmsIP;}
    //商家后台IP
    const string & GetMepayIP(){return this->msMepayIP;}
    const string & GetWalletIP(){return this->mWalletIP;}
    const string & GetImIP(){return this->msImIP;}
    const string & GetAppKey(){return this->appKey;}
    const string & GetAppSecret(){return this->appSecret;}

private:    
    //调试日志
    string msDebugLog;
    //是否使用https
    string msOpenSSL;
    //Evhtp启动的线程数量
    string msEvthtpThreads;
    //mysql地址
    string msMySqlAddr;
    string msMysqlUser;
    string msMysqlPwd;
    //短信服务
    string msPhonesmsIP ;
    //商家后台IP
    string msMepayIP ;
    //推送服务IP
    string msPushIP;
    //红包转帐
    string mWalletIP;
    string msImIP;
    string appKey;
    string appSecret;

};


#endif // TDAL_H
