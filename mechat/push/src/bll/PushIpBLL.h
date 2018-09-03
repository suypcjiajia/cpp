#ifndef PushIpBLL_H
#define PushIpBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include "json/json.h"
#include "dal/PushIpDAL.h"
using namespace std;

class PushIpBLL
{
public:
    //初始化
    int Init();
    //方法调用
    int Process( const string& psMethod, const Json::Value& pJson, Json::Value& pResponse);
    //推送机-更新
    int Replace( const  Json::Value  & pJson ,Json::Value & pResponse);
    //推送机-查询行数据
    int GetEntity( const  Json::Value  & pJson  ,Json::Value & pResponse);
private:    
    //数据库对像
    PushIpDAL mPushIpDAL;
};

#endif // PushIpBLL_H
