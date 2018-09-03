#include "WsServerBLL.h"
#include "bll/PushDataBLL.h"
#include "com/RetJsonMsg.hpp"
#include "com/WsSendCOM.h"
#include "TConvert.h"
#include "ConnHashMapBLL.h"
#include "CmdValue.h"
#include "bll/ConnKeepLiveBLL.h"

#include <fstream>
//回调:接收消息
void WsServerBLL::on_message(connection_hdl pHdl, server::message_ptr msg) {

    OnMessage(&this->mServer,pHdl,msg->get_payload());
}
//回调：关闭链接
void WsServerBLL::on_close(connection_hdl hdl) {
    appendlog(TTDLogger::LOG_DEBUG,"WsServerBLL::on_close hdl:%ld",hdl.lock().get());
    ConnKeepLiveBLL::GetInstance()->del(hdl);
}
//回调：打开链接
void WsServerBLL::on_open(connection_hdl hdl)
{
    appendlog(TTDLogger::LOG_DEBUG,"WsServerBLL::on_open hdl:%ld",hdl.lock().get());
    ConnKeepLiveBLL::GetInstance()->Replace(hdl,&this->mServer);
}

//回调：ping
bool WsServerBLL::on_ping(connection_hdl hdl, string msg)
{
    return true;
}

//回调：pong
void WsServerBLL::on_pong(connection_hdl hdl, string msg)
{
    //appendlog(TTDLogger::LOG_DEBUG,"WsServerBLL::on_pong hdl:%ld msg:%s",hdl.lock().get(),msg.c_str());
    ConnKeepLiveBLL::GetInstance()->Replace(hdl,&this->mServer);
}

// No change to TLS init methods from echo_server_tls
//std::string get_password() {
//    return "test";
//}
//context_ptr on_tls_init(websocketpp::connection_hdl hdl) {
//    //std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
//    context_ptr ctx(new boost::asio::ssl::context(boost::asio::ssl::context::tlsv1));

//    try {
//        ctx->set_options(boost::asio::ssl::context::default_workarounds |
//                         boost::asio::ssl::context::no_sslv2 |
//                         boost::asio::ssl::context::no_sslv3 |
//                         boost::asio::ssl::context::single_dh_use);
//        ctx->set_password_callback(bind(&get_password));
//        ctx->use_certificate_chain_file("/certs/cert.pem");
//        ctx->use_private_key_file("/certs/key.pem", boost::asio::ssl::context::pem);
//    } catch (std::exception& e) {
//        appendlog(TTDLogger::LOG_ERROR,"WsServerBLL::on_tls_init exception=%s",e.what());
//    }
//    return ctx;
//}
void* PushOffline(void*ptr)
{
    sleep(60*2);
    ifstream infile;
    infile.open("./1.txt");   //将文件流对象与文件连接起来

    TConvert tConvert;
    string s;PushDataBLL push;
    while(getline(infile,s))
    {
        push.PushOffline(tConvert.StrToLong(s));
    }
    infile.close();             //关闭文件输入流
}

void WsServerBLL::run(uint16_t pPort,int iThreads ) {


    pthread_t tid1;
    pthread_create( &tid1, NULL, PushOffline, (void*)NULL );


    ConnKeepLiveBLL::GetInstance()->Start();

    try {
        this->mServer.set_access_channels(websocketpp::log::alevel::none);
        this->mServer.clear_access_channels(websocketpp::log::alevel::none);
        //this->mServer.set
        this->mServer.init_asio();

        this->mServer.set_open_handler(bind(&WsServerBLL::on_open,this,::_1));
        this->mServer.set_close_handler(bind(&WsServerBLL::on_close,this,::_1));
        this->mServer.set_message_handler(bind(&WsServerBLL::on_message,this,::_1,::_2));
        this->mServer.set_ping_handler(bind(&WsServerBLL::on_ping,this,::_1,::_2));
        this->mServer.set_pong_handler(bind(&WsServerBLL::on_pong,this,::_1,::_2));
        this->mServer.set_reuse_addr(true);



        //this->mServer.set_tls_init_handler(bind(&on_tls_init,::_1));


        this->mServer.listen(pPort);
        this->mServer.start_accept();

        typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_ptr;
        std::vector<thread_ptr> ts;
        for (int i = 0; i < iThreads; i++) {
            ts.push_back(websocketpp::lib::make_shared<websocketpp::lib::thread>(&server::run, &this->mServer));
        }

        for (int i = 0; i < iThreads; i++) {
            ts[i]->join();
        }

    } catch (websocketpp::exception const & e) {
        appendlog(TTDLogger::LOG_ERROR,"WsServerBLL::run exception=%s",e.what());
    }


}


