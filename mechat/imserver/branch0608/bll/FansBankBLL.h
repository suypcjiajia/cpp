#ifndef FANSBANKBLL_H
#define FANSBANKBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"

#include "TConvert.h"
#include "dal/UserDataDAL.h"
#include "dal/FansBankKValueDAL.h"
#include "dal/FansBankUserValueDAL.h"
#include "dal/FansBankOrderDAL.h"
#include "dal/FansBankFansmoveDAL.h"
#include "dal/FansbankKeyvaleDAL.h"
#include "dal/FansbankCountDAL.h"
#include "dal/FenZhangDAL.h"
using namespace std;


//分润列表 enum start
enum AccountTypeD{
     AccountTypeD_IN =0,//收入0
    AccountTypeD_OUT  =1 //支出1
};
enum AccountTypeT{
    AccountTypeT_TAX = 10,//10税费
    AccountTypeT_SALE = 18,//18销售
    AccountTypeT_GIVECOIN = 9, //送积分9
    AccountTypeT_SALESER = 24 //销售服务
};
enum AccountTypeC{//币种:余额0,绑定余额1,粉丝余额6,通用积分3,乐兑积分4
    AccountTypeC_YUE = 0,
    AccountTypeC_BANDING = 1,
    AccountTypeC_FANSYUE = 6,
    AccountTypeC_TONYONCOIN = 3,
    AccountTypeC_LEDUICOIN = 4,
    AccountTypeC_TONORLE = 8,  //通用积分或乐兑积分8
    AccountTypeC_FANYUORBINDYUORYU = 9//粉丝余额或绑定余额或余额9
};
//分润列表 enum end

//hongkong币种
enum HongkongCurrency{
    HKC_FANSXIAN = 0,//粉丝现金
    HKC_FANSYU = 1,//粉丝余额
    HKC_FANCOIN = 2 //积分

};
//hongkong Type
enum HongkongType{
    HKT_FUWUFEI = 4,//手续费
    HKT_GIVECOIN = 6//送积分
};

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装业务接口
 *Others:
