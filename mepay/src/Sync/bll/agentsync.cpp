#include "agentsync.h"
#include "TFile.h"

AgentSync::AgentSync()
{

}

void* AgentSync::SyncPerAgent(void*)
{
    TFile config;
    string v;
    config.GetValue("/tiyoume/agentSync.txt","upTimePerAgent",v);



    string sLastTime = v;
    string sUrl = "http://guanli.tymplus.com/GetDataInterface/PersonalAgent/GetPerInfoByLastTime.aspx";

    while(true){
        Json::Value jPost,jResponse;
        jPost["sLastTime"] = sLastTime;
        int iRet= Request::WalletHttpPost(sUrl,jPost,jResponse);
        string s = jResponse.toStyledString();
        if( iRet == 0){
            Json::Value jlistRecOrder = jResponse["listPerAgent"];
            for ( int i = 0; i < jlistRecOrder.size() ; i++){
                Json::Value jOrder = jlistRecOrder[i];
                sLastTime = SyncPerAgent(jOrder);
                if( sLastTime.empty()){
                    break;
                }
                config.SetValue("/tiyoume/agentSync.txt","upTimePerAgent",sLastTime);
            }
            if(jlistRecOrder.size() < 100 ){
                sleep(10);
                continue;
            }
        }
    }

}

void AgentSync::Start()
{

    pthread_t tid1;
    pthread_create( &tid1, NULL, SyncPerAgent, (void*)this );

    TFile config;
    string v;
    config.GetValue("/tiyoume/agentSync.txt","upTime",v);



    string sLastTime = v;
    string sUrl = "http://mepay.tymplus.com/GetDataInterface/Consume/ReportForm/GetOrderListById.aspx";

    while(true){
        Json::Value jPost,jResponse;
        jPost["sLastTime"] = sLastTime;
        int iRet= Request::WalletHttpPost(sUrl,jPost,jResponse);
        if( iRet == 0){
            Json::Value jlistRecOrder = jResponse["listRecOrder"];
            for ( int i = 0; i < jlistRecOrder.size() ; i++){
                Json::Value jOrder = jlistRecOrder[i];
                sLastTime = OrderInfo(jOrder);
                if( sLastTime.empty()){
                    break;
                }
                config.SetValue("/tiyoume/agentSync.txt","upTime",sLastTime);
            }
            if(jlistRecOrder.size() < 100 ){
                sleep(10);
                continue;
            }
        }
    }
}


string AgentSync::OrderInfo(const Json::Value & j)
{


    TConvert tConvert;
    OfflineOrder en;
    en.offLineDisCount  = tConvert.Json2Int( j["iOffLineDisCount"] );
    en.recType  = tConvert.Json2Int( j["iRecType"] );
    en.isFans  = tConvert.Json2Int( j["iIsFans"] );
    en.money  = tConvert.Json2Double( j["Money"] );
    en.notDividedMoney  = tConvert.Json2Double( j["dNotDividedMoney"] );

    en.coin  = tConvert.Json2Double( j["dShopCoin"] );
    en.remark  = tConvert.Json2String( j["sRemark"] );
    en.orderNo  = tConvert.Json2String( j["OrderNO"] );
    en.payType =  tConvert.Json2String( j["MoneyType"] );

    en.payState  = tConvert.Json2Int( j["Status"] );
    en.platformId  = tConvert.Json2Long( j["MerchantQrcodeID"] );
    en.shopName  = tConvert.Json2String( j["MerchantName"] );
    en.shopPhone  = tConvert.Json2String( j["MerchantPhone"] );
    en.zone  = tConvert.Json2String( j["MerchantAccount"] );
    en.payUserId  = tConvert.Json2Long( j["CustomerUserID"] );
    en.payNickName  = tConvert.Json2String( j["CustomerName"] );
    en.payPhone  = tConvert.Json2String( j["CustomerPhone"] );

    en.coinType  = tConvert.Json2Int( j["iShopCoinType"] );
    en.unPlatformDivided  = tConvert.Json2Double( j["dUnsettledPushPlatformDivided"] );
    en.unPushDivided  = tConvert.Json2Double( j["dUnsettledPushDivided"] );
    en.addTime  = tConvert.Json2String( j["ADDTIME"] );
    en.upTime  = tConvert.Json2String( j["UPTIME"] );

    if( en.addTime.find("T") != string::npos)
        en.addTime = en.addTime.substr(0,10) + " " + en.addTime.substr(11);


    if( en.upTime.find("T") != string::npos)
        en.upTime = en.upTime.substr(0,10) + " " + en.upTime.substr(11);

    if( 0 != mOfflineOrderDAL.Replace(en)){
        return "";
    }


    return en.upTime;

}

string AgentSync::SyncPerAgent(const Json::Value & j)
{
    PerAgentDAL mPerAgentDAL;
    TConvert tConvert;
    PerAgent en;
    en.activationState = tConvert.Json2Int( j["iActivationState"] );
    en.activationTime =  tConvert.Json2String( j["ActivationTime"] );
    en.addTime = tConvert.Json2String( j["ADDTIME"] );
    en.city = tConvert.Json2String( j["sWorkZone"] );
    en.dealTime = tConvert.Json2String( j["DealTime"] );
    en.extendType = tConvert.Json2Int( j["iExtendType"] );
    en.orderNo = tConvert.Json2String( j["sOrderNo"] );
    en.payCount =tConvert.Json2Int( j["iPayCount"] );
    en.payMoney =tConvert.Json2Double( j["dPayMoney"] );
    en.peragentName =tConvert.Json2String( j["sName"] );
    en.peragentPhone =tConvert.Json2String( j["sPhone"] );
    en.pushId = tConvert.Json2Long( j["lPushId"] );
    en.pushPhone = tConvert.Json2String( j["sPushPhone"] );
    en.pushZone = tConvert.Json2String( j["sExtendZone"] );
    en.state = 0;
    en.totalMoney = tConvert.Json2Double( j["dMoneyCount"] );
    en.trialDays = tConvert.Json2Int( j["iTrialDays"] );
    en.upTime = tConvert.Json2String( j["UPTIME"] );
    en.userId = tConvert.Json2Long( j["lUserId"] );

    if( en.addTime.find("T") != string::npos)
        en.addTime = en.addTime.substr(0,10) + " " + en.addTime.substr(11);


    if( en.upTime.find("T") != string::npos)
        en.upTime = en.upTime.substr(0,10) + " " + en.upTime.substr(11);

    if( 0 != mPerAgentDAL.Replace(en)){
        return "";
    }


    return en.upTime;
}
