#include "TBLL.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "dal/TMultiMysqlDAL.h"

/*
walletdb
wallet
Wallet1234
mechatdb
mechat
Mechat1234
*/
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
    const string sConfig = "/tiyoume/"+sServiceName+"/"+sServiceName+".conf";
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

    tFile.GetValue(sConfig,"PhonesmsIP",this->msPhonesmsIP);
    if(msPhonesmsIP.empty()){
        msPhonesmsIP = "120.76.154.15";
    }
    tFile.GetValue(sConfig,"MechatIP",this->msMechatIP);
    if(msMechatIP.empty()){
        msMechatIP = "120.76.84.231";
    }


    tFile.GetValue(sConfig,"LeduiShop",this->mLeduiShop);
    if(mLeduiShop.empty()){
        mLeduiShop = "appbs.tymshop.com:8088";
    }

    tFile.GetValue(sConfig,"WalletIP",this->mWalletIP);
    if(mWalletIP.empty()){
        mWalletIP = "mepay.tymplus.com";
    }

    tFile.GetValue(sConfig,"OpendiaoDu",this->mOpendiaoDu);
    if(mOpendiaoDu.empty()){
        mOpendiaoDu = "true";
    }


    TMultiMysqlDAL::SetMysqlAddr(msMySqlAddr);
    TMultiMysqlDAL::SetMysqlUser("wallet");
    TMultiMysqlDAL::SetMysqlPwd("Wallet1234");

    return 0;
}
