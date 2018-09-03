#ifndef WsServerBLL_H
#define WsServerBLL_H

#include"json/json.h"
#include "log/Logger.h"
#include "com/WsSendCOM.h"

class WsServerBLL {
public:
    WsServerBLL() {

    }

    void run(uint16_t pPort,int iThreads);

    void OnMessage(server* pServer, connection_hdl pHdl, const string& psMsg);

    //回调：打开链接
    void on_open(connection_hdl hdl);
    //回调:接收消息
    void on_message(connection_hdl hdl, server::message_ptr msg);
    //回调：关闭链接
    void on_close(websocketpp::connection_hdl);
    //回调：ping
    bool on_ping(connection_hdl hdl, string msg);
    //回调：pong
    void on_pong(connection_hdl hdl, string msg);

private:

    void Close(server* pServer,connection_hdl hdl);

    server mServer;


};

#endif // UserWorkBLL_H
