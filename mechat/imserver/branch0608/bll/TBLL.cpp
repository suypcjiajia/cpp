#include "TBLL.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"
#include "UserDataBLL.h"
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
    tFile.GetValue(sConfig,"MySqlAddr",this->msMySqlAddr);
    if(msMySqlAddr.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set MySqlAddr");
        return -1;
    }
    tFile.GetValue(sConfig,"MySqlUser",this->msMysqlUser);
    if(msMysqlUser.empty()){
        msMysqlUser = "mechat";
    }
    tFile.GetValue(sConfig,"MySqlPwd",this->msMysqlPwd);
    if(msMysqlPwd.empty()){
        msMysqlPwd = "Mechat1234";
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
    tFile.GetValue(sConfig,"MepayIP",this->msMepayIP);
    if(msMepayIP.empty()){
        msMepayIP = "120.25.129.101";
    }
    tFile.GetValue(sConfig,"PhonesmsIP",this->msPhonesmsIP);
    if(msPhonesmsIP.empty()){
        msPhonesmsIP = "120.25.129.101";
    }
    tFile.GetValue(sConfig,"sPushIP",this->msPushIP);
    if(msPushIP.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set sPushIP");
        return -1;
    }
    tFile.GetValue(sConfig,"LkMoneyTimeOutCalu",this->mLkMoneyTimeOutCalu);
    if(mLkMoneyTimeOutCalu.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set LkMoneyTimeOutCalu");
        return -1;
    }

    tFile.GetValue(sConfig,"WalletIP",this->mWalletIP);
    if(mWalletIP.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set WalletIP");
        return -1;
    }

    tFile.GetValue(sConfig,"FansPerLots",this->mFansPerLots);
    if(mFansPerLots.empty()){
        appendlog(TTDLogger::LOG_ERROR,"TBLL::Init not set FansPerLots");
        return -1;
    }

    tFile.GetValue(sConfig,"appKey",this->appKey);


    tFile.GetValue(sConfig,"appSecret",this->appSecret);

    tFile.GetValue(sConfig,"FinanceIP",this->mFinanceIP);

    tFile.GetValue(sConfig,"SunWeiIP",this->mSunWeiIP);
    if(mSunWeiIP.empty()){
        appendlog(TTDLogger::LOG_ERROR,"not set SunWeiIP");
        return -1;
    }

    tFile.GetValue(sConfig,"hongkong",this->mHongkong);
    if(mHongkong.empty()){
        appendlog(TTDLogger::LOG_ERROR,"not set hongkong");
        return -1;
    }
    tFile.GetValue(sConfig,"kunhai",this->mKunhai);
    if(mKunhai.empty()){
        appendlog(TTDLogger::LOG_ERROR,"not set mKunhai");
        return -1;
    }


    //
    TMultiMysqlDAL::SetMysqlAddr(msMySqlAddr);
    TMultiMysqlDAL::SetMysqlUser(msMysqlUser);
    TMultiMysqlDAL::SetMysqlPwd(msMysqlPwd);

    return 0;
}