//消息接口
void WsServerBLL::OnMessage(server* pServer, connection_hdl pHdl, const string& psMsg)
{
    appendlog(TTDLogger::LOG_DEBUG,"WsServerBLL::OnMessage hdl:%ld, Msg:%s",pHdl.lock().get(),psMsg.c_str());
    ConnKeepLiveBLL::GetInstance()->Replace(pHdl,pServer);

    int iRet = -1;
    WsSendCOM wsSendCOM;
    string sMsg = wsSendCOM.WsReceive(psMsg);

    //数据合法性
    Json::Value pResponse;
    Json::Value jsonValue;
    TConvert tConvert;
    if ( -1 == tConvert.Str2Json(sMsg, jsonValue)){
        //JOSN格式错误
        RetJsonMsg::SetRetMsg(-20,pResponse);
        try {
            WsSendCOM wsSendCOM;
            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
        } catch (const websocketpp::exception& e) {
            appendlog(TTDLogger::LOG_ERROR,"WsServerBLL::OnMessage  err:%s", e.what());
        }
        return;
    }
    if( ! jsonValue["iRet"].isNull() && ! jsonValue["iCmd"].isNull()){//客服端收到消息后，客服端发个确认的消息
        int iRet = tConvert.Json2Int(jsonValue["iRet"]);
        int iCmd = tConvert.Json2Int(jsonValue["iCmd"]);
        if( enum_cmd_chat== iCmd ){//确认收到私聊
            string sKey = tConvert.Json2String(jsonValue["sKey"]);
            PushDataBLL pushDataBLL;
            pushDataBLL.OnRetForChat(iRet,sKey);
        }else if( enum_cmd_group_chat== iCmd ){//确认收到群聊
            string sKey = tConvert.Json2String(jsonValue["sKey"]);
            long lUserId = tConvert.Json2Long(jsonValue["lUserId"]);
            PushDataBLL pushDataBLL;
            pushDataBLL.OnRetForGroupChat(iRet,sKey,lUserId);
        }else if( enum_cmd_public_chat== iCmd ){//确认收到公众号
            string sKey = tConvert.Json2String(jsonValue["sKey"]);
            long lUserId = tConvert.Json2Long(jsonValue["lUserId"]);
            PushDataBLL pushDataBLL;
            pushDataBLL.OnRetForPublicChat(iRet,sKey,lUserId);
        }

    }else if( ! jsonValue["iCmd"].isNull() && ! jsonValue["lUserId"].isNull() ) {//客服端发来业务消息

        //注册
        int iCmd = tConvert.Json2Int(jsonValue["iCmd"]);
        if( enum_cmd_register == iCmd ) {
            if( jsonValue["iPhoneType"].isNull() || jsonValue["sToken"].isNull() ) {
                //POST参数错误
                RetJsonMsg::SetRetMsg(-15,pResponse);
                try {
                    WsSendCOM wsSendCOM;
                    wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());

                } catch (const websocketpp::exception& e) {
                    appendlog(TTDLogger::LOG_ERROR,"WsServerBLL::OnMessage WsSendByServer err:%s", e.what());
                }
                return;
            }
            string sVer = "";
            if( !jsonValue["sVer"].isNull()){
                sVer = tConvert.Json2String(jsonValue["sVer"]);
            }
            string  sUserId = tConvert.Json2String(jsonValue["lUserId"]);
            if( sUserId.empty()){
                return ;
            }
            PushDataBLL pushDataBLL;
            iRet = pushDataBLL.Register(pServer,
                                        pHdl,
                                        tConvert.StrToLong(sUserId),
                                        tConvert.Json2Int(jsonValue["iPhoneType"]),
                    tConvert.Json2String(jsonValue["sToken"]),sVer);

            RetJsonMsg::SetRetMsg(iRet,pResponse);

            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
        }
        //下线
        else if( enum_cmd_offline == iCmd){
            PushDataBLL pushDataBLL;
            iRet = pushDataBLL.OffLine(pServer,pHdl,tConvert.Json2Long(jsonValue["lUserId"]) );
            RetJsonMsg::SetRetMsg(iRet,pResponse);
            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
            Close(pServer,pHdl);
        }
        //IOS推送角标清0
        else if( enum_cmd_clearmsgcount == iCmd){
            ConnHashMapBLL::GetInstance()->SetMsgCount(tConvert.Json2Long(jsonValue["lUserId"]),0);
        } else if( enum_cmd_get_registerinfo == iCmd){
            ConnHashMapEntity connINfo;
            if( ConnHashMapBLL::GetInstance()->Find(tConvert.Json2Long(jsonValue["lUserId"]),connINfo) == 0){
                pResponse["lUserId"] = tConvert.LongToStr(connINfo.lUserId);
                pResponse["iPhoneType"] = tConvert.IntToStr(connINfo.iPhoneType);
                pResponse["sTime"] = tConvert.TimeToStr(connINfo.lAddTime/1000000);
                pResponse["mHdl"] = tConvert.LongToStr((long)connINfo.mHdl.lock().get());
                pResponse["sToken"] = connINfo.sToken;
                RetJsonMsg::SetRetMsg(0,pResponse);
            }else{
                RetJsonMsg::SetRetMsg(-5,pResponse);
            }
            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
        }
        else{
            RetJsonMsg::SetRetMsg(-15,pResponse);
            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
        }

    }else {
        //POST参数错误
        RetJsonMsg::SetRetMsg(-15,pResponse);
        try {
            WsSendCOM wsSendCOM;
            wsSendCOM.WsSendByServer(pServer,pHdl,pResponse.toStyledString());
        } catch (const websocketpp::exception& e) {
            appendlog(TTDLogger::LOG_ERROR,"WsServerBLL::OnMessage WsSendByServer err:%s", e.what());
        }
        return;

    }


}

void WsServerBLL::Close(server* pServer,connection_hdl hdl)
{
    ConnKeepLiveBLL::GetInstance()->del(hdl);
    pServer->close(hdl,0,"");
}
