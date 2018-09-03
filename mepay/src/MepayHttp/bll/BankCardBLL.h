#ifndef BANKCARD_BLL_H
#define BANKCARD_BLL_H
#include "basebll.h"
#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "define.h"
#include"json/json.h"
using namespace std;

class BankCardBLL : public BaseBLL
{
public:  static BankCardBLL* GetInstance();
private: static BankCardBLL* mInstance;
private: TMutex mMutex;//线程锁
public:
    int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse);
    //初始化
    int Init();
    //增加(绑定银行卡)
    int Insert( const  Json::Value  & pJson ,Json::Value & pResponse);
    //删除(解绑银行卡)
    int Delete( const  Json::Value  & pJson  ,Json::Value & pResponse);

    //获取单条银行卡
    int GetEntity(const  Json::Value  & pJson,
                  Json::Value & pResponse);

    //查询数据-條件查詢(获取我绑定的条银行卡 )
    int GetData(const  Json::Value  & pJson,
                              Json::Value & pResponse);
    void SetDefault( BankCardEntity &  entity);

    //银行卡4要素验证(apix平台)
    int Verify( const string & psBankCardNo, const string & psName, const string & psIdCardNo, const string & psPhone);

};

#endif // BANKCARD_BLL_H
