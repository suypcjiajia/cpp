#ifndef PHONE_CODE_INDEX_BLL_H
#define PHONE_CODE_INDEX_BLL_H

#include <string>
#include <list>
#include <map>
#include "TMutex.hpp"

using namespace std;

struct PhoneCodeState
{
   long lAddTime;//验证码生成时间
   int iState;//验证码状态 0-unuse 1-be used
};

typedef map<string,PhoneCodeState> MapPhoneCodeState;

class PhoneCodeMapBLL
{
public:  static PhoneCodeMapBLL* GetInstance();
private: static PhoneCodeMapBLL* mInstance;
private: TMutex mMutex;//线程锁
public:
    PhoneCodeMapBLL();
    static void * ThreadFun(void *p);
    //线程运行逻辑
    void RealRun();
    //设置
    int SetPhoneCodeList(const string& psPhone, const string& psCode);
    //是否存在
    bool IsdExist(const string& psPhone, const string& psCode);
    //Set beused
    int SetUsed( const string& psPhone, const string& psCode );
private:
    //验证码,手机列表   
    MapPhoneCodeState mPhoneCodes;
    void ClearTimeOut();

};

#endif // PHONE_CODE_INDEX_BLL_H
