#ifndef PHONE_CODE_BLL_H
#define PHONE_CODE_BLL_H
#include <string>
#include"json/json.h"
#include "TMutex.hpp"
using namespace std;

class PhoneCodeBLL
{
public:
    //EvhtpSvr服务接口
    int Process(const string& psMethod,const Json::Value& pJson, Json::Value& pResponse);

    //使用手机验证码
    int UseCode(const Json::Value& pJson, Json::Value& pResponse);

    //发送手机-TTD
    int CodeTTD(const Json::Value& pJson, Json::Value& pResponse);
    //增强版发送短信-TTD
    int CodeTTDv2(const Json::Value& pJson, Json::Value& pResponse);

    //TTD秒嘀云 语音验证码
    int SendTTDYuyingToMiaoDiYun(const string& psPhone,string & err);



    int QuerySms();


private:
    //生成指定位数的随机数,产生区间[a,b]上的随机数
    int Random( int a, int b  );

    //TTD阿里大鱼短信（阿里官方短信服务商）
    int SendTTDMsgToAliyuncsSMS(const string& psPhone,const string psType, const string& psMsg,string & err);
    //TTD秒嘀
    int SendTTDMsgToMiaoDiYun(const string& psPhone, const string psType,const string& psMsg,string & err);



    int Send(string sPhone,string sType,string sMsg,Json::Value& pResponse,string & err);


private:
    TMutex mMutex;//线程锁

};

#endif // PHONE_CODE_BLL_H
