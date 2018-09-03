#ifndef ConnHashMapEntity_H
#define ConnHashMapEntity_H

#include <string>
using namespace std;

#include "bll/WsServerBLL.h"

//结构体定义
class UserWorkBLL;
typedef struct tsConnHashMapEntity
{
    long lUserId;                  //用户ID（主键）
    long lAddTime;                 //创建时间
    //string sMachineCode;         //APP安装时唯一码
    int iPhoneType;                //手机类型,1:苹果,2:安卓
    string sToken;                 //苹果或安卓Token
    server* mServer;               //WS对像
    connection_hdl mHdl;           //连接对像
    int iMsgCount;
    string sVer;
}ConnHashMapEntity;

#endif // ConnHashMapEntity_H
