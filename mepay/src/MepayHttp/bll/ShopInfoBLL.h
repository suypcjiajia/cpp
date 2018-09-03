#ifndef SHOPINFO_BLL_H
#define SHOPINFO_BLL_H
#include "basebll.h"
#include <string>
#include <list>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "define.h"
#include"json/json.h"
#include "dal/BankDAL.h"
#include "dal/ShopSettingDAL.h"
#include "dal/AdvDAL.h"
#include "dal/CommentDAL.h"
#include "dal/ShowDAL.h"
#include "dal/KeySecretDAL.h"
#include "dal/SubjectDAL.h"
#include "dal/ClassDAL.h"
#include "com/Request.h"

#include <tr1/unordered_map>
#include "TypeDef.h"

using namespace std;
//主键-哈唏表
typedef std::tr1::unordered_map<long,string> UserIdPhoneMap;

using namespace std;


class ShopInfoBLL : public BaseBLL
{


public:  static ShopInfoBLL* GetInstance();
private: static ShopInfoBLL* mInstance;
private: TMutex mMutex;//线程锁
public:

     int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
     //处理带token，不加密的情况
     int Process2(EvhtpSvr * pEvhtp,const  Json::Value  & pJson, Json::Value & pResponse);

    //初始化
    int Init();



    int Delete( const  Json::Value  & pJson ,Json::Value & pResponse );

    //更新商家基本资料
    int Update( const  Json::Value  & pJson ,Json::Value & pResponse );
    //更新商家基本资料,处理带token，不加密的情况
    int Update2(long lPlatformId, const  Json::Value  & pJson ,Json::Value & pResponse  );


    //查询数据-平台号ID或帐号或用户ID(APP,管理后台)
    int GetEntity( const  Json::Value  & pJson ,Json::Value & pResponse );
    //查询数据-平台号ID或帐号或用户ID,(处理带token，不加密的情况)
    int GetEntity2( long lPlatformId ,const  Json::Value  & pJson ,Json::Value & pResponse);
    //查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)(管理后台)
     int GetData(const  Json::Value  & pJson ,Json::Value & pResponse);
     //查询数据-條件查詢（piCount==0时不分批)(管理后台)(处理带token，不加密的情况)
      int GetData2(const  Json::Value  & pJson ,Json::Value & pResponse);
     //查询数据（APP调用）
     int GetPage(const  Json::Value  & pJson ,Json::Value & pResponse);

     //获取附近联盟商家(APP调用)
     int GetNearBy(const  Json::Value  & pJson ,Json::Value & pResponse);

