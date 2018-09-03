#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include "log/Logger.h"
#include <string>
#include "dal/MysqlConnect.h"
#include <bll/TBLL.h>
#include "com/EvhtpSvr.h"
#include "bll/ShopInfoBLL.h"
#include "bll/WalletAccountBLL.h"
#include "bll/WalletDayBookBLL.h"
#include "bll/ZoneAgnetBLL.h"
#include "bll/PersonalAgentBLL.h"
#include "bll/ServerBLL.h"
#include "bll/BankCardBLL.h"
#include "json/json.h"
#include "TConvert.h"
#include "com/RetJsonMsg.hpp"
#include "bll/Dispathcer.h"
#include "bll/CouponBLL.h"
#include "bll/PowerBLL.h"

using namespace std;


#define MAKE_BUSINESS(business)     p  = business##BLL::GetInstance();\
    if( p != NULL){\
    if ( 0 != p->Init() ) {\
    appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::MakeBLL %sBLL init error ",#business);\
    exit(0);\
    }\
    mBLLMap[#business] = p;\
    }
//业务工厂
class BusinessFactory{
public:
    void MakeBLL();
    BaseBLL * GetBLL(const string & pType);
private:
    map<string ,BaseBLL*> mBLLMap;
};

//添加业务插件
void BusinessFactory::MakeBLL(){
    //在这里添加业务插件
    BaseBLL * p  = NULL;
    //第一个业务插件
    MAKE_BUSINESS(ZoneAgent)
    //第2个业务插件
    MAKE_BUSINESS(ShopInfo)

    //第6个业务插件
    MAKE_BUSINESS(BankCard)

    MAKE_BUSINESS(Coupon)

    MAKE_BUSINESS(Power)



   //添加下一个业务插件
}

//GET业务插件
BaseBLL * BusinessFactory::GetBLL(const string & pType){
    map<string ,BaseBLL*>::iterator it =  mBLLMap.find(pType);
    if( it != mBLLMap.end()){
        return it->second;
    }else{
        return NULL;
    }
}
BusinessFactory bf;
// Evhtp通信回调函数
void ProcessCall(EvhtpSvr * pEvhtp,const  Json::Value  & pJsonValue, Json::Value & pResponse)
{
    string psBusiness = pEvhtp->GetQuery("sBusiness");
    string psMethod = pEvhtp->GetQuery("sMethod");
    pResponse.clear();
    BaseBLL * p = bf.GetBLL(psBusiness);
    if( p != NULL){
        p->Process(psMethod,pJsonValue,pResponse);
    }else{
        RetJsonMsg::SetRetMsg(-12,pResponse);
    }
}



// Evhtp通信回调函数,处理带token，不加密的情况
void ProcessCall2(EvhtpSvr * pEvhtp, const  Json::Value  & pJsonValue, Json::Value & pResponse)
{
    pResponse.clear();
    string sBusiness = "";
    string sUrl = pEvhtp->GetUrl();

    if( sUrl.substr(0,5) == "/near"){
       sBusiness = "ShopInfo";
    }else if( sUrl.substr(0,7) == "/coupon"){
        sBusiness = "Coupon";
    }
    else if( sUrl.substr(0,6) == "/power"){
            sBusiness = "Power";
    }
    else if( sUrl.substr(0,10) == "/zoneAgent"){
            sBusiness = "ZoneAgent";
    }
    BaseBLL * p = bf.GetBLL(sBusiness);
    if( p != NULL){
        p->Process2(pEvhtp,pJsonValue,pResponse);
    }
}




int main(int argc, const char* argv[]) {

   // MyAES aes;
   // aes.SetKey((BYTE * )"meizhifu",8);
   // string de = aes.Decrypt("B2v5l45jEr66zoW+0PJaOCdNPGgLegdidSO+sbdyiA+BSr2jWdB9i0H0zTTjC6Hqf8YvG3z8tn1+YGK0tdBqLC1GJUE2w3jayQuo1SrwrTlPI9AxQTeLnkzpmQijdFLQCwobbWQIoyXIAQC3YfnptHmXBUw2uU+F4QPppOeRXVg=",4);

    //string s = "a=1&b=2";
    //EvhtpSvr::ToJson(s);




    //服务名称
    string sServiceName = "MepayHttp";

    //查看版本，命令行下执行 "程序名 -version -v"
    if(argc == 2){
        if( strcmp(argv[1],"-version" ) == 0 || strcmp(argv[1],"-v" ) == 0){
            printf("1.89\n");//每次版本升级，这里必须递增修改，比如最新版本是1.02，那这次就是1.03
            exit(0);
        }
    }
    //初始化
    if ( 0 != TBLL::GetInstance()->Init( sServiceName )) {
        return -1;
    }

    //定义日志输出路径
    TTDLogger::Logger::getInstance()->setPath(string("/log/"+sServiceName + "/").c_str());

    if ("true" == TBLL::GetInstance()->GetDebugLog()) {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_DEBUG);
    } else {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_INFO);
    }

    //初始化要操作的数据库
    MysqlConnect::GetInstance()->Init("walletdb");

    //日志
    appendlog(TTDLogger::LOG_INFO,"%s::main",sServiceName.c_str());

    bf.MakeBLL();

    //启动http服务
    EvhtpSvr::Init(sServiceName);
    EvhtpSvr::StartSvr(ProcessCall,1314,TBLL::GetInstance()->IsUseSSL(),TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());
    EvhtpSvr::StartSvr(ProcessCall,2314,"false",TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());

    EvhtpSvr::SetProcessCallBack2(ProcessCall2);


    Dispathcer dispathcer;
    dispathcer.Run();

    while(true){
        sleep(2);
    }
    return 0;
}
