#ifndef RetJsonMsg_H
#define RetJsonMsg_H

#include"json/json.h"
#include "TConvert.h"

class RetJsonMsg
{
public:

    static void SetRetMsg2(int piID, Json::Value & pRetJson,string msg = "")
    {
        TConvert tConvert;

        if( 0 == piID ){
            pRetJson["state"] = piID ;
            if( msg.empty()){
                pRetJson["msg"] = "成功";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -2 == piID ){
            pRetJson["state"] = piID;
            pRetJson["result"] = "Token验证不通过";

            if( msg.empty()){
                pRetJson["msg"] = "Token验证不通过";
            }else{
                pRetJson["msg"] = msg;
            }
        }
        else if( -4 == piID){
            //数据库访问错误
            pRetJson["state"] = piID;
            pRetJson["result"] = "数据库访问错误";

            if( msg.empty()){
                pRetJson["msg"] = "数据库访问错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -5 == piID ){

            pRetJson["state"] = piID;
            pRetJson["result"] = "数据不存在";

            if( msg.empty()){
                pRetJson["msg"] = "数据不存在";
            }else{
                pRetJson["msg"] = msg;
            }

        }
        else if( -6 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "密码错误";

            if( msg.empty()){
                pRetJson["msg"] = "密码错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -13 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "不支持此请求类型";

            if( msg.empty()){
                pRetJson["msg"] = "不支持此请求类型";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -15 == piID){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "POST参数错误";


            if( msg.empty()){
                pRetJson["msg"] = "POST参数错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }
        else if( -20 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "JOSN格式错误";


            if( msg.empty()){
                pRetJson["msg"] = "JOSN格式错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -21 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "数据不合法";

            if( msg.empty()){
                pRetJson["msg"] = "数据不合法";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -301 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "验证码已过期";

            if( msg.empty()){
                pRetJson["msg"] = "验证码已过期";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -302 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "验证码已使用";


            if( msg.empty()){
                pRetJson["msg"] = "验证码已使用";
            }else{
                pRetJson["msg"] = msg;
            }

        }else if( -303 == piID ){
            pRetJson["state"] =  piID;
            pRetJson["result"] = "验证码无效";

            if( msg.empty()){
                pRetJson["msg"] = "验证码无效";
            }else{
                pRetJson["msg"] = msg;
            }
        }
        else{
            pRetJson["state"] =  piID;
            pRetJson["result"] = "未知错误";

            if( msg.empty()){
                pRetJson["msg"] = "未知错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }
    }

    //根据错误ID获取错误描述
    static void SetRetMsg(int piID, Json::Value & pRetJson,string msg = "")
    {
        if( 0 == piID ){
            pRetJson["iRet"] =  piID;
            if( msg.empty()){
                pRetJson["sMsg"] = "成功";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( 1 == piID ){
            pRetJson["iRet"] = piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "通过APNS推送了";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -1 == piID ){
            pRetJson["iRet"] = piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "未知错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -2 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -3 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据已经存在";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -4 ==piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据库访问错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -5 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据不存在";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -6 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "密码错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -7 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "发送手机短信验证码失败";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -8 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "手机短信验证码不存在";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -9 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "已经注册过了";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -10 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "正在注册中";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -11 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "密码更改验证错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -12 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "不支持此业务";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -13 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "不支持此请求类型";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -14 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "安全检查错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -15 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "POST参数错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -16 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "错误的数据操作";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -17 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "签名错误";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -18 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "余额不足";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -19 == piID ){
            pRetJson["iRet"] = piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "方法错误";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -20 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "JOSN格式错误";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -21 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据不合法";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -22 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "获取数据库连接时失败";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -23 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "未注册";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -100 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "推广人不存在";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -101 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "平台号不存在";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -102 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "店名已存在";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -103 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "没激活或认证不通过";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -104 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "当天给同一手机号发送短信的条数太多";
            }else{
                pRetJson["sMsg"] = msg;
            }


        }else if( -105 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "不是个代无法开店";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -120 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "已经是好友了";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -121 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "拉黑了不能申请";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -122 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "拉黑状态下不能操作";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -123 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "操作对像不能是自己";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -124 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "操作对像不能是系统号";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -131 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "超过2分钟了不能被撤回";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -132 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "红包已经被拆过了";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -133 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "找不到对应的红包";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -134 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "红包已过期";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -135 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "不能撤销红包";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( -136 == piID ){
            pRetJson["iRet"] =  piID;


            if( msg.empty()){
                pRetJson["sMsg"] = "你跟对方还不是好友发送消息失败";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -137 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "对方已经拒收你的消息";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -140 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "已经是群的成员";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -141 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "不是群主，没有权限";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -142 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "三人才能成群";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -143 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "你不是群成员,无法操作";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -144 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "邀请者不是群成员,无法操作";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -201 == piID){

            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "剩余手数不足";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }else if( -202 == piID){

            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "均价发生变化，请刷新";
            }else{
                pRetJson["sMsg"] = msg;
            }
        } else if( -203 == piID){

            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "获取粉丝失败";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }
        else if( -999 == piID ){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "ICE异常";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }else if( 1062 == piID){
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "数据库重复键";
            }else{
                pRetJson["sMsg"] = msg;
            }
        }
        else{
            pRetJson["iRet"] =  piID;

            if( msg.empty()){
                pRetJson["sMsg"] = "未知异常";
            }else{
                pRetJson["sMsg"] = msg;
            }

        }
    }

};

#endif // RetJsonMsg_H
