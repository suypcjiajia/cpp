#ifndef BASEBLL_H
#define BASEBLL_H
#include <string>
#include"json/json.h"
#include "com/EvhtpSvr.h"
using namespace std;
class BaseBLL
{
public:
    virtual int Process(const string & psMethod,const  Json::Value  & pJson, Json::Value & pResponse ) = 0;
    virtual int Process2(EvhtpSvr * pEvhtp,const  Json::Value  & pJson, Json::Value & pResponse ){return 0;}
    virtual int Init() = 0;
protected:
    long GetPlatformId(const string & sToken);


};

#endif // BASEBLL_H

