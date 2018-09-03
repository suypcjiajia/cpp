#ifndef CONNKEEPLIVEBLL_H
#define CONNKEEPLIVEBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include "bll/WsServerBLL.h"
#include <tr1/unordered_map>
#include"json/json.h"
using namespace std;

/*
 * websocket连接心跳探测
策列：对于超时的连接，关闭
 */



struct ConnKeepLiveEntity
{
    server* service;               //WS对像
    long lLastTime;
    connection_hdl hdl;
};

typedef std::tr1::unordered_map<void*,ConnKeepLiveEntity> ConnKeepLiveHashMap;

class ConnKeepLiveBLL
{
public:
    static ConnKeepLiveBLL* GetInstance();

public:
    //刷新链接
    int Replace( const connection_hdl & hdl, server * sevice);
    //删除链接
    int del( const connection_hdl & hdl );

    void Start();
    static void* PingThread(void*ptr);
    void Ping();
    bool GetOneHdl( ConnKeepLiveEntity & entity);

private:
    ~ConnKeepLiveBLL(){}

private:
    static ConnKeepLiveBLL* mInstance;
    TMutex mMutex;                  //线程锁

    ConnKeepLiveHashMap mConnKeepLiveHash;

};

#endif // CONNKEEPLIVEBLL_H
