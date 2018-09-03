
#include "basebll.h"
#include "TConvert.h"
long BaseBLL::GetPlatformId(const string & sToken)
{
    Json::Value jsonValue;
    TConvert tConvert;
    int iRet = tConvert.Str2Json(sToken,jsonValue );
    if( jsonValue["lPlatformId"].isNull()){
        return -1;
    }
    return tConvert.Json2Long( jsonValue["lPlatformId"] );
}

