#ifndef UserDataBLL_H
#define UserDataBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "TypeDef.h"
#include "UserVideoEntity.h"
#include "dal/UserDataDAL.h"
#include "dal/UserGradeDAL.h"
#include "dal/UserSignDAL.h"
#include "dal/UserVideoDAL.h"
#include "dal/FansBankUserValueDAL.h"
#include "dal/FansBankKValueDAL.h"
#include "dal/ThirdUserDAL.h"
#include "dal/UserDeviceDAL.h"

using namespace std;


/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户业务接口
 *Others:
*/
class UserDataBLL
{
public:
    /*
    Description:初始化
    Input: null
    Output: null
    Return: 0 -success
    */
    int Init();
    /*
    Description:分配业务接口
    Input: @psMethod 方法名 @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse);



    /*
    Description:用户注册
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int AddData( const  Json::Value  & pJson ,Json::Value & pResponse);

    /*
    Description:通过绑定来注册
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int RegByBind(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:绑定手机号(手机号不存在，会先注册)
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int BindPhone(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:第三方用户注册
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ThirdUser(const  Json::Value  & pJson  ,Json::Value & pResponse);


    //删除
    //int Delete( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:修改普通资料
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Update( const  Json::Value  & pJson  ,Json::Value & pResponse);



    /*
    Description:忘记密码
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ForgetPassword( const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:修改密码
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ModifyPwd( const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:手机号是否被注册了
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int IsRegistered( const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:修改帐号(手机号)
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UpdateAccount( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:修改GCI会员
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int BandGciID( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:更改我的关注
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Attention( const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:平台粉丝
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int PlatformFans(const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:获取一批用户数据
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetUserInfoList( const  Json::Value  & pJson  ,Json::Value & pResponse );

    /*
    Description:
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Id2Phone( const  Json::Value  & pJson  ,Json::Value & pResponse );


    /*
    Description:用户登陆
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UserLogin( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:短信登录（如不存大帐号则自动注册）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int LoginByCode(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:CheckDeivce
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int CheckDeivce(const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:UpdateDeivce
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UpdateDeivce(const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:用户信息（主键或手机号或用户ID）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetUserInfo( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:根据用户获取我的推广者信息（手机号或用户ID）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyPushMan( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:根据用户获取我的推送人数（手机号或用户ID）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyPushCount( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:根据用户获取我的推送和登陆过人数（手机号或用户ID）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyPushAndLoginedCount( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:获取用户下两级推广者数量
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyFSLevelPushCount( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:绑定第三方账号
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int BandThirdId( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:解绑第三方账号
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int UnBandThird( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:第三方Id登录
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int LoginByThird( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:查询绑定信息
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int BindInfo( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取我开的店
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int MyShops( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:变更用户等级
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetGrade( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:分批获取会员
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetVips( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:查询用户的等级
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetGrade( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:判断用户密码
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Pass(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取注册量
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetRegisters(const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:获取每日待激活数量
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetUnhotCount(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:如果不存在用户，则自动注册
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Auto(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取N层上级推广者
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetPushUpChain(const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:每日签到
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Sign(const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取粉丝列表
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetFans( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:获取生产的粉丝列表
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMakedFans( const  Json::Value  & pJson  ,Json::Value & pResponse);

    //获取我的视频
    int GetVideo( const  Json::Value  & pJson  ,Json::Value & pResponse);

    //判断用户资料是否完整
    int Perfect( const  Json::Value  & pJson  ,Json::Value & pResponse );


    //解冻
    int Jiedong(const  Json::Value  & pJson  ,Json::Value & pResponse);

    //生成假粉丝
    int Make(const  Json::Value  & pJson  ,Json::Value & pResponse);

    //获取个代列表(粉丝信息)
    int PeragentLst(const  Json::Value  & pJson  ,Json::Value & pResponse);

    //获取联盟商家列表(粉丝信息)
    int ShopLst(const  Json::Value  & pJson  ,Json::Value & pResponse);

    //获取粉丝审核列表(某一天的)
    int GetUnHotInfoByDay(const  Json::Value  & pJson  ,Json::Value & pResponse);
    //获取粉丝审核列表(所有日期)
    int GetVerifyList(const  Json::Value  & pJson  ,Json::Value & pResponse);
    //审核更改状态
    int HotVerify(const  Json::Value  & pJson  ,Json::Value & pResponse);

    //粉丝回收
    int FansRecover(const  Json::Value  & pJson  ,Json::Value & pResponse);



private:
    int Make(UserDataEntity & entity);
public:
    UserDataDAL mUserDataDAL;
    UserGradeDAL mUserGradeDal;
    UserSignDAL mUserSignDAL;
    UserVideoDAL mUserVideoDAL;
    FansBankUserValueDAL mFansBankUserValueDAL;
    FansBankKValueDAL mFansBankKValueDAL;
    ThirdUserDAL mThirdUserDAL;
    UserDeviceDAL mUserDeviceDAL;
    //验证短信验证码
    int IsUserPhoneCode(const string& psPhone, const string& psCode,string & sMsg);
    /*注册送购物币
     *iActivityType 京东专题_注册送购物币 = 0 京东专题_分享送购物币 = 1 完善资料赠送购物币 = 2
    */
    bool GiveActivityReward(const string& psPhone,int iActivityType);
    //设置默认值
    //void SetDefaultValue( UserDataEntity& pEntity );
    //获取平台
    int GetPlatformByID(const string& psPlatformId, long & plUserId,int & iVerify);
    int GetPlatformState(const string& sPlatformId, int & iState);
    //是否个代
    int IsPersonAgent(const long & plUserId );
    void DataLst2Json(  UserDataList & pLst,Json::Value & pResponse );
    void Data2Json( const UserDataEntity & entity,Json::Value & pResponse);

    bool IsRightNick(const string & sNickName);

    //判断用户资料是否完整
    bool IsUserInfoPerfect(const UserDataEntity& entity);

    // 获取微信用户unionid信息
    string GetWeiXinUserInfo(string sOpenId,string sAccessToken);
    // 获取微信用户unionid信息
    string GetWeiXinUserInfo(string sOpenId,string sAccessToken,Json::Value & jReturn);
    // 获取alipay用户unionid信息
    string GetAlipayUserInfo(string sAccessToken);
    //注册钱包帐号
    int MoneyBagServiceAddByRegister(string phone,string name,Json::Value & jResponse);



};

#endif // UserDataBLL_H
