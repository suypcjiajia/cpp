#ifndef COUPON_BLL_H
#define COUPON_BLL_H
#include "basebll.h"
#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "define.h"
#include"json/json.h"
#include "TConvert.h"
#include "dal/CouponCodeDAL.h"
#include "dal/CouponDAL.h"
#include "dal/ShowDAL.h"
#include "dal/CommentDAL.h"
#include "dal/ClassDAL.h"
#include "TypeDef.h"

using namespace std;

class CouponBLL : public BaseBLL
{
public:  static CouponBLL* GetInstance();
private: static CouponBLL* mInstance;
private: TMutex mMutex;//线程锁
public:
    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
    //处理带token，不加密的情况
    int Process2(EvhtpSvr * pEvhtp, const  Json::Value  & pJson, Json::Value & pResponse);
    //初始化
    int Init();

    //获取我的券
    int GetMyCoupon(const  Json::Value  & pJson,
                              Json::Value & pResponse);

    //下单
    int Order(const  Json::Value  & pJson,
            Json::Value & pResponse);


    //获取一张券的详情
    int Detail(const  Json::Value  & pJson,
            Json::Value & pResponse);

    //超时信息
    int IsTimeOut(const  Json::Value  & pJson,
            Json::Value & pResponse);

    //产品是否过期
    int ProductTimeOut(const  Json::Value  & pJson,
            Json::Value & pResponse);


    //申请退款
    int ReqBack(const  Json::Value  & pJson,
            Json::Value & pResponse);


    //cancel退款
    int CancelBack(const  Json::Value  & pJson,
            Json::Value & pResponse);

    //add commtent
    int WriteCommtent( const  Json::Value  & pJson ,Json::Value & pResponse );


    //获取产品详情
    int ProductDetail( const  Json::Value  & pJson ,Json::Value & pResponse );


    //验证使用验证码
    int Check( const  Json::Value  & pJson ,Json::Value & pResponse );


    //同意退款
    int Agree( const  Json::Value  & pJson ,Json::Value & pResponse );


    //拒绝退款
    int RejBack( const  Json::Value  & pJson ,Json::Value & pResponse );


    //获取一个平台下的券
    int GetCouponByPlatfrom( const  Json::Value  & pJson ,Json::Value & pResponse );

    //首页商品轮播
    int Loop( const  Json::Value  & pJson ,Json::Value & pResponse );

    //商品分类数量
    int TypeInfo( const  Json::Value  & pJson ,Json::Value & pResponse );

    //商品分区分类分页
    int GetPageByType( const  Json::Value  & pJson ,Json::Value & pResponse );


public:
    //检验支付密码,调用桌斌
    int CheckPayPassWord( string sPhone , string sPassWord,Json::Value & jResponse);

    //批量创建订单
    int WalletOrder(const StringLst & sOrderNos,double dPrice,double dCoin,
                    int iPayType,long lPlatformId,const string & sPayerPhone,  Json::Value & jRespons);
    //支付
    int WalletPay(string sPayOrderNo,string sPassWord,int iCoinType,Json::Value & jRespons);
    //结算
    int WalletSettlement(string sOrderNo,Json::Value & jRespons);
    //退款
    int WalletBack(const  Json::Value & jCode,string sPassWord,Json::Value & jRespons);

private:

    int GetShopType(const string & sShopType);

    //是否超过直接退款的时间
    bool IsAutoTimeOut(const Coupon & entity);


    //取用户信息
    int GetUserInfo(long lUserId,string & sPhone);

    void Replace(Json::Value & jsonRespons);

    //发送验证成功通知,by Im
    bool SendCheckNotice( long lUserId, const string & sConntent, const string & sCode, int iCoin, int iCoinType);

    void AppendShopType(StringLongStdMap & typenum);




    TConvert tConvert;

    CouponOrderDAL mCouponOrderDAL;
    CouponCodeDAL mCouponCodeDAL;
    ShowDAL mShowDAL;
    CommentDAL mCommentDAL;
    ClassDAL mClassDAL;

};

#endif // COUPON_BLL_H
