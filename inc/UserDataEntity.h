#ifndef UserDataEntity_H
#define UserDataEntity_H

#include <string>
#include <vector>
using namespace std;


//用户状态 0-正常  1-封死  2-pass 3-禁止交易 333-逻辑删除
enum enum_user_state
{
    enum_user_state_normal = 0,
    enum_user_state_colse = 1,
    enum_user_state_pass = 2,
    enum_user_state_disablefans = 3,
    enum_user_state_del = 333
};

struct UserDataEntity
{
    UserDataEntity(){
        sAccount = "-1nochange";
        sPassword = "-1nochange";
        sNickName = "-1nochange";
        iSex = -1;
        sProvince = "-1nochange";
        sCity = "-1nochange";
        sSignature = "-1nochange";
        sEmail = "-1nochange";
        sHeadImage = "-1nochange";
        iPhoneAuth = -1;
        lPushManId = -1;
        lPlatformId = -1;
        lCurrentPlatformId = -1;
        sGciID = "-1nochange";
        sGciPwd = "-1nochange";
        sGciPhone = "-1nochange";
        sBirthDay = "-1nochange";
        iHot = 0;
    }
    string sPrimaryKey;     //主键
    long lUserId;           //用户ID（唯一、递增）
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iState;             //状态  0默认 333删除
    long lUpdateId;         //更新ID
    int iSync;              //

    string sAccount;        //帐号
    string sPassword;       //密码
    string sNickName;       //昵称
    int iSex;               //性别 0女 1男
    string sProvince;       //省份
    string sCity;           //城市
    string sSignature;      //个性签名
    string sEmail;          //邮箱地址
    string sHeadImage;      //头像路径
    int iPhoneAuth;         //手机认证 0未认证 1已认证
    long lPushManId;        //推广人ID
    long lPlatformId;       //注册关注平台号
    long lCurrentPlatformId;//当前关注平台号
    string sGciID;          //GCI会员ID
    string sGciPwd;         //GCI会员密码
    string sGciPhone;       //GCI会员手机号
    string sQqNum;
    string sWeixinNum;
    string sAliNum;
    string sBirthDay;
    int iHot;
};
typedef vector<UserDataEntity> UserDataList;

struct UserGradeEntity
{
    long lUserId;
    int iLevel;
    long lAddTime;
};
typedef vector<UserGradeEntity> UserGradeList;

#endif // UserDataEntity_H
