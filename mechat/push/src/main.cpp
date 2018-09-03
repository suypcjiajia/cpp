#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include "log/Logger.h"
#include "TConvert.h"
#include <string>
#include <dal/MysqlConnect.h>
#include <bll/TBLL.h>
#include "dal/TDAL.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "bll/WsServerBLL.h"
#include "bll/PushDataBLL.h"
#include "dal/RedisCli.h"
#include <stdlib.h>
#include "com/ios_push/original_ssl_client.h"
#include "dal/ImDAL.h"


using namespace std;

// Evhtp通信回调函数
void ProcessCall(EvhtpSvr * pEvhtp,const Json::Value& pJsonValue,Json::Value & pResponse)
{
    string psBusiness = pEvhtp->GetQuery("sBusiness");
    string psMethod = pEvhtp->GetQuery("sMethod");
    if (psBusiness == "PushData" ){
        PushDataBLL pushDataBLL;
        pushDataBLL.Process(psMethod,pJsonValue,pResponse);
    } else {
        RetJsonMsg::SetRetMsg(-12,pResponse);
    }
}

//线程
void* WsServerThread(void*ptr)
{
    pthread_detach(pthread_self());

    //启动ws服务
    TConvert tConvert;
    WsServerBLL server;
    server.run( 23242, tConvert.StrToInt( TBLL::GetInstance()->GetThreads() ) );

    pthread_exit(NULL);
}

int main(int argc, const char* argv[]) {

    //    RedisCli redisdal;int iRet2 = 0;
    //    redisdal.SetAddrs("192.168.168.58:7000,192.168.168.58:7001,192.168.168.58:7002,192.168.168.69:7000,192.168.168.69:7001,192.168.168.69:7002");

    //    iRet2 = redisdal.HsetSync("hk2","f2","suypyjtkjlg");

    //    char buf[256] = {0};
    //    iRet2 = redisdal.HgetSync("hk2", "f11",buf);
    //    return 0;




    //服务名称
    string sServiceName = argv[0];
    int iPos = sServiceName.find_last_of("/");
    sServiceName = sServiceName.substr(iPos+1);

    //查看版本，命令行下执行 "程序名 -version -v"
    if(argc == 2){
        if( strcmp(argv[1],"-version" ) == 0 || strcmp(argv[1],"-v" ) == 0){
            printf("1.21\n");//每次版本升级，这里必须递增修改，比如最新版本是1.02，那这次就是1.03
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

    //初始化
    if ( 0 != TDAL::GetInstance()->Init()) {
        return -1;
    }

    //日志
    appendlog(TTDLogger::LOG_INFO,"%s::main",sServiceName.c_str());

    //启动ws服务
    pthread_t tid1;
    int iRet = pthread_create( &tid1, NULL, WsServerThread, (void*)NULL );
    if ( 0 != iRet ) {
        appendlog(TTDLogger::LOG_INFO,"main pthread_create() failure");
        return -1;
    }

    //启动http or https服务
    EvhtpSvr::Init(sServiceName);
    EvhtpSvr::StartSvr(ProcessCall,13243,TBLL::GetInstance()->IsUseSSL(),TBLL::GetInstance()->GetThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());
    EvhtpSvr::StartSvr(ProcessCall,23243,"false",TBLL::GetInstance()->GetThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());



//    try {
//        //发送
//        original_ssl_client ios_ssl_client;
//        int iRet = ios_ssl_client.SslSendMsg("0d8e30e3 8259542e e432fb90 60de9405 42fe4f83 7473dad8 59777897 4b3f3347","{\"aps\":{\"alert\":\"This is some fancy message.\",\"badge\":6,\"sound\": \"default\"}}");
//        if (-1 == iRet){
//            return -1;
//        }
//    } catch ( const Exception &e ) {
//        appendlog(TTDLogger::LOG_ERROR,"UserWorkBLL::SendDataByIOS err=%s ",e.what());
//        return -1;
//    }


    while(true){
        sleep(2);
    }
    return 0;
}
