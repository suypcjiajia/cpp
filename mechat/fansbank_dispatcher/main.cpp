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
#include "json/json.h"
#include "TConvert.h"
#include "com/RetJsonMsg.hpp"



#include "bll/Priceispatch.h"

#include "com/Request.h"



int main(int argc, char *argv[])
{



    //服务名称
    string sServiceName = "fansbank_dispatcher";

    //查看版本，命令行下执行 "程序名 -version -v"
    if(argc == 2){
        if( strcmp(argv[1],"-version" ) == 0 || strcmp(argv[1],"-v" ) == 0){
            printf("1.01\n");//每次版本升级，这里必须递增修改，比如最新版本是1.02，那这次就是1.03
            exit(0);
        }
    }



    //定义日志输出路径
    TTDLogger::Logger::getInstance()->setPath(string("/log/"+sServiceName + "/").c_str());

    //初始化
    if ( 0 != TBLL::GetInstance()->Init( sServiceName )) {
        return -1;
    }

    if ("true" == TBLL::GetInstance()->GetDebugLog()) {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_DEBUG);
    } else {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_INFO);
    }


    //初始化要操作的数据库
    MysqlConnect::GetInstance()->Init("mechatdb");

    //日志
    appendlog(TTDLogger::LOG_INFO,"%s::main",sServiceName.c_str());

    PriceDispatch dispatch;
    dispatch.Run();


    return 0;
}
