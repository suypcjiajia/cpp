#ifndef GROUPDATADAL_H
#define GROUPDATADAL_H


/*
 *群资料mysql访问层
 * */

#include <string>
#include <vector>
#include "TypeDef.h"
#include "GroupDataEntity.h"
using namespace std;
/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/

class GroupDataDAL
{

public:

    int CreateTable();

    int IsExist(const long & plGroupId);

    int AddData( const GroupDataEntity & entity);

    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const long & plGroupId,GroupDataEntity & entity);

    long GetMaxGroupId();

    int UpGroupName(long lGroupId, string sNewName);

    int UpGroupHead(long lGroupId, string sStr);

    int UpMaster(long lGroupId, long lMaserId);

    int GetGroupIds(longs & lGroupIds, int iIndex, int iCount);

private:
    const string msTableName = "group_data";

};

#endif // GROUPDATADAL_H
