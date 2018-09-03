#include "TBLL.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"
#include "ConnHashMapBLL.h"
#include "dal/RedisConnect.h"
TBLL* TBLL::mInstance = NULL;
TBLL* TBLL::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new TBLL;
        }
        return mInstance;
}
//初始化
int TBLL::Init(const string& sServiceName)
{
    //配置文件
    const string sConfig = "/tiyoume/"+sServiceName+"/push.conf";
    //读配置文件
    TFile tFile;

    if (tFile.IsFileExist(sConfig)) {
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not file=%s",sConfig.c_str());
        return -1;
    }
    tFile.GetValue(sConfig,"DebugLog",this->msDebugLog);
    if(msDebugLog.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set DebugLog");
        return -1;
    }
    tFile.GetValue(sConfig,"OpenSSL",this->msOpenSSL);
    if(msOpenSSL.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set OpenSSL");
        return -1;
    }
    tFile.GetValue(sConfig,"Threads",this->msThreads);
    if(msThreads.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set Threads");
        return -1;
    }
    tFile.GetValue(sConfig,"MySqlAddr",this->msMySqlAddr);
    if(msMySqlAddr.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set MySqlAddr");
        return -1;
    }
    tFile.GetValue(sConfig,"CheckSafe",this->msCheckSafe);
    if(msCheckSafe.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set CheckSafe");
        return -1;
    }
    tFile.GetValue(sConfig,"Aes",this->msAes);
    if(msAes.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set Aes");
        return -1;
    }
    tFile.GetValue(sConfig,"sLocalNetworkIP",this->msLocalNetworkIP);
    if(msLocalNetworkIP.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set sLocalNetworkIP");
        return -1;
    }
    tFile.GetValue(sConfig,"IosSandbox",this->msIosSandbox);
    if(msIosSandbox.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set IosSandbox");
        return -1;
    }

    tFile.GetValue(sConfig,"RedisAddr",msRedisAddr);
    if(msRedisAddr.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set RedisAddr");
        return -1;
    }

    tFile.GetValue(sConfig,"OtherPushAddr",msOtherPushAddr);


    //
    TMultiMysqlDAL::SetMysqlAddr(msMySqlAddr);
    TMultiMysqlDAL::SetMysqlUser("mechat");
    TMultiMysqlDAL::SetMysqlPwd("Mechat1234");

    RedisConnect::GetInstance()->SetAddress(msRedisAddr);

    return 0;
}