     //获取推荐的商家(推荐的商家优先显示，接下来是符合所在地址并按距离排序显示)(APP调用)
     int RecommendShops( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取热门商家(APP调用)
     int HotShops( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取乐兑商品（分页）(APP调用)
     int Products( const  Json::Value  & pJson ,Json::Value & pResponse );


     //获取联盟商家商品（分页）(APP调用)
     int ShowProduct( const  Json::Value  & pJson ,Json::Value & pResponse );
     //获取联盟商家商品（分页）处理带token，不加密的情况
     int ShowProduct2( const  Json::Value  & pJson ,Json::Value & pResponse );
     //获取联盟商家商品（分页） 处理带token，不加密的情况
     int ShowProduct2(long lPlatformId, int iPage,Json::Value & pResponse );

     //获取我的平台列表
     int GetMyShop( const  Json::Value  & pJson ,Json::Value & pResponse );
     //商家资料完整判断
     int ShopPerfect(const  Json::Value  & pJson ,Json::Value & pResponse);

     //修改密码
     int ModifyPwd(const  Json::Value  & pJson ,Json::Value & pResponse);
     //修改密码,处理带token，不加密的情况
     int ModifyPwd2(long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse);
     //重置密码(没有验证码)
     int ResetPwd( const  Json::Value  & pJson ,Json::Value & pResponse);
     //使用验证码重置密码
     int Reset( const  Json::Value  & pJson ,Json::Value & pResponse);

     //申请变更折扣
     int ApplyDiscount( const  Json::Value  & pJson ,Json::Value & pResponse );
     //申请变更折扣,处理带token，不加密的情况
     int ApplyDiscount2( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse );

     //通过变更申请
     int ChangeDiscount( const  Json::Value  & pJson ,Json::Value & pResponse  );

     //创建平台
     int CreateShop(const  Json::Value  & pJson ,Json::Value & pResponse);

     //开店（APP调用）
     int CreateShopApp( const  Json::Value  & pJson ,Json::Value & pResponse);

     //登陆
     int Login(const  Json::Value  & pJson ,Json::Value & pResponse);

     //获取个代开的店
     int ShopsByPerAgent(const  Json::Value  & pJson ,Json::Value & pResponse);


     //归档
     int Doc( const  Json::Value  & pJson ,Json::Value & pResponse );


     int AddBank( const  Json::Value  & pJson ,Json::Value & pResponse );

     int GetBank( const  Json::Value  & pJson ,Json::Value & pResponse );

     //修改开关配置
     int Setting( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取开关配置
     int GetSetting( const  Json::Value  & pJson ,Json::Value & pResponse );

     int ModifyPhone( const  Json::Value  & pJson ,Json::Value & pResponse );

     int GetState( const  Json::Value  & pJson ,Json::Value & pResponse );

     int CashBackShops( const  Json::Value  & pJson ,Json::Value & pResponse );



     int GetLotsPlatfrom( const  Json::Value  & pJson ,Json::Value & pResponse );

     //add commtent
     int WriteCommtent( const  Json::Value  & pJson ,Json::Value & pResponse );
     //add commtent 处理带token，不加密的情况
     int WriteCommtent2( const  Json::Value  & pJson ,Json::Value & pResponse );
     //超管写评论 处理带token，不加密的情况
     int WriteCommtentBySuper2( const  Json::Value  & pJson ,Json::Value & pResponse );
     //get commtent
     int GetCommtent( const  Json::Value  & pJson ,Json::Value & pResponse );
     //get commtent 处理带token，不加密的情况
     int GetCommtent2( long lPlatformId,long lAddTime,int iCount ,Json::Value & pResponse );

     //添加广告
     int AddAdv( const  Json::Value  & pJson ,Json::Value & pResponse );
     //删除广告
     int DelAdv( const  Json::Value  & pJson ,Json::Value & pResponse );
     //更改广告审核状态
     int AdvVerify( const  Json::Value  & pJson ,Json::Value & pResponse );
     //查询广告
     int GetAdv( const  Json::Value  & pJson ,Json::Value & pResponse );


     //添加产品
     int AddProduct( const  Json::Value  & pJson ,Json::Value & pResponse );
     //添加产品 处理带token，不加密的情况
     int AddProduct2( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse );
     //删除产品
     int DelProduct( const  Json::Value  & pJson ,Json::Value & pResponse );
     //更改产品审核状态
     int ProductVerify( const  Json::Value  & pJson ,Json::Value & pResponse );
     //获取展示产品
     int GetProduct( const  Json::Value  & pJson ,Json::Value & pResponse );
     //获取展示产品 处理带token，不加密的情况
     int GetProduct2( const  Json::Value  & pJson ,Json::Value & pResponse );
     //获取展示产品 处理带token，不加密的情况
     int GetProductByToken( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse );

     //修改展示产品
     int UpProduct( const  Json::Value  & pJson ,Json::Value & pResponse );
     //修改展示产品
     int UpProduct2( long lPlatformId,const  Json::Value  & pJson ,Json::Value & pResponse );

     //检查查看密码
     int Check( const  Json::Value  & pJson ,Json::Value & pResponse );
     //Modify查看密码
     int ModifyCheck( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取收款记录,处理带token，不加密的情况
     int GetTrade2(const string & sPlatformId, const string & sStartTime, const string & sEndTime
                   ,const string & sPayerPhone,const string & sPage,Json::Value & pResponse);

     //设置APP secret
     int SetSecret( const  Json::Value  & pJson ,Json::Value & pResponse );

     //验证key和secret，然后返回平台号
     int GetIdBySecret( const  Json::Value  & pJson ,Json::Value & pResponse );


     //某个时间点之前各个区的开通联盟商家数（0个的不算）
     int CityShopCount( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取专题列表
     int GetObjects( const  Json::Value  & pJson ,Json::Value & pResponse );


     //获取专题店铺
     int GetShopsByObjest( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取快速评论
     int GetQuikMark( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取联盟商家分类
     int GetClass( const  Json::Value  & pJson ,Json::Value & pResponse );


     //获取个代开的供应商
     int OpenbyPerAgent( const  Json::Value  & pJson ,Json::Value & pResponse );

     //特殊促销配置
     int PromotionConfig( const  Json::Value  & pJson ,Json::Value & pResponse );

     //根据平台号获取代理信息
     int GetAgentInfoByPlatformId( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取属于我商店(收款记录)
     int MyShop( const  Json::Value  & pJson ,Json::Value & pResponse );

     //获取属于我授权商店(收款记录)
     int MyImpowerShop( const  Json::Value  & pJson ,Json::Value & pResponse );

public:
     //商家资料完整判断
     bool IsShopPerfect(const ShopInfoEntity & entity);

private:
     /*计算两点距离
      * return : 两点距离(米)
      * */
      double GetDistance(double pdLat1,double pdLong1 , double pdLat2,double pdLong2);
      //排序
       static bool SortByDistance(const ShopInfoEntity & f , const ShopInfoEntity & s);
       static bool SortByAddTime(const ShopInfoEntity & f , const ShopInfoEntity & s);

       void DataLst2Json(  ShopInfoList & pLst,Json::Value & pResponse , int piFilt);
       //更改关注
       bool Attention(const long & plUserId, const long & plPlatformId);
       void SetDefault( ShopInfoEntity & entity);
       //发送商家开通通知
       bool SendPhoneNotifyCode(const string& psPhone, const string& psMsg,string sType);
       //验证短信验证码
       bool IsUserPhoneCode(const string& psPhone, const string& psCode);
       //发送商家开通通知,by Im
       bool SendNotify(long lToUserId, const string& sPlatformId, const string & sPwd);
       //发送联盟商家信息更新通知,by Im
       bool SendModifyShopNotice( long lPlatformId, const string & sShopName, const string & sShopAddr
                                               ,int iCoinType, int iDiscount);
       //通过手机获取用户信息
       int GetUserInfoByPhone(const string & psPhone, long & plUserId);
       //通过用户获取手机信息
       int GetUserInfoById(long  lUserId, string & sPhone);
       //是否正确的验证码
       int UserPhoneCode(const string & psPhone, const string & psCode);


       //从商家帐号提取拼音
       string GetPinFromAccount(string sAccount);

       //发起调用C#服务端
       int Requst(const string & sUrl, const Json::Value & jsonPost, Json::Value & jsonRespons);

       //获取商城产品
       int GetProduct(int iPage, long lUserId, string sSearch,Json::Value & jsonRespons);

       //得到赠送购物币比例
       int GetGiveCoin(int iDiscount);

       double GetAvgScore(long lPlatfromId,long lProductId);

       //long GetPlatformId(const string & sToken);

       //获取个代手机号
       bool GetPhone( const ShopInfoList & plsTmp);

       bool IsPic(string sAddr);

       void ArrayToString(const Json::Value & j,string & s);





private:
       BankDAL mBankDal;
       ShopSettingDAL mShopSettingDal;
       AdvDAL mAdvDAL;
       CommentDAL mCommentDAL;
       ShowDAL mShowDAL;
       KeySecretDAL mKeySecretDAL;
       SubjectDAL mSubjectDAL;
       ClassDAL mClassDAL;
       ShopSettingDAL mShopSettingDAL;
       const string sAesKey = "!@#$%^&*()QWERTY";//zhuo bin c#
       UserIdPhoneMap mUserIdPhoneMap;
       LongLongMap mSuperCommentTime;//记录超管评论，每一个平台号最后的评论时间


};

#endif // SHOPINFO_BLL_H
