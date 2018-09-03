#include "PhoneCodeBLL.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <sys/time.h>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "TMd5.h"
#include "bll/TBLL.h"
#include "PhoneCodeMapBLL.h"
#include "com/TCurl.h"
#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "Base64EnDecode.h"
#include "../inc/top-sdk-c/topsdk.h"
using namespace std;

//EvhtpSvr服务接口
int PhoneCodeBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
    int iRet = -1;

    if("UseCode" == psMethod ){
        //使用手机验证码
        iRet = UseCode(pJson,pResponse);
    }
    else if("CodeTTD" == psMethod ){
        //TTD send code
        iRet = CodeTTD(pJson,pResponse);
    }else if("CodeTTDv2" == psMethod ){
        //TTD send code
        iRet = CodeTTDv2(pJson,pResponse);
    }else{
        RetJsonMsg::SetRetMsg(-19,pResponse);
    }
    return iRet;
}

//使用手机验证码
int PhoneCodeBLL::UseCode(const  Json::Value  & pJson, Json::Value & pResponse)
{
    //合法性验证
    if( pJson["sPhone"].isNull() || pJson["sCode"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    string sPhone = tConvert.Json2String(pJson["sPhone"]);
    string sCode = tConvert.Json2String(pJson["sCode"]);

    appendlog(TTDLogger::LOG_DEBUG,"PhoneCodeBLL::UseCode sPhone=%s,Code=%s",sPhone.c_str(),sCode.c_str());

    int iRet = PhoneCodeMapBLL::GetInstance()->SetUsed(sPhone,sCode);
    if ( -1 == iRet ) {
        RetJsonMsg::SetRetMsg(-8,pResponse);
    } else if( -2 == iRet ){
        RetJsonMsg::SetRetMsg(-1,pResponse,"短信验证码已被使用");
    }else{
        RetJsonMsg::SetRetMsg(0,pResponse);
    }

    return 0;
}

//发送手机-TTD
int PhoneCodeBLL::CodeTTD(const Json::Value& pJson, Json::Value& pResponse)
{
    //合法性验证
    if( pJson["sPhone"].isNull() || pJson["sType"].isNull()|| pJson["sMsg"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iRet = -1;
    TConvert tConvert;
    string sPhone = tConvert.Json2String(pJson["sPhone"]);
    string sType = tConvert.Json2String(pJson["sType"]);
    string sMsg = tConvert.Json2String(pJson["sMsg"]);

//    if( sType == "1" || sType == "13"){
//        RetJsonMsg::SetRetMsg(-1,pResponse,"屏蔽发短信验证码");
//        return -1;
//    }

    string err;
    iRet = Send(sPhone,sType,sMsg,pResponse,err);
    RetJsonMsg::SetRetMsg(iRet,pResponse,err);
    return iRet;
}

//增强版发送短信-TTD
int PhoneCodeBLL::CodeTTDv2(const Json::Value& pJson, Json::Value& pResponse)
{
    if( pJson["sPhone"].isNull() || pJson["sType"].isNull()  || pJson["sCode"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    int iRet = -1;
    TConvert tConvert;
    string sPhone = tConvert.Json2String(pJson["sPhone"]);
    string sCode = tConvert.Json2String(pJson["sCode"]);
    string sType = tConvert.Json2String(pJson["sType"]);


    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    //http://appbs.tymshop.com:8080/appbs/newUserService/getImage/13411367357

    string sReturn;
    TCurl tCurl;
    string sUrl = "http://appbs.tymshop.com:8080/appbs/newUserService/checkCode/";
    sUrl += sPhone + "/" + sCode;
    iRet =tCurl.Get(sUrl,httpHeaderVector,sReturn);

    if( 0 == iRet ){
        Json::Value jsonRespons;
        tConvert.Str2Json(sReturn,jsonRespons);
        if( !jsonRespons["state"].isNull()){
            iRet = tConvert.Json2Int(jsonRespons["state"]);
            string result = tConvert.Json2String(jsonRespons["result"]);
            if(iRet == 0){

                string err;
                iRet = Send(sPhone,sType,"",pResponse,err);
                if( iRet == 0){
                    err = "短信验证码发送成功";
                }
                RetJsonMsg::SetRetMsg(iRet,pResponse,err);
                return iRet;

            }else{
                RetJsonMsg::SetRetMsg(-1,pResponse,result);
                return -1;
            }
        }

    }
    RetJsonMsg::SetRetMsg(-1,pResponse,"验证时异常");
    return -1;

}

int PhoneCodeBLL::Send(string sPhone,string sType,string sMsg,Json::Value& pResponse,string & err)
{
    int iRet  =-1;

    iRet = SendTTDMsgToAliyuncsSMS(sPhone,sType,sMsg,err);
    pResponse["sYunyin"] = "Ali";

    if( iRet != 0){
       iRet = SendTTDMsgToMiaoDiYun(sPhone, sType,sMsg,err);
        pResponse["sYunyin"] = "MiaoDi";
    }else{
        pResponse["sYunyin"] = "Ali";
    }
    return iRet;
}


//TTD秒嘀云 语音验证码
int PhoneCodeBLL::SendTTDYuyingToMiaoDiYun(const string& psPhone,string & err)
{
    //https://api.miaodiyun.com/20150822/call/voiceCode
    string sid = "a0fd4964bcef4f2fbd349a8f69af5066";
    string token = "5a0387a1fc73439893c8e52fd6e54941";
    string to = psPhone;        //短信接收端手机号码
    TConvert tConvert;

    //生成注册码
    int iCode = Random(100000,999999);


    char cTime[80];                                       //时间戳是当前系统时间，格式"yyyyMMddHHmmss"。时间戳有效时间为24小时，如：20140416142030
    //返回年月日时分秒
    struct tm *ptr;
    time_t lt;

    lt=time(0);
    ptr=localtime(&lt);

    sprintf(cTime,"%02d%02d%02d%02d%02d%02d",
            ptr->tm_year + 1900,
            ptr->tm_mon + 1,
            ptr->tm_mday,
            ptr->tm_hour,
            ptr->tm_min,
            ptr->tm_sec);


    string sig = Md5ToLower( sid  + token + string(cTime) );//使用MD5加密（账户Id + 账户授权令牌 + 时间戳）。其中账户Id和账户授权令牌根据url的验证级别对应主账户。，共32位（小写）


    string sUrl = "https://api.miaodiyun.com/20150822/call/voiceCode";

    //accountSid=a14f6bfd43ce44c9b019de57f4e2de4b&verifyCode=2596&called=13896543210&callDisplayNumber
    //   =18520639856&playTimes=3&timestamp=20150821100312&sig=a14f6bfd43ue44c9b019du57f4e2ee4r&respDataType=JSON
    string sTimeStamp = cTime;
    string callDisplayNumber = "13411367358";
    string sUrlPost = "accountSid="+sid+"&verifyCode="+tConvert.IntToStr(iCode)+"&called="+to + "&callDisplayNumber=" + callDisplayNumber +"&timestamp="+sTimeStamp+"&sig="+sig+"&respDataType=JSON";

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/x-www-form-urlencoded");

    appendlog(TTDLogger::LOG_INFO,"PhoneCodeBLL::SendTTDYuyingToMiaoDiYun sUrl=%s",sUrl.c_str());
    appendlog(TTDLogger::LOG_INFO,"PhoneCodeBLL::SendTTDYuyingToMiaoDiYun sUrlPost=%s",sUrlPost.c_str());


    //获取HTTP请求
    string sContent;
    TCurl tCurl;
    if ( 0 != tCurl.PostJosn(sUrl,httpHeaderVector,sUrlPost,sContent) ) {
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendTTDYuyingToMiaoDiYun sUrl=%s",sUrl.c_str());
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendTTDYuyingToMiaoDiYun sUrlPost=%s",sUrlPost.c_str());
        return -1;
    }


    //判断返回结果
    if (std::string::npos == sContent.find( "00000", 0 ) || std::string::npos != sContent.find( "\"failCount\": \"1\"", 0 )) {
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendMsgToMiaoDiYun sContent=%s",sContent.c_str());

        Json::Value jContent;
        tConvert.Str2Json(sContent,jContent);

        if( !jContent["respDesc"].isNull()){

            err = tConvert.Json2String(jContent["respDesc"]);

        }
        return -1;
    }


    PhoneCodeMapBLL::GetInstance()->SetPhoneCodeList(psPhone,tConvert.IntToStr(iCode));
    appendlog(TTDLogger::LOG_DEBUG,"PhoneCodeBLL::SendCode sPhone=%s,Code=%d",psPhone.c_str(),iCode);

    return 0;


}

//TTD秒嘀云
int PhoneCodeBLL::SendTTDMsgToMiaoDiYun(const string& psPhone, const string psType,const string& psMsg,string & err)
{
    string sid = "a0fd4964bcef4f2fbd349a8f69af5066";
    string token = "5a0387a1fc73439893c8e52fd6e54941";
    string to = psPhone;        //短信接收端手机号码
    string smsContent = "";
    TConvert tConvert;

    std::vector<std::string> resultVector;
    tConvert.Split(psMsg,",",resultVector);
    //生成注册码
    int iCode = Random(100000,999999);
    if( psType == "1"){
        //smsContent = "【天天兑】验证码:"+ tConvert.IntToStr(iCode)+",请勿向任何人提供您收到的短信验证码";
        smsContent = "【天天兑】验证码:"+ tConvert.IntToStr(iCode);
    }else if (psType == "2"){
        smsContent = "【天天兑】您在实体店["+resultVector[0]+"]消费["+resultVector[1]+"],单号:"+resultVector[2];
    }else if( psType == "3"){
        smsContent = "【天天兑】已有客户在天天兑商城购物了[" + psMsg +"]并已经付款，请您发货";
    }else if( psType == "4"){
        smsContent = "【天天兑】您开通商家"+ psMsg+"时涉嫌恶意违规操作,重复三次将可能冻结您的开店权限,请严格遵守操作规则,如有疑问请联系客服";
    }else if( psType == "5"){
        smsContent = "【天天兑】您开通商家"+ psMsg+"时上传资质文件不合格,系统已暂停商家交易功能,请严格遵守操作规则,如有疑问请联系客服";
    }else if( psType == "6"){
        smsContent = "【天天兑】"+ psMsg+",您的店铺开通时,上传的资质文件不合格,请严格遵守操作规则,如有疑问请联系客服";
    }else if( psType == "7"){
        smsContent = "【天天兑】已有客户在您的商店买下" + resultVector[0] + " 并已付款 ,订单号:"+ resultVector[1];
    }else if( psType == "8"){
        smsContent = "【天天兑】您在天天兑购买的" + resultVector[0]  + "已发货,(快递公司:" + resultVector[1] + ",快递单号:" + resultVector[2] + ")";
    }else if(psType == "9" ){
        smsContent = "【天天兑】" + resultVector[0] + "购买的" + resultVector[1] +  "已申请退货/款";
    }else if(psType == "10" ){
        smsContent = "【天天兑】" + resultVector[0]  + "购买的"  + resultVector[1] + "已确认";
    }else if(psType == "11" ){//恭喜您的商家平台已开通，平台号：{1}，密码：{2}
        smsContent = "【天天兑】恭喜您的商家平台已开通，平台号：" + resultVector[0]  + "，密码："  + resultVector[1];
    }else if(psType == "12" ){//你在（{1}店）消费{2}现金，单号{3}
        smsContent = "【天天兑】你在（" + resultVector[0]  + "店）消费"  + resultVector[1] + "现金，单号" + resultVector[2];
    }else if( psType == "13"){
        smsContent = "【天天兑】验证码:"+ tConvert.IntToStr(iCode) + "(开通商家验证码),开通人员可能会向您索要,勿向其他人泄露";
    }else if(psType == "14" ){
        smsContent = "【天天兑】已有用户反馈店铺(" + resultVector[0]  + ")的产品("  + resultVector[1] + ")价格虚高";
    }else if(psType == "15" ){
        smsContent = "【天天兑】您的商店（" + resultVector[0]  + "）变更折扣审核"  + resultVector[1] ;
    }else if(psType == "16" ){//恭喜您的商家平台已开通,请前往APP补全店铺资料，平台号：{1}，密码：{2}
        smsContent = "【天天兑】恭喜您的商家平台已开通,请前往APP补全店铺资料，平台号：" + resultVector[0]  + "，密码："  + resultVector[1] ;
    }


    char cTime[80];                                       //时间戳是当前系统时间，格式"yyyyMMddHHmmss"。时间戳有效时间为24小时，如：20140416142030
    //返回年月日时分秒
    struct tm *ptr;
    time_t lt;

    lt=time(0);
    ptr=localtime(&lt);

    sprintf(cTime,"%02d%02d%02d%02d%02d%02d",
            ptr->tm_year + 1900,
            ptr->tm_mon + 1,
            ptr->tm_mday,
            ptr->tm_hour,
            ptr->tm_min,
            ptr->tm_sec);
    //appendlog(TTDLogger::LOG_DEBUG,"cTime=%s",cTime);

    string sig = Md5ToLower( sid  + token + string(cTime) );//使用MD5加密（账户Id + 账户授权令牌 + 时间戳）。其中账户Id和账户授权令牌根据url的验证级别对应主账户。，共32位（小写）

    //短信
    string sUrl = "https://api.miaodiyun.com/20150822/industrySMS/sendSMS";
    string sUrlPost = "accountSid="+sid+"&smsContent="+smsContent+"&to="+to+"&timestamp="+string(cTime)+"&sig="+sig+"&respDataType=JSON";

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/x-www-form-urlencoded");



    //获取HTTP请求
    string sContent;
    TCurl tCurl;
    if ( 0 != tCurl.PostJosn(sUrl,httpHeaderVector,sUrlPost,sContent) ) {
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendMsgToMiaoDiYun sUrl=%s",sUrl.c_str());
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendMsgToMiaoDiYun sUrlPost=%s",sUrlPost.c_str());
        return -1;
    }

    //判断返回结果
    if (std::string::npos == sContent.find( "00000", 0 ) || std::string::npos != sContent.find( "\"failCount\": \"1\"", 0 )) {
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendMsgToMiaoDiYun sContent=%s",sContent.c_str());
        //{"respCode":"00023","respDesc":"开发者余额不足 "}
        Json::Value jContent;
        tConvert.Str2Json(sContent,jContent);
        if( sContent.find("respCode",0) != string::npos){//{"respCode":"00104"}
            string  sResCode= sContent.substr(13,5);
            if( sResCode == "00104"){
                return -104;
            }
        }
        if( !jContent["respDesc"].isNull()){

            err = tConvert.Json2String(jContent["respDesc"]);

        }
        return -1;
    }

    if(psType == "1" || psType == "13"){
        //存入队列
        PhoneCodeMapBLL::GetInstance()->SetPhoneCodeList(psPhone,tConvert.IntToStr(iCode));
        appendlog(TTDLogger::LOG_INFO,"PhoneCodeBLL::SendCode sPhone=%s,Code=%d",psPhone.c_str(),iCode);
    }
    return 0;

}


//生成指定位数的随机数,产生区间[a,b]上的随机数
int PhoneCodeBLL::Random( int a, int b )
{
    //ScopeLock lock(mMutex);
    usleep(1000);
    TConvert tCovnert;
    srand(tCovnert.GetCurrentTime());
    return ((double)rand()/RAND_MAX)*(b-a) + a;
}



//TTD阿里大鱼短信（阿里官方短信服务商）
int PhoneCodeBLL::SendTTDMsgToAliyuncsSMS(const string& psPhone, const string psType,const string& psMsg,string & err)
{

    string sUrl="https://eco.taobao.com/router/rest";
    string sAppsecret="a48ebe7ba452a7ae81914467dd1f8f2a";
    string sAppkey="23395708";

    pTopRequest pRequest = alloc_top_request();
    pTopResponse pResponse = NULL;
    pTaobaoClient pClient = alloc_taobao_client(sUrl.c_str(), sAppkey.c_str(), sAppsecret.c_str());
    set_api_name(pRequest,"alibaba.aliqin.fc.sms.num.send");
    add_param(pRequest,"extend","123456");
    add_param(pRequest,"sms_type","normal");
    add_param(pRequest,"sms_free_sign_name","天天兑");

    string sTemplateId;
    string sSms_param = "";
    TConvert tConvert;
    std::vector<std::string> resultVector;
    tConvert.Split(psMsg,",",resultVector);
    //生成注册码
    int iCode = Random(100000,999999);
    if( psType == "1"){
        sSms_param = "{\"code\":\""+ tConvert.IntToStr(iCode)+"\"}";
        //sTemplateId = "SMS_15060241";
        sTemplateId = "SMS_16385049";
    }else if (psType == "2"){//您在实体店[${shop}]消费[${money}],单号:${ordernum}
        sSms_param = "{\"shop\":\""+resultVector[0]+"\",\"money\":\""+resultVector[1]+"\",\"ordernum\":\""+resultVector[2] +"\"}";
        sTemplateId = "SMS_15105087";
    }else if( psType == "3"){//已有客户在天天兑商城购物了[${name}]并已经付款，请您发货
        sSms_param = "{\"name\":\"" + psMsg +"\"}";
        sTemplateId = "SMS_15160158";
    }else if( psType == "4"){//您开通商家${shop}时涉嫌恶意违规操作,重复三次将可能冻结您的开店权限,请严格遵守操作规则,如有疑问请联系客服
        sSms_param = "{\"shop\":\""+ psMsg+"\"}";
        sTemplateId = "SMS_15130126";
    }else if( psType == "5"){//您开通商家${shop}时上传资质文件不合格,系统已暂停商家交易功能,请严格遵守操作规则,如有疑问请联系客服
        sSms_param = "{\"shop\":\""+ psMsg+"\"}";
        sTemplateId = "SMS_15105088";
    }else if( psType == "6"){//${name},您的店铺开通时,上传的资质文件不合格,请严格遵守操作规则,如有疑问请联系客服
        sSms_param = "{\"name\":\""+ psMsg+"\"}";
        sTemplateId = "SMS_15115043";
    }else if( psType == "7"){//已有客户在您的商店买下${name} 并已付款 ,订单号:${order}
        sSms_param = "{\"name\":\"" + resultVector[0] + "\",\"order\":\""+ resultVector[1] + "\"}";
        sTemplateId = "SMS_15160176";
    }else if( psType == "8"){//您在天天兑购买的${name}已发货，(快递公司:${com} 快递单号:${num})
        sSms_param = "{\"name\":\"" + resultVector[0] + "\",\"com\":\""+ resultVector[1] +  "\",\"num\":\"" + resultVector[2] + "\" }";
        sTemplateId = "SMS_25560282";
    }else if(psType == "9" ){//${user}购买的${name}已申请退货/款
        sSms_param = "{\"user\":\"" + resultVector[0] + "\",\"name\":\""+ resultVector[1] +  "\" }";
        sTemplateId = "SMS_33415478";
    }else if(psType == "10" ){//${user}购买的${name}已确认
        sSms_param = "{\"user\":\"" + resultVector[0] + "\",\"name\":\""+ resultVector[1] +  "\" }";
        sTemplateId = "SMS_33445414";
    }else if(psType == "11" ){//恭喜您的商家平台已开通，平台号：${num}，密码：${pwd}
        sSms_param = "{\"num\":\"" + resultVector[0] + "\",\"pwd\":\""+ resultVector[1] +  "\" }";
        sTemplateId = "SMS_67296242";
    }else if(psType == "12" ){//你在（${name}店）消费${price}现金，单号${num}
        sSms_param = "{\"name\":\"" + resultVector[0] + "\",\"price\":\""+ resultVector[1] +  "\",\"num\":\"" + resultVector[2] + "\" }";
        sTemplateId = "SMS_67276458";
    }else if( psType == "13"){
        sSms_param = "{\"code\":\""+ tConvert.IntToStr(iCode)+"\"}";
        sTemplateId = "SMS_87675005";
    }else if(psType == "14" ){//已有用户反馈店铺(${shop})的产品(${name})价格虚高
        sSms_param = "{\"shop\":\"" + resultVector[0] + "\",\"name\":\""+ resultVector[1]  + "\" }";
        sTemplateId = "SMS_101180110";
    }else if(psType == "15" ){//您的商店(${shop})变更折扣审核(${msg})
        sSms_param = "{\"shop\":\"" + resultVector[0] + "\",\"msg\":\""+ resultVector[1]  + "\" }";
        sTemplateId = "SMS_109520327";
    }else if(psType == "16" ){//恭喜您的商家平台已开通,请前往APP补全店铺资料，平台号：${num}，密码：${pwd}
        sSms_param = "{\"num\":\"" + resultVector[0] + "\",\"pwd\":\""+ resultVector[1]  + "\" }";
        sTemplateId = "SMS_115205012";
    }else if(psType == "17" ){//商家开通成功通知
        sSms_param = "{}";
        sTemplateId = "SMS_135340116";
    }else if(psType == "18" ){//个代/合伙人接到一个待开通联盟商家任务
        sSms_param = "{}";
        sTemplateId = "SMS_135415124";
    }

    add_param(pRequest,"sms_param",sSms_param.c_str());

    add_param(pRequest,"rec_num",psPhone.c_str());
    add_param(pRequest,"sms_template_code",sTemplateId.c_str());
    pResponse = top_execute(pClient,pRequest,NULL);

    int iRet = -7;
    if(pResponse->code != 0){
        appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendTTDMsgToAliyuncsSMS ret code=%d msg:%s submsg:%s",pResponse->code,pResponse->msg,
                  pResponse->subMsg);
        err = pResponse->subMsg;
        if( err == "触发业务流控"){
            err = "发送短信次数受限";
        }


        pTopResponseIterator ite = init_response_iterator(pResponse);
        pResultItem pResultItem = alloc_result_item();
        while(parseNext(ite, pResultItem) == 0){
            appendlog(TTDLogger::LOG_ERROR,"PhoneCodeBLL::SendTTDMsgToAliyuncsSMS sContent=%s:%s",pResultItem->key,pResultItem->value);
        }
        destroy_response_iterator(ite);
        destroy_result_item(pResultItem);
    } else {
        iRet = 0;
        if(psType == "1" || psType == "13" ){
            //存入队列
            PhoneCodeMapBLL::GetInstance()->SetPhoneCodeList(psPhone,tConvert.IntToStr(iCode));
            appendlog(TTDLogger::LOG_INFO,"PhoneCodeBLL::SendCode sPhone=%s,Code=%d",psPhone.c_str(),iCode);
        }
    }

    destroy_top_request(pRequest);
    destroy_top_response(pResponse);
    destroy_taobao_client(pClient);

    //判断返回结果
    return iRet;

}

int PhoneCodeBLL::QuerySms()
{
    string url="https://eco.taobao.com/router/rest";//https://eco.taobao.com/router/rest
    string appsecret="a48ebe7ba452a7ae81914467dd1f8f2a";
    string appkey="23395708";

    pTopRequest pRequest = alloc_top_request();
    pTopResponse pResponse = NULL;
    pTaobaoClient pClient = alloc_taobao_client(url.c_str(), appkey.c_str(), appsecret.c_str());
    set_api_name(pRequest,"alibaba.aliqin.fc.sms.num.query");
    //add_param(pRequest,"biz_id","1234^1234");
    add_param(pRequest,"rec_num","13411367357");
    add_param(pRequest,"query_date","20171016");
    add_param(pRequest,"current_page","1");
    add_param(pRequest,"page_size","10");
    pResponse = top_execute(pClient,pRequest,NULL);
    printf("ret code:%d\n",pResponse->code);
    if(pResponse->code == 0){
        pTopResponseIterator ite = init_response_iterator(pResponse);
        pResultItem pResultItem = alloc_result_item();
        while(parseNext(ite, pResultItem) == 0){
            printf("%s:%s\n",pResultItem->key,pResultItem->value);
        }
        destroy_response_iterator(ite);
        destroy_result_item(pResultItem);
    }
    destroy_top_request(pRequest);
    destroy_top_response(pResponse);
    destroy_taobao_client(pClient);

}

