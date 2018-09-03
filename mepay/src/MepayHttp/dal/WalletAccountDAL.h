//#ifndef WALLETACCOUNT_DAL_H
//#define WALLETACCOUNT_DAL_H

//#include <string>
//#include <vector>
//#include <deque>
//#include <hash_map>
//#include "TMutex.hpp"
//#include "dal/TMysqlDAL.h"
//#include "bll/WalletAccountBLL.h"
//#include "dal/MysqlConnect.h"
//using namespace std;

//class WalletAccountDAL
//{
//    // 定义单键
//public:  static WalletAccountDAL* GetInstance();
//private: static WalletAccountDAL* mInstance;
//private: TMutex mMutex;//线程锁
//public:
//    //创建表
//    int CreateTable();

//    //现金充值
//    int InCash( const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum);
//    //绑定现金充值
//    int InBandCash(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum);
//    //购物币充值
//    int InShopTicket(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum);
//    //现金提现
//    int OutCash(const long & lUserId,const long & lValue, const string & sCard,const string & sOrderNum);
//    //现金提现退回
//    int BackCash(const long & lUserId, const string & sRemark,const string & sOrderNum);
//    //第三方支付
//    int ThirdPartyPay(const long & lUserId,const long & lValue, const long & lPlatformId,const string & sRemark,const string & sOrderNum);
//    // 钱包支付
//    int Pay(const long & lUserId,const long & lValue, const long & lValue2, const int & iPayType,
//            const long & lPlatformId,const string & sRemark,const string & sOrderNum,const int & iDiveied);
//    //退款
//    int BackPay(const long & lUserId,const string & sRemark,const string & sOrderNum);
//    //确认支付(简易版分成)
//    int OkPay(const long & lUserId,const string & sOrderNum, const long & lCost);
//    //个代付费
//    int PerAgentPay(const long & lUserId, const long & lValue, const int & iPayType, const string & sOrderNum, const string & sCity, const long & lPushId);

//    //全民推广收益
//    int AllPush( const long & plPushId, const long & plPlatformId,const string & psOrderNum);
//    //增加
//    int Insert( const WalletAccountEntity& entity );
//    //删除
//    int Delete( const string& psPrimaryKey, const long lUptTime,const long lUpdateId );
//    //修改
//    int Update( const string& psPrimaryKey,const WalletAccountEntity& entity );
//    int ModifyPwd(const string & psPrimaryKey,const string & psNewPwd,const long & plUpdateId);
//    //获取记录
//    int GetList(const WalletAccountQueryEntity & pQueryEntity, int piIndex, int piCount,WalletAccountList& plstOut, int & piTotalCount);
//    //获取单个
//    int GetEntityByUseId(const long & plUserId ,WalletAccountEntity & pOut);
//    int GetEntityByKey(const string & pKey, WalletAccountEntity & pOut);
//    int GetEntity(const string & pCond,int piType, WalletAccountEntity & pOut);
//    //更换手机号
//    int UpdatePhone(const string & psPhone, const string & psNewPhone);
//private:
//    //构建插入流水表语句
//    string MakeSqlInsertDayBook(const long & lUserId,const long & lValue,const int & iDiscount,const string & sOrderNum,const int & iDir,
//                                const int & iType,const int & iStep,const long & lPlatformId,const string & sQRcode,const string & sRemark,const string & sBusinessRemark);
//    //事务处理（只能是增册改的操作）
//    int Transaction(TMultiMysqlDAL * con,const vector<string>  & pQueryLst);

//    int GetQueryTotalCount(const string & psQuery, int & piCount);

//};

//#endif // WALLETACCOUNT_DAL_H
