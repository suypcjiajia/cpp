#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include "log/Logger.h"
#include <string>
#include <dal/MysqlConnect.h>
#include <bll/TBLL.h>
#include "dal/TDAL.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "bll/UserDataBLL.h"
#include "bll/UserStateBLL.h"
#include "bll/FriendResquestBLL.h"
#include "bll/UserRelationBLL.h"
#include "bll/PrivateChatBLL.h"
#include "bll/UserSetBLL.h"
#include "bll/LukyMoneyBLL.h"
#include "bll/PushBLL.h"
#include "bll/GroupChatBLL.h"
#include "bll/GroupDataBLL.h"
#include "bll/GroupMemberBLL.h"
#include "bll/LukyMoneyBLL.h"
#include "bll/NoticeBLL.h"
#include "bll/PublicFansBLL.h"
#include "bll/FansBankBLL.h"
#include "bll/CommunityBLL.h"
#include "com/EvhtpSvr.h"
#include "Token.h"
#include "com/Request.h"
#include "com/TCurl.h"
#include "Base64EnDecode.h"
#include "MyUtil.h"

#include<iostream>
#include<iomanip>
#include <math.h>
using namespace std;



// Evhtp通信回调函数
void ProcessCall(EvhtpSvr * pEvhtp,const Json::Value& pJsonValue,Json::Value & pResponse)
{
    string psBusiness = pEvhtp->GetQuery("sBusiness");
    string psMethod = pEvhtp->GetQuery("sMethod");
    if (psBusiness == "UserRegister" || psBusiness == "UserQuery" || psBusiness == "UserData"){

        Json::Value jPost = pJsonValue;

        if( psMethod == "UpdateData"){
            string versions = pEvhtp->GetHead("versions");
            string token = pEvhtp->GetHead("token");
            string clientType = pEvhtp->GetHead("clientType");


            appendlog(TTDLogger::LOG_INFO,"ProcessCall UpdateData versions:%s token:%s clientType:%s",versions.c_str(),token.c_str(),clientType.c_str());

            jPost["bGive"] = false;
            if( (versions >= "3.9.2" && clientType == "01" ) || (versions >= "4.5" && clientType == "2" )){//IOs 3.9.2 android 4.5
                //check token
                string sDeAes = Token::UnLeiduLoginToken(token);
//                if( sDeAes.empty()){
//                    RetJsonMsg::SetRetMsg(-1,pResponse,"身份验证失败");
//                    return;
//                }
                jPost["bGive"] = true;
                jPost["IdFromToken"] = sDeAes;
            }


        }



        UserDataBLL userDataBLL;
        userDataBLL.Process(psMethod,jPost,pResponse);
    } else if ( psBusiness == "UserState" ){
        UserStateBLL userStateBLL;
        userStateBLL.Process(psMethod,pJsonValue,pResponse);
    }else if ( psBusiness == "FriendResquest" ){
        FriendResquestBLL friResBLL;
        friResBLL.Process(psMethod,pJsonValue,pResponse);
    }else if ( psBusiness == "UserRelation" ){
        UserRelationBLL relaiotnBLL;
        relaiotnBLL.Process(psMethod,pJsonValue,pResponse);
    }else if ( psBusiness == "PrivateChat" ){

        if( psMethod == "Send"){

//            string token = pEvhtp->GetHead("sToken");
//            //check token
//            string sDeAes = Token::UnAesLoginToken(token);
//            if( sDeAes.empty()){
//                RetJsonMsg::SetRetMsg(-1,pResponse,"身份验证失败");
//                return;
//            }
        }

        PrivateChatBLL prichatBLL;
        prichatBLL.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "UserSet"){
        UserSetBLL usersetBll;
        usersetBll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "GroupChat"){
        GroupChatBLL Bll;
        Bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "GroupData"){
        GroupDataBLL Bll;
        Bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "GroupMember"){
        GroupMemberBLL Bll;
        Bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "LukyMoney"){
        LukyMoneyBLL Bll;
        Bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "Notice"){
        NoticeBLL Bll;
        Bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "PublicFans"){
        PublicFansBLL bll;
        bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "FansBank"){
        FansBankBLL bll;
        bll.Process(psMethod,pJsonValue,pResponse);
    }else if( psBusiness == "Community"){
        CommunityBLL bll;
        bll.Process(psMethod,pJsonValue,pResponse);
    }
    else {
        RetJsonMsg::SetRetMsg(-12,pResponse);
    }
}

// Evhtp通信回调函数,处理带token，不加密的情况
void ProcessCall2(EvhtpSvr * pEvhtp, const  Json::Value  & pJsonValue, Json::Value & pResponse)
{
    pResponse.clear();

    string sUrl = pEvhtp->GetUrl();

    Json::Value jPost;

    if( sUrl.substr(0,9) == "/user/jie"){
        UserDataBLL userDataBLL;


        userDataBLL.Jiedong(pJsonValue,pResponse);
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
            printf("1.46\n");//每次版本升级，这里必须递增修改，比如最新版本是1.02，那这次就是1.03
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


    PushBLL::GetInstance()->Start();



    //启动http or https服务
    EvhtpSvr::Init(sServiceName);
    EvhtpSvr::StartSvr(ProcessCall,13241,TBLL::GetInstance()->IsUseSSL(),TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());
    EvhtpSvr::StartSvr(ProcessCall,23241,"false",TBLL::GetInstance()->GetEvhtpThreads(),TBLL::GetInstance()->IsOpenAes(),TBLL::GetInstance()->IsCheckSafe());

    EvhtpSvr::SetProcessCallBack2(ProcessCall2);


    while(true){
        sleep(2);
    }
    return 0;
}
