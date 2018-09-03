#ifndef TBLL_H
#define TBLL_H

#include <string>
#include "TMutex.hpp"
#include <json/json.h>
#include <bll/PhoneCodeBLL.h>
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
    //获取Evhtp启动的线程数量
    const string & GetEvhtpThreads(){return this->msEvthtpThreads;}

    //是否打开安全检验,true or false
    const string & IsCheckSafe(){return this->msCheckSafe;}
    //是否打开AES加密,true or false
    const  string  & IsOpenAes(){return this->msAes;}

    //初始化
    int Init(const string& sServiceName);

private:    
    //调试日志
    string msDebugLog;
    //是否使用https
    string msOpenSSL;
    //Evhtp启动的线程数量
    string msEvthtpThreads;
    //是否打开安全检验
    string msCheckSafe;
    //是否打开AES加密
    string msAes;

};


#endif // TDAL_H
