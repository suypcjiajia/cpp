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
    //是否打开安全检验,true or false
    const string & IsCheckSafe(){return this->msCheckSafe;}
    //是否打开AES加密,true or false
    const  string  & IsOpenAes(){return this->msAes;}
    //获取推送服务IP
    const  string& GetPushIP(){return this->msPushIP;}
    //获取是否打开红包超时计算
    const  string& GetLkMoneyTimeOutCalu(){return this->mLkMoneyTimeOutCalu;}

    //初始化
    int Init(const string& sServiceName);

    //短信服务
    const string & GetPhonesmsIP(){return this->msPhonesmsIP;}
    //商家后台IP
    const string & GetMepayIP(){return this->msMepayIP;}
    const string & GetWalletIP(){return this->mWalletIP;}
    const string & GetFansPerLots(){return this->mFansPerLots;}

    const string & GetAppKey(){return this->appKey;}
    const string & GetAppSecret(){return this->appSecret;}

    //yu server
    const string & GetFinanceIP(){return this->mFinanceIP;}
    //sun  wei server
    const string & GetSunWeiIP(){return this->mSunWeiIP;}
    //hong kong server
    const string & GetHongkongIP(){return this->mHongkong;}
    //kong hai server
    const string & GetKonhaiIP(){return this->mKunhai;}


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
    //是否打开安全检验
    string msCheckSafe;
    //是否打开AES加密
    string msAes;
    //短信服务
    string msPhonesmsIP ;
    //商家后台IP
    string msMepayIP ;
    //推送服务IP
    string msPushIP;
    //是否打开红包超时计算
    string mLkMoneyTimeOutCalu;
    //红包转帐
    string mWalletIP;

    string mFansPerLots;

    string appKey;
    string appSecret;
    string mFinanceIP;//余总财务IP
    string mSunWeiIP;//孙伟IP
    string mHongkong;//Hong kong
    string mKunhai;//Kunhai


};


#endif // TDAL_H
