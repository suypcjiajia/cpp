#include "TBLL.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"

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
    const string sConfig = "/tiyoume/"+sServiceName+"/config.cnf";
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
    tFile.GetValue(sConfig,"EvthtpThreads",this->msEvthtpThreads);
    if(msEvthtpThreads.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set EvthtpThreads");
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

    //其它BLL逻辑的Init


    return 0;
}

