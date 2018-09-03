#ifndef USERVIDEOENTITY_H
#define USERVIDEOENTITY_H

#include <string>
#include <vector>
using namespace std;

struct UserVideoEntity
{
    long addTime;          //添加时间
    int type;
    string url;
    string title;
    string icon;
    string content;
};

typedef vector<UserVideoEntity> UserVideoLst;

//0普通用户 1联盟商家 2供应商 3个代 4区代 5fansbank
enum enum_user_role
{
    enum_user_role_normal = 0,
    enum_user_role_nearshop =1,
    enum_user_role_gongying = 2,
    enum_user_role_peragent =3,
    enum_user_role_zoneagent = 4,
    enum_user_role_fansbank = 5
};

#endif // USERVIDEOENTITY_H
