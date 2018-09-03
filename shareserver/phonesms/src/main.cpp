#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include "log/Logger.h"
#include <string>
#include <bll/TBLL.h>
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "bll/PhoneCodeBLL.h"
#include "dal/MysqlConnect.h"

using namespace std;

// Evhtp通信回调函数
void ProcessCall(EvhtpSvr * pEvhtp,const  Json::Value  & pJsonValue, Json::Value & pResponse)
{
    string psBusiness = pEvhtp->GetQuery("sBusiness");
    string psMethod = pEvhtp->GetQuery("sMethod");
    if (psBusiness == "phonesms"){
        PhoneCodeBLL phoneCodeBLL;
        phoneCodeBLL.Process(psMethod,pJsonValue,pResponse);
    } else {
        RetJsonMsg::SetRetMsg(-12,pResponse);
    }
}

int main(int argc, const char* argv[]) {           
    //服务名称
    string sServiceName = argv[0];
    int iPos = sServiceName.find_last_of("/");
    sServiceName = sServiceName.substr(iPos+1);

    //查看版本，命令行下执行 "程序名 -version -v"
    if(argc == 2){
        if( strcmp(argv[1],"-version" ) == 0 || strcmp(argv[1],"-v" ) == 0){
            printf("1.19\n");//每次版本升级，这里必须递增修改，比如最新版本是1.02，那这次就是1.03
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
#ifdef SMSWAI
    TMultiMysqlDAL::SetMysqlAddr("rm-wz9c618cpa59o812l.mysql.rds.aliyuncs.com");
#else
    TMultiMysqlDAL::SetMysqlAddr("127.0.0.1");
#endif
    TMultiMysqlDAL::SetMysqlUser("wallet");
    TMultiMysqlDAL::SetMysqlPwd("Wallet1234");
    MysqlConnect::GetInstance()->Init("walletdb");


    //日志
    appendlog(TTDLogger::LOG_INFO,"%s::main start",sServiceName.c_str());


    //启动http or https服务
    EvhtpSvr::Init(sServiceName);
    EvhtpSvr::StartSvr(ProcessCall,13240,TBLL::GetInstance()->IsUseSSL(),TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());
    EvhtpSvr::StartSvr(ProcessCall,23240,"false",TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());

    while(true){
        sleep(2);
    }
    return 0;
}
