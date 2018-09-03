//#include "ServerBLL.h"
//#include <unistd.h>
//#include <stdlib.h>
//#include <sstream>
//#include "log/Logger.h"
//#include "TFile.h"
//#include "TConvert.h"
//#include "bll/ZoneAgnetBLL.h"
//#include "dal/ZoneAgentDAL.h"
//#include "dal/ShopInfoDAL.h"
//#include "dal/WalletAccountDAL.h"
//#include "TBLL.h"
//#include "com/EvhtpSvr.h"
//#include "com/RetJsonMsg.hpp"
//#include "com/PostsAes.h"
//using namespace std;
//ServerBLL* ServerBLL::mInstance = NULL;
//ServerBLL* ServerBLL::GetInstance()
//{
//    if (mInstance == NULL){
//        mInstance = new ServerBLL;
//    }
//    return mInstance;
//}

//int ServerBLL::Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse){
//    int iRet = -1;
//    if( "UpdatePhone" == psMethod){

//        iRet = UpdatePhone(pJson,pResponse);

//    }else{
//      RetJsonMsg::SetRetMsg(-19,pResponse);
//    }
//    return iRet;
//}



////初始化
//int ServerBLL::Init()
//{
//    return 0;
//}

////更换手机号
//int ServerBLL::UpdatePhone( const  Json::Value  & pJson, Json::Value & pResponse )
//{
//    TConvert tConvert;
//    int iRet = -1;

//    if( pJson["sAccount"].isNull()    || pJson["sPassWord"].isNull()  || pJson["sCode"].isNull()  || pJson["sNewAccount"].isNull()  ){
//        RetJsonMsg::SetRetMsg(-15,pResponse);
//        return -15;
//    }
//    string sAccount = tConvert.Json2String( pJson["sAccount"]);
//    string sPassWord = tConvert.Json2String( pJson["sPassWord"]);
//    string sCode = tConvert.Json2String( pJson["sCode"]);
//    string sNewAccount = tConvert.Json2String( pJson["sNewAccount"]);

//    iRet  = UpdateAccount(sAccount,sPassWord,sNewAccount,sCode);
//    if( iRet != 0){
//        RetJsonMsg::SetRetMsg(iRet,pResponse);
//        return iRet;
//    }

//    //以下手机号只是用来看，手机号不会参与逻辑计算，所以不会验证新手机号
//    ZoneAgentDAL::GetInstance()->UpdatePhone(sAccount, sNewAccount);

//    pResponse["iRet"] = 0;
//    return 0;
//}

////更换用户服务的手机号
//int ServerBLL::UpdateAccount(const string& psAccount, const string& psPassword, const string& psNewAccount, const string& psCode)
//{
//    //参数转换
//    TConvert tConvert;
//    Json::Value jsonValue;
//    jsonValue["sAccount"]=psAccount;
//    jsonValue["sPassword"]=psPassword;
//    jsonValue["sNewAccount"]=psNewAccount;
//    jsonValue["sCode"]=psCode;

//    //URL
//    string sUrl = "http://" + TBLL::GetInstance()->GetPhonesmsIP() + ":23241/?sBusiness=UserRegister&sMethod=UpdateAccount";

//    //获取HTTP请求
//    PostsAes postsAes;
//    string sReturn = postsAes.HttpsPost(sUrl,jsonValue.toStyledString());
//    Json::Value jsonReturn;
//    Json::Reader reader;
//    if (!reader.parse(sReturn, jsonReturn)){
//        return -1;
//    }
//    int iRet = tConvert.Json2Int(jsonReturn["iRet"]);
//    return iRet;
//}
