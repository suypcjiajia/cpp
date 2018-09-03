#ifndef PUBLICINFOENTITY_H
#define PUBLICINFOENTITY_H

#include <string>
#include <vector>
using namespace std;

struct PublicInfoEntity
{
    string sKey;
    long lSendId;
    long Id;
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    int iType;
    string sContent;
    string sReaded;
    string sRev;
};

typedef vector<PublicInfoEntity> PublicInfoList;

struct PublicInfoQueryEntity
{
};

#endif // PUBLICINFOENTITY_H
