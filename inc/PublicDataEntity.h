#ifndef PUBLICDATAENTITY_H
#define PUBLICDATAENTITY_H

#include <string>
#include <vector>
using namespace std;

struct PublicDataEntity
{
    long Id;           //公众号Id
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    string sName;//公众号名
    long lMasterId;//公众号主人
    string sHead; //头像

};

typedef vector<PublicDataEntity> PublicDataList;

struct PublicDataQueryEntity
{
};

#endif // PUBLICDATAENTITY_H
