//#ifndef WALLETACCOUNT_BLL_H
//#define WALLETACCOUNT_BLL_H
//#include "basebll.h"
//#include <string>
//#include "TMutex.hpp"
//#include <tr1/unordered_map>
//#include "define.h"
//#include"json/json.h"
//using namespace std;
////主键-哈唏表
//typedef std::tr1::unordered_map<string,WalletAccountEntity> WalletAccountHashMap;

//class WalletAccountBLL : public BaseBLL
//{
//public:  static WalletAccountBLL* GetInstance();
//private: static WalletAccountBLL* mInstance;
//private: TMutex mMutex;//线程锁
//public:
//    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
//    //初始化
//    int Init();

//    //现金充值
//    int InCash(const  Json::Value  & pJson, Json::Value & pResponse);
//    //绑定现金充值
//    int InBandCash(const  Json::Value  & pJson, Json::Value & pResponse);
//    //购物币充值
//    int InShopTicket(const  Json::Value  & pJson, Json::Value & pResponse);
//    //现金提现
//    int OutCash(const  Json::Value  & pJson, Json::Value & pResponse);
//    //现金提现退回
//    int BackCash(const  Json::Value  & pJson, Json::Value & pResponse);

//    //第三方支付
//    int ThirdPartyPay(const  Json::Value  & pJson, Json::Value & pResponse);
//    // 钱包支付
//    int Pay(const  Json::Value  & pJson, Json::Value & pResponse);
//    // 退款
//    int BackPay(const  Json::Value  & pJson, Json::Value & pResponse);
//    // 确认支付
//    int OkPay(const  Json::Value  & pJson, Json::Value & pResponse);
//    //个代付费
//    int PerAgentPay(const  Json::Value  & pJson, Json::Value & pResponse);

//    //增加
//    int Insert( const  Json::Value  & pJson, Json::Value & pResponse);
//    //删除
//    int Delete( const  Json::Value  & pJson  ,Json::Value & pResponse );
//    //修改
//    int Update( const  Json::Value  & pJson  ,Json::Value & pResponse );
//    //查询一行
//    int GetEntity(const  Json::Value  & pJson  ,Json::Value & pResponse);
//    //查询数据-條件查詢，为控制流量，设为分批返回（piCount==0时不分批)
//    int GetData(const  Json::Value  & pJson  ,Json::Value & pResponse );
//    int ModifyPwd(const  Json::Value  & pJson  ,Json::Value & pResponse);
//    //登陆
//    int Login(const  Json::Value  & pJson  ,Json::Value & pResponse);
//    //重置密码
//    int ResetPwd(const  Json::Value  & pJson  ,Json::Value & pResponse);

//private:
//    //全民推广收益
//    int AllPush( const long & plPushId, const long & plPlatformId);
//    void SetDefault(WalletAccountEntity & entity);
//};

//#endif // WALLETACCOUNT_BLL_H