*/
class FansBankBLL
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
    Description:获取粉丝银行
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetBankInfo( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取行情中的我的
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMarketOnSalling ( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取行情中的K线图
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMarketK ( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取详情中的成交记录
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetTradeById( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:获取所有卖出记录
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetAllTrade( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取全部行情（按分页）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetPageMarket( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取市值
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetMarketValue( const  Json::Value  & pJson  ,Json::Value & pResponse);





    /*
    Description:卖出操作
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Sell( const  Json::Value  & pJson  ,Json::Value & pResponse);



    /*
    Description:搜索行情
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SearchMarket( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:搜索
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Search( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:获取交易中的可买入列表
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetSelling( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:搜索交易中的可买入列表
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SearchSelling( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:买入操作
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int Buy( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:报单购买
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int InnerBuy( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:报单下单
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int InnerCreate( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:赠送
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int FansGive( const  Json::Value  & pJson  ,Json::Value & pResponse);



    /*
    Description:分页获取订单
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetPageOrder( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:分页获取DasaiBaodan订单
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetDasaiBaodanOrder( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:分页获取订单(后台管理)
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetOrderByManage( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:取消订单
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int CancelOrder( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:红点提示
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int RedPoin( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int GetConfig( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int SetConfig( const  Json::Value  & pJson  ,Json::Value & pResponse);

    /*
    Description:
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int OrderState( const  Json::Value  & pJson  ,Json::Value & pResponse);


    /*
    Description:增加均价（可以是负数，表示减少）
    Input:  @pJson Post请求数据
    Output: @pResponse 处理结果
    Return: 0 -success
    */
    int ChangePrice( const  Json::Value  & pJson  ,Json::Value & pResponse);



private:
    string GetShopName(long Id);
    int GetShopEntity(long Id,Json::Value & jRes);
    string GetShopPhone(long Id);
    long GetShopManId(long Id);
    string GetPhoneByUserId(long Id);

    string MakeAnOrderNum(long Id);

    //得到可以挂卖的手数
    int GetCanSellLots(long Id, int iType);


    //支付，调用桌斌
    int Pay(string sOrderNo,string sPayPhone, string sMerPhone, string sDetail,string sPayPassWord,
            int iPayType, double dMoney,double dShopCoin, Json::Value & jResponse);
public:
    //支付，调用yu
   // int Payv2(string sOrderNo,string sPayPhone, string sMerPhone, string sDetail,string sPayPassWord,
   //                       double dMoney,double dShopCoin, Json::Value & jResponse);

    //支付，调用yu
    int Payv3(string orderNo,string payPhone, string recPhone, string remark,string payPassWord,
                          double payAmount,int payCurrency,double payAmountB,int payCurrencyB,
              Json::Value & jResponse);

    //支付，调用yu
    int Payv4(string orderNo,string payPassWord,Json::Value & jResponse);

    //支付，调用Hongkong
    int PayByHongkong(string orderNo,string payPassWord,Json::Value & jResponse);

    //获取粉丝余额信息，调用yu
    int BalanceGetModel(string phone,int currency,Json::Value & jResponse);

    //创建订单(生成分润列表)，调用yu
    int WalletCreateOrder(string orderNo,string payPhone, long payId,string recPhone,long recId, string remark,
                                       double payAmount,int payCurrency,double payAmountB,int payCurrencyB,string scene,
                                       Json::Value & jResponse);

    //创建订单(生成分润列表)，调用hongkongfans
    int CreateOrderHongkong(string orderNo,string payPhone,long payId, string recPhone,long recId, string remark,
                                       double payAmount,int payCurrency,double payAmountB,int payCurrencyB,string scene,
                                       Json::Value & jResponse);

    //检验支付密码,调用桌斌
    int CheckPayPassWord( string sPhone , string sPassWord,Json::Value & jResponse);

    //获取推广收益,调用桌斌
    int GetPushIncome( string sPhone ,Json::Value & jResponse);

    //获取PlatformId推广收益,调用桌斌
    int GetPlatformIdIncome( long lPlatformId ,Json::Value & jResponse);

    void Replace(Json::Value & jsonRespons);
    void Replacev2(Json::Value & jsonRespons);

    //如果不存在某天的K柱，则补充
    void MakeK(long Id,int iType, const string & sDay);

    void Make20K(long Id,int iType);

    //获取一批要卖的粉丝(按粉丝价值排序后分段，从每段抽出粉丝)
    int GetSellUserString(string sSellPhone,long IdSell, int iType, int iNum,string & sSellUser);
    //获取一批要卖的粉丝(高价值的粉丝先被卖掉)
    int GetSellUserString3(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser);
    //获取一批要卖的粉丝(只有线下消费过的)
    int GetSellUserString2(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser);
public:
    //获取一批要卖的粉丝(随机)
    int GetSellUserString4(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser);
    //获取一批要卖的粉丝
    int GetSellUserString5(string sSellPhone,long IdSell, int iType, int iNum ,string & sSellUser);
    double CaluSellAvg(UserValueLst & lows);
    void Append(UserValueLst & lows, UserValueLst &  hights);

    double GetRandByPrice(double dPrice);
    void GetRandByPrice(double dPrice, double & dOpen, double & dClose, double & dHight, double & dLow);

    void GetTodayYesterdayKvalue(long Id, int iType,KValueEntity & kvalue, KValueEntity & ykvalue, double & dUpPrice, double & dUplist);


    //更新实时价格 最高价 最低价 收盘价
    void UpdatePrice(long Id,int iType);


    //是否vip[报单员]
    bool IsBadodanMember(long lUserId);
    //是否是否大赛会员
    bool IsDaSaiMember(long lUserId);
    //孙伟那边的交易支付
    int BaodanPay(string orderNo,long buy, long sell, int type,double amount,Json::Value & jResponse);


private:    

    UserDataDAL mUserDataDAL;
    FansBankKValueDAL mKValueDAL;
    FansBankUserValueDAL mUserValueDAL;
    FansBankOrderDAL mFansBankOrderDAL;
    FansBankFansmoveDAL mFansBankFansmoveDAL;
    FansBankKeyvalueDAL mFansBankKeyvalueDAL;
    FansBankCountDAL mFansBankCountDAL;
    FenZhangDAL mFenZhangDAL;

    TConvert mtConvert;

    //服务费persent
    const double COUNTER_FEE_PROP = 0.10;
    const double TAXATION_PROP = 0.006;

    //hong kong
    const string mHongkongAPP_SECRET = "d6fbf5ae32c61e97";
    //hong kong
    const string mHongkongAppKey = "a00d6fbf5ae32c61e97174f6456e61c7f5f0338d";

    //全球粉丝贸易管理中心有限公司B平台服务费 phone
    const string mHongkongFuwuFeiPhone = "11111111245";
    //全球粉丝贸易管理中心有限公司B平台服务费 id
    const long mHongkongFuwuFeiId = 190;

};

#endif // FANSBANKBLL_H
