#ifndef PUSHUSEREntity_H
#define PUSHUSEREntity_H

#include <string>
using namespace std;
//推送机用户信息
struct PushUserEntity
{
    long lUserId;           //用户ID
    long lAddTime;          //创建时间
    string sPushIp;         //推送机IP
    int iPhoneType;         //手机类型,1:苹果,2:安卓
    string sToken;          //苹果或安卓Token
};

#endif // PUSHUSEREntity_H
