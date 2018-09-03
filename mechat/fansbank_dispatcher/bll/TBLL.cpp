#include "TBLL.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"

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
    const string sConfig = "/tiyoume/"+sServiceName+"/"  + sServiceName +  ".conf";
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

    tFile.GetValue(sConfig,"MySqlAddr",this->msMySqlAddr);
    if(msMySqlAddr.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set MySqlAddr");
        return -1;
    }
    tFile.GetValue(sConfig,"MepayIP",this->msMepayIP);
    if(msMepayIP.empty()){
        msMepayIP = "120.25.129.101";
    }
    tFile.GetValue(sConfig,"PhonesmsIP",this->msPhonesmsIP);
    if(msPhonesmsIP.empty()){
        msPhonesmsIP = "120.25.129.101";
    }

    tFile.GetValue(sConfig,"WalletIP",this->mWalletIP);
    if(mWalletIP.empty()){
        mWalletIP = "mepay.tymplus.com:10001";
    }

    tFile.GetValue(sConfig,"ImIP",this->msImIP);
    if(msImIP.empty()){
        msImIP = "120.25.129.101";
    }

    tFile.GetValue(sConfig,"appKey",this->appKey);


    tFile.GetValue(sConfig,"appSecret",this->appSecret);




    //
    TMultiMysqlDAL::SetMysqlAddr(msMySqlAddr);
    TMultiMysqlDAL::SetMysqlUser("mechat");
    TMultiMysqlDAL::SetMysqlPwd("Mechat1234");


    return 0;
}

