#include "dal/ShopInfoDAL.h"
#include "dal/KeySecretDAL.h"
#include "sha.h"
#include "TypeDef.h"
#include "com/Request.h"
#include <fstream>
#include "bll/agentsync.h"
//导出某一天区域粉丝
void ExportZoneFans(string day);
//导出区域粉丝 by start_day
void ExportZoneFansByStartDay(string start_day);
//批量发福利
void SendNoticeList();
//把钱包的个代信息，商家订单同步过来
void SyncAgent();

void UpdateWeixin();
int main(int argc, char *argv[])
{
    //定义日志输出路径
    TTDLogger::Logger::getInstance()->setPath(string("/log/Sync/").c_str());
    TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_DEBUG);


    string day = argv[1];
    ExportZoneFans(day);
}

void UpdateWeixin()
{
    ifstream infile;
    infile.open("./out.txt");   //将文件流对象与文件连接起来

    string s;
    TConvert tConvert;
    while(getline(infile,s))
    {

       ShopInfoDAL::GetInstance()->UpdateWeixinNum(s);

    }
    infile.close();             //关闭文件输入流
}



void ExportZoneFansByStartDay(string start_day)
{
    TConvert t;

    string sCurDay = t.GetCurrentDay();


    while( start_day <= sCurDay){
      ExportZoneFans(start_day);
      string time = start_day + " 00:00:01";
      long lt = t.StrToTime(time.c_str());
      start_day = t.TimeToStr(lt + 60*60*24);
      start_day = start_day.substr(0,10);

    }

}

//把钱包的个代信息，商家订单同步过来
void SyncAgent()
{
        TMultiMysqlDAL::SetMysqlAddr("rm-wz9c618cpa59o812l.mysql.rds.aliyuncs.com");
        TMultiMysqlDAL::SetMysqlUser("wallet");
        TMultiMysqlDAL::SetMysqlPwd("Wallet1234");



    //    //初始化要操作的数据库
        MysqlConnect::GetInstance()->Init("walletdb");




        AgentSync agentsync;
        agentsync.Start();


}

//批量发福利
void SendNoticeList()
{


        ifstream infile;
        infile.open("./1.txt");   //将文件流对象与文件连接起来

        string s;
        TConvert tConvert;
        while(getline(infile,s))
        {
            StringLst split;
            tConvert.Split(s,"sss",split);
            if(split.size() == 2){
                string sUrl = "http://120.25.129.101:23241/?sBusiness=Notice&sMethod=Send";
                TConvert tConvert;
                Json::Value jsonValue,jsonContent;
                Json::Value jRespons;
                jsonContent["sTitle"] = "天天兑福利";
                jsonContent["sText"] = split[1];//
                jsonContent["sPic"] = "";//
                jsonContent["sUrl "] = "";//
                jsonValue["lSendId"]= tConvert.LongToStr(28);
                jsonValue["iTarget"]= 2;//2 组播或单播
                jsonValue["sContent"]=jsonContent;
                jsonValue["sRevIds"][0]= split[0];
                jsonValue["iType"]=1;//

                Request::CppHttpPost(sUrl,jsonValue,jRespons);


            }
        }
        infile.close();             //关闭文件输入流


}


//导出某一天区域粉丝
void ExportZoneFans(string day)
{
    printf("%s\n",day.c_str());
    StringLst zones;
    ShopInfoDAL::GetInstance()->GetZones(zones);
    for( int i = 0; i < zones.size(); i ++){
        string zone = zones[i];
        LongLst platformIds;
        ShopInfoDAL::GetInstance()->GetPlatformIdsByZone(zone,platformIds);
        long count = ShopInfoDAL::GetInstance()->GetCountByZone(day,platformIds);
        if( count>0)
            appendlog(TTDLogger::LOG_DEBUG,"%s %d\n",zone.c_str(),count);
    }
}



