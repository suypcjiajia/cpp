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
    //获取Evhtp启动的线程数量
    const string & GetEvhtpThreads(){return this->msEvthtpThreads;}
    //mysql地址
    const string & GetMysqlAddr(){return this->msMySqlAddr;}
    //是否打开安全检验,true or false
    const string & IsCheckSafe(){return this->msCheckSafe;}
    //是否打开AES加密,true or false
    const  string  & IsOpenAes(){return this->msAes;}
    //获取推广人收百分比
    float GetPePush(){return miPePush;}
    //获取分帐运营手续百分比
    float GetPeRun(){return miPeRun;}
    //获取不分帐运营手续百分比
    float GetNDPeRun(){return miNDPeRun;}
    //获取个代百分比
    float GetPePerAg(){return miPePerAg;}
    //获取县代百分比
    float GetPeCountyAg(){return miPeCountyAg;}
    //获取省代百分比
    float GetPeProAg(){return miPeProAg;}
    //获取购物币百分比
    float GetPeSpTicket(const int &  piDiscount){
        if(piDiscount == 95){
            return 0.25;
        }else if( piDiscount == 90){
            return 0.5;
        }else if( piDiscount == 85){
            return 0.75;
        }else if( piDiscount == 80){
            return 1.0;
        }else if( piDiscount == 85){
            return 1.25;
        }else if( piDiscount == 85){
            return 1.5;
        }else if( piDiscount == 85){
            return 1.75;
        }else if( piDiscount == 85){
            return 2.0;
        }
    }
    //第一次关注平台主的收益百分比
    float GetFstAttPltAg(){
        return 0.2;
    }

    //获取短信服务IP
    string GetPhonesmsIP(){
        return msPhonesmsIP;
    }

    //获取IM后台IP
    string GetMechatIP(){
        return msMechatIP;
    }

    //get 乐兑商城IP
    const string & GetLeduiShop(){return this->mLeduiShop;}

    //get wallet IP
    const string & GetWalletIP(){return this->mWalletIP;}

    //get OpendiaoDu
    const string & GetOpendiaoDu(){return this->mOpendiaoDu;}

    //初始化
    int Init(const string& sServiceName);

private:
    //调试日志
    string msDebugLog;
    //是否使用https
    string msOpenSSL;
    //Evhtp启动的线程数量
    string msEvthtpThreads;
    //mysql地址
    string msMySqlAddr;
    //是否打开安全检验
    string msCheckSafe;
    //是否打开AES加密
    string msAes;
    //推广人收百分比
    const float miPePush = 0.002;
    //分帐运营手续百分比
    const float miPeRun = 0.011;
    //不分帐运营手续百分比
    const float miNDPeRun = 0.008;
    //个代百分比
    const float miPePerAg = 0.266;
    //区代百分比
    const float miPeCountyAg = 0.266    ;
    //省代百分比
    const float miPeProAg = 0.066;
    //短信服务
    string msPhonesmsIP ;
    //IM后台IP
    string msMechatIP ;
    //乐兑商城IP
    string mLeduiShop;
    //
    string mWalletIP;
    string mOpendiaoDu;
};


#endif // TDAL_H
