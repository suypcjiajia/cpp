#ifndef GROUPDATAENTITY_H
#define GROUPDATAENTITY_H

#include <string>
#include <vector>
using namespace std;

struct GroupDataEntity
{
    long lGroupId;           //群Id
    long lAddTime;          //添加时间
    long lUpTime;           //更新时间
    string sGroupName;//群名
    long lMasterId;//群主
    string sGroupHead; //群头像(用,号分隔)

};

typedef vector<GroupDataEntity> GroupDataList;

struct GroupDataQueryEntity
{
};

#endif // GROUPDATAENTITY_H
